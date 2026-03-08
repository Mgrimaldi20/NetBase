#ifndef _NETBASE_SYS_WIN32_IO_IOCONTEXT_H_
#define _NETBASE_SYS_WIN32_IO_IOCONTEXT_H_

#include <cstddef>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <string>
#include <string_view>
#include <span>
#include <vector>
#include <list>
#include <unordered_map>

#include "framework/Log.h"

#include "../net/Socket.h"
#include "../net/WinSockAPI.h"

#include "OverlappedIO.h"

class IOContext : public std::enable_shared_from_this<IOContext>
{
public:
	IOContext(
		Log &log,
		std::list<std::shared_ptr<IOContext>> &ioctxlist,
		std::mutex &ioctxlistmtx,
		std::unordered_map<std::string, std::weak_ptr<IOContext>> &clientidmap,
		std::mutex &clientidmapmtx
	);

	~IOContext();

	void Initialize();	// create a wrapper for this class that calls this in the constructor and then use that wrapper in the server code instead of this class directly

	void SetListIter(std::list<std::shared_ptr<IOContext>>::iterator listiter) noexcept;

	void PostRecv();
	void PostSend(std::span<const std::byte> data);
	void CloseClient();

	void CancelOverlappedIO() noexcept;

	Socket &GetAcceptSocket() noexcept;

	std::string_view GetClientID() const noexcept;
	void SetClientID(std::string_view id) noexcept;

	OverlappedIO &GetAcceptOverlapped() noexcept;

	std::atomic<bool> &GetConnected() noexcept;
	void SetConnected(bool val) noexcept;

	std::atomic<bool> &GetRecving() noexcept;
	void SetRecving(bool val) noexcept;

	std::atomic<bool> &GetSending() noexcept;
	void SetSending(bool val) noexcept;

	std::vector<std::byte> &GetIncomingBuffer() noexcept;
	std::vector<std::byte> &GetAcceptBuffer() noexcept;

private:
	static constexpr size_t NET_MAX_BUFFER_SIZE = 8192;

	Socket acceptsocket;

	std::string clientid;

	OverlappedIO acceptov;
	OverlappedIO recvov;
	OverlappedIO sendov;

	WSABUF recvwsabuf;
	WSABUF sendwsabuf;

	std::chrono::milliseconds timeout;
	std::condition_variable iocv;
	std::mutex iomtx;

	std::atomic<bool> connected;
	std::atomic<bool> recving;
	std::atomic<bool> sending;

	std::vector<std::byte> buffer;
	std::vector<std::byte> outgoing;
	std::vector<std::byte> acceptbuffer;

	std::list<std::shared_ptr<IOContext>>::iterator iter;

	std::atomic<bool> closing;

	Log &log;

	std::list<std::shared_ptr<IOContext>> &ioctxlist;
	std::mutex &ioctxlistmtx;
	std::unordered_map<std::string, std::weak_ptr<IOContext>> &clientidmap;
	std::mutex &clientidmapmtx;
};

#endif
