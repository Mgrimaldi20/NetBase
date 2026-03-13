#include "asio/write.hpp"
#include "asio/use_awaitable.hpp"

#include "Session.h"

Session::Session(asio::ip::tcp::socket socket, std::shared_ptr<Log> log)
	: socket(std::move(socket)),
	log(log)
{
}

Session::~Session()
{
}

asio::awaitable<void> Session::Start()
{
	try
	{
		while (true)
		{
			char buffer[1024];
			char response[1024] = "Hello client!";

			std::size_t n = co_await socket.async_read_some(asio::buffer(buffer), asio::use_awaitable);
			(void)n;
			// parse, handle, respond...
			co_await asio::async_write(socket, asio::buffer(response), asio::use_awaitable);
		}
	}

	catch (const std::exception &e)
	{
		log->Error("Session error: {}", e.what());
	}
}
