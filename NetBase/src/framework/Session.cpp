#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/write.hpp"
#include "asio/use_awaitable.hpp"

#include "Session.h"

Session::Session(asio::ip::tcp::socket socket, std::shared_ptr<Log> log)
	: writequeue(),
	socket(std::move(socket)),
	log(log)
{
	log->Info("New session started with client: {}", this->socket.remote_endpoint().address().to_string());
}

Session::~Session()
{
	log->Info("Session ended with client: {}", socket.remote_endpoint().address().to_string());
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
			log->Debug("Received message from client {}: [{} bytes]: {}", socket.remote_endpoint().address().to_string(), n, message);

			writequeue.push_back(message);
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session error: {}", e.what());
	}
}

asio::awaitable<void> Session::Writer()
{
	try
	{
		while (socket.is_open())
		{
			if (writequeue.empty())
				continue;

			co_await asio::async_write(socket, asio::buffer(writequeue.front()), asio::use_awaitable);
			writequeue.pop_front();
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session error: {}", e.what());
	}
}
