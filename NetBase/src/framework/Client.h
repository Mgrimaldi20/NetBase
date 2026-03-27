#ifndef _NETBASE_FRAMEWORK_CLIENT_H_
#define _NETBASE_FRAMEWORK_CLIENT_H_

#include <string_view>
#include <string>
#include <memory>

/*
* Class: Client
* An interface representing a client object in the system, and its properties.
* Derived classes should implement the Send() function for inter-client communication.
* 
*	Send: Pure virtual function, designed for sending data
*/
class Client
{
public:
	Client() = default;
	virtual ~Client() = default;

	virtual std::string_view GetAddr() = 0;
	virtual void Send(std::shared_ptr<std::string> message) = 0;
};

#endif
