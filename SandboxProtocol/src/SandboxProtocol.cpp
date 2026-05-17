#include <memory>

#include "ClientAPIImpl.h"
#include "ParserImpl.h"

__declspec(dllexport) std::shared_ptr<ClientAPI> GetClientAPI(std::shared_ptr<NetBaseAPI> netbaseapi)
{
	return std::make_shared<ClientAPIImpl>(
		netbaseapi,
		std::make_shared<ParserImpl>(),
		"SandboxProtocol"
	);
}
