#ifndef _NETBASE_SYS_DYNAMICLIBRARY_H_
#define _NETBASE_SYS_DYNAMICLIBRARY_H_

#include <any>
#include <string>
#include <filesystem>
#include <memory>

/*
* Class: DynamicLibrary
* An abstract interface for a systems Dynamic Library format.
* This interface should be implemented for each OS as its system dependent.
* 
*	GetSymbol: Pure virtual function, designed to get the function from the name supplied in the dynamic lib
*	CreateDynamicLibrary: Factory function to be implemented on each OS to return a sepcific impl of this interface
*/
class DynamicLibrary
{
public:
	virtual ~DynamicLibrary() = default;

	virtual std::any GetSymbol(const std::string &funcname) = 0;

	static std::unique_ptr<DynamicLibrary> CreateDynamicLibrary(std::filesystem::path fullpath);

protected:
	DynamicLibrary(std::filesystem::path fullpath)
		: fullpath(fullpath) {}

	std::filesystem::path fullpath;
};

#endif
