#ifndef _NETBASE_SYS_WIN32_IO_IOCOMPLETIONPORT_H_
#define _NETBASE_SYS_WIN32_IO_IOCOMPLETIONPORT_H_

#include "../net/WinSockAPI.h"
#include "../net/Socket.h"

class IOCompletionPort
{
public:
	IOCompletionPort();
	~IOCompletionPort();

	bool UpdateIOCompletionPort(Socket &socket, ULONG_PTR completionkey) const;
	bool GetQueuedCompletionStatus(unsigned long *iosize, unsigned long long *completionkey, WSAOVERLAPPED **wsaoverlapped) const;
	bool PostQueuedQuitStatus();

private:
	HANDLE iocp;
};

#endif
