#include <chrono>

#include "../Asio.h"

#include "Session.h"

Session::Session(asio::ip::tcp::socket socket, std::shared_ptr<Log> log)
	: writequeue(),
	clientaddr(socket.remote_endpoint().address().to_string()),
	timer(socket.get_executor()),
	socket(std::move(socket)),
	log(log)
{
	log->Info("New session started with client: {}", clientaddr);
	timer.expires_at(std::chrono::steady_clock::time_point::max());
}

Session::~Session()
{
	log->Info("Session ended with client: {}", clientaddr);
}

void Session::Start()
{
	asio::co_spawn(
		socket.get_executor(),
		[self = shared_from_this()] { return self->Reader(); },
		asio::detached
	);

	asio::co_spawn(
		socket.get_executor(),
		[self = shared_from_this()] { return self->Writer(); },
		asio::detached
	);
}

asio::awaitable<void> Session::Reader()
{
	try
	{
		while (true)
		{
			std::string message;
			message.resize(1024);

			std::size_t n = co_await socket.async_read_some(asio::buffer(message, message.size()), asio::use_awaitable);
			log->Debug("Received message from client {}: [{} bytes]: {}", clientaddr, n, message);

			writequeue.push_back(message);
			timer.cancel_one();
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session error: {}", e.what());
		Close();
	}
}

asio::awaitable<void> Session::Writer()
{
	try
	{
		while (socket.is_open())
		{
			if (writequeue.empty())
			{
				asio::error_code ec;
				co_await timer.async_wait(asio::redirect_error(asio::use_awaitable, ec));
			}

			else
			{
				co_await asio::async_write(socket, asio::buffer(writequeue.front()), asio::use_awaitable);
				writequeue.pop_front();
			}
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session error: {}", e.what());
		Close();
	}
}

void Session::Close()
{
	socket.close();
	timer.cancel();
}
