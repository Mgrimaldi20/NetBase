#include <memory>

#include "ClientAPIImpl.h"
#include "ParserImpl.h"

extern "C" __declspec(dllexport) ClientAPI *GetClientAPI(NetBaseAPI *netbaseapi)
{
	static ClientAPIImpl clientapi(
		std::shared_ptr<NetBaseAPI>(netbaseapi),
		std::make_shared<ParserImpl>(),
		"SandboxProtocol"
	);

	return &clientapi;
}
