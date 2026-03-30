#ifndef _NETBASE_FRAMEWORK_SESSION_H_
#define _NETBASE_FRAMEWORK_SESSION_H_

#include <memory>
#include <string>
#include <queue>
#include <vector>
#include <string_view>

#include "Asio.h"
#include "Channel.h"
#include "Log.h"

#include "Client.h"

/*
* Class: Session
* A session that represents a clients connection and all active session information.
* Handles reading from and writing to channels that the client is connected to.
* Implements the client interface, the Session is a Client.
* 
*	Start: Creates the Reader and Writer coroutines used when a client connects
*	GetAddr: Implementation of the Client interface to return the clients address
*	Send: Implementation of the Client interface function to broadcast on a channel
*/
class Session : public Client, public std::enable_shared_from_this<Session>
{
public:
	Session(asio::ip::tcp::socket socket, std::shared_ptr<Log> log);
	~Session();

	void Start();

	std::string_view GetAddr() override final;
	void Send(std::shared_ptr<std::string> message) override final;

private:
	asio::awaitable<void> Reader();
	asio::awaitable<void> Writer();

	void Close();

	std::queue<std::shared_ptr<std::string>> writequeue;
	std::string clientaddr;

	asio::strand<asio::any_io_executor> strand;
	asio::steady_timer timer;

	std::vector<std::shared_ptr<Channel>> joinedchannels;

	asio::ip::tcp::socket socket;
	std::shared_ptr<Log> log;
};

#endif
