#include <chrono>
#include <exception>
#include <system_error>

#include "Session.h"

Session::Session(
	asio::ip::tcp::socket socket,
	std::shared_ptr<CmdDispatcher> dispatcher,
	ClientAPI::Parser &parser,
	std::shared_ptr<Log> log
)
	: writequeue(),
	clientaddr(socket.remote_endpoint().address().to_string()),
	timer(socket.get_executor()),
	socket(std::move(socket)),
	dispatcher(dispatcher),
	parser(parser),
	log(log)
{
	timer.expires_at(std::chrono::steady_clock::time_point::max());

	log->Info("New session started with Client: {}", clientaddr);
}

Session::~Session()
{
	log->Info("Session ended with Client: {}", clientaddr);
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

std::string_view Session::GetAddr()
{
	return clientaddr;
}

void Session::Send(std::string message)
{
	bool empty = writequeue.empty();
	writequeue.push(std::move(message));

	if (empty)
		timer.cancel_one();
}

asio::awaitable<void> Session::Reader()
{
	try
	{
		while (true)
		{
			std::string message;
			message.resize(1024);

			asio::error_code ec;

			std::size_t n = co_await socket.async_read_some(
				asio::buffer(message, message.size()),
				asio::redirect_error(asio::use_awaitable, ec)
			);

			if (ec == asio::error::eof)
				break;

			if (ec)
				throw std::system_error(ec);

			log->Debug("Received message from Client {}: [{} bytes] :: {}", clientaddr, n, message);

			message.resize(n);

			ClientAPI::Parser::ParsedCmd parsedcmd = parser.get().Parse(message);
			dispatcher->Dispatch(shared_from_this(), std::move(parsedcmd));
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session Reader error: {}", e.what());
	}

	Close();
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

				continue;
			}

			std::string message = std::move(writequeue.front());
			writequeue.pop();

			co_await asio::async_write(socket, asio::buffer(message), asio::use_awaitable);

			log->Debug("Wrote message to Client {}: [{} bytes] :: {}", clientaddr, message.size(), message);
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session Writer error: {}", e.what());
	}

	Close();
}

void Session::Close()
{
	socket.close();
	timer.cancel();
}
