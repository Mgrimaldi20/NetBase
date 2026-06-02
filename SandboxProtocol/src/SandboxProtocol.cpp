#include <memory>

#include "ClientAPIImpl.h"

extern "C" __declspec(dllexport) ClientAPI *GetClientAPI(NetBaseAPI *netbaseapi)
{
	static ClientAPIImpl clientapi(netbaseapi, "SandboxProtocol");
	return &clientapi;
}
