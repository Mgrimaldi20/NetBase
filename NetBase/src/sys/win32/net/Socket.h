#ifndef _NETBASE_SYS_WIN32_NET_SOCKET_H_
#define _NETBASE_SYS_WIN32_NET_SOCKET_H_

#include "../io/OverlappedIO.h"

#include "WinSockAPI.h"

class Socket
{
public:
	static constexpr std::string_view NET_DEFAULT_PORT = "5001";

	Socket();
	~Socket();

	void Bind(std::string_view port) const;
	void Listen() const;

	bool SetSocketOption(const char *optval, int optlen, int level = SOL_SOCKET, int optname = SO_UPDATE_ACCEPT_CONTEXT) const;

	int Send(WSABUF &wsabuf, OverlappedIO &overlapped);
	int Recv(WSABUF &wsabuf, OverlappedIO &overlapped);

	void CancelIO(OverlappedIO &overlapped) noexcept;

	SOCKET &GetSocket() noexcept;

private:
	SOCKET socket;
};

#endif
