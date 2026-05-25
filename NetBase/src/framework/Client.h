#ifndef __NETBASE_FRAMEWORK_CLIENT_H__
#define __NETBASE_FRAMEWORK_CLIENT_H__

#include <string_view>
#include <string>

/*
* Class: Client
* An interface representing a client object in the system, and its properties.
* Derived classes should implement the Send() function for inter-client communication.
* 
*	GetAddr: Pure virtual function, designed to get the IP of the client
*	Send: Pure virtual function, designed for sending data
*/
class Client
{
public:
	Client() = default;
	virtual ~Client() = default;

	virtual std::string_view GetAddr() = 0;
	virtual void Send(std::string message) = 0;
};

#endif
