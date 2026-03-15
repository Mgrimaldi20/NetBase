#ifndef _NETBASE_FRAMEWORK_SESSION_H_
#define _NETBASE_FRAMEWORK_SESSION_H_

#include <memory>
#include <string>
#include <deque>

#include "asio/awaitable.hpp"
#include <asio/steady_timer.hpp>

#include "asio/ip/tcp.hpp"
#include "asio/ip/udp.hpp"

#include "Log.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(asio::ip::tcp::socket socket, std::shared_ptr<Log> log);
	~Session();

	void Start();

private:
	asio::awaitable<void> Reader();
	asio::awaitable<void> Writer();

	void Close();

	std::deque<std::string> writequeue;
	std::string clientaddr;
	asio::steady_timer timer;
	asio::ip::tcp::socket socket;
	std::shared_ptr<Log> log;
};

#endif
