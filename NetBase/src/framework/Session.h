#ifndef __NETBASE_FRAMEWORK_SESSION_H__
#define __NETBASE_FRAMEWORK_SESSION_H__

#include <memory>
#include <string>
#include <queue>
#include <string_view>
#include <functional>

#include "NetBaseAPI.h"

#include "Asio.h"
#include "Channel.h"
#include "CmdDispatcher.h"

#include "log/Log.h"

#include "Client.h"

/*
* Class: Session
* A session that represents a clients connection and all active session information.
* Handles reading from and writing to channels that the client is connected to.
* Implements the client interface, the session is a client.
* 
*	Start: Creates the Reader and Writer coroutines when a client connects
*	GetAddr: Returns the clients IP address
*	Send: Broadcasts a message on a channel
*/
class Session : public Client, public std::enable_shared_from_this<Session>
{
public:
	Session(
		asio::ip::tcp::socket socket,
		std::shared_ptr<CmdDispatcher> dispatcher,
		ClientAPI::Parser &parser,
		std::shared_ptr<Log> log
	);

	virtual ~Session();

	void Start();

	std::string_view GetAddr() override final;
	void Send(std::string message) override final;

private:
	asio::awaitable<void> Reader();
	asio::awaitable<void> Writer();

	void Close();

	std::queue<std::string> writequeue;
	std::string clientaddr;

	asio::steady_timer timer;

	asio::ip::tcp::socket socket;
	std::shared_ptr<CmdDispatcher> dispatcher;
	std::reference_wrapper<ClientAPI::Parser> parser;
	std::shared_ptr<Log> log;
};

#endif
