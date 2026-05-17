# CMakeList.txt : CMake project for NetBase, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.25)

# Add source to this project's executable
add_executable(NetBase)

project(NetBase LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set_property(TARGET NetBase PROPERTY CXX_STANDARD 23)

# Set the NetBase version
target_compile_definitions(NetBase PRIVATE NETBASE_VERSION=1)
target_compile_definitions(NetBase PRIVATE NETBASE_EXPORTS)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(NetBase PRIVATE NETBASE_DEBUG)
endif()

if(WIN32)
	target_compile_definitions(NetBase PRIVATE UNICODE _UNICODE)
	target_compile_definitions(NetBase PRIVATE _WIN32_WINNT=0x0601)
endif()

if(MSVC)
	target_compile_definitions(NetBase PRIVATE NETBASE_MSVC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_definitions(NetBase PRIVATE NETBASE_CLANG)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_definitions(NetBase PRIVATE NETBASE_GCC)
endif()

if (WIN32)
	target_compile_definitions(NetBase PRIVATE NETBASE_WIN32)
elseif (APPLE)
	target_compile_definitions(NetBase PRIVATE NETBASE_APPLE)
elseif (UNIX)
	target_compile_definitions(NetBase PRIVATE NETBASE_LINUX)
endif()

# Specify the common OS independent source files for the NetBase project
target_sources(NetBase PRIVATE
	"src/NetBaseAPI.h"
	"src/NetBaseAPIImpl.h"
	"src/NetBaseAPIImpl.cpp"
	"src/Main.cpp"
	"src/sys/DynamicLibrary.h"
	"src/framework/Asio.h"
	"src/framework/Asio.cpp"
	"src/framework/Server.h"
	"src/framework/Server.cpp"
	"src/framework/Client.h"
	"src/framework/Session.h"
	"src/framework/Session.cpp"
	"src/framework/Channel.h"
	"src/framework/Channel.cpp"
	"src/framework/ChannelManager.h"
	"src/framework/ChannelManager.cpp"
	"src/framework/CmdDispatcher.h"
	"src/framework/CmdDispatcher.cpp"
	"src/framework/log/Log.h"
	"src/framework/log/Log.cpp"
	"src/framework/log/entry/Entry.h"
	"src/framework/log/entry/Entry.cpp"
	"src/framework/log/sink/Sink.h"
	"src/framework/log/sink/text/TextSink.h"
	"src/framework/log/sink/text/file/FileSink.h"
	"src/framework/log/sink/text/file/FileSink.cpp"
	"src/framework/log/sink/text/console/ConsoleSink.h"
	"src/framework/log/sink/text/console/ConsoleSink.cpp"
	"src/framework/log/formatter/text/TextFormatter.h"
	"src/framework/log/formatter/text/basic/BasicTextFormatter.h"
	"src/framework/log/formatter/text/basic/BasicTextFormatter.cpp"
	"src/framework/log/policy/Policy.h"
	"src/framework/log/policy/trace/StacktracePolicy.h"
	"src/framework/log/policy/trace/StacktracePolicy.cpp"
	"src/framework/log/policy/trace/SourceLocationPolicy.h"
	"src/framework/log/policy/trace/SourceLocationPolicy.cpp"
)

# Include different source code depending on OS
if(WIN32)
	target_sources(NetBase PRIVATE
		"src/sys/win32/WinDynamicLibrary.h"
		"src/sys/win32/WinDynamicLibrary.cpp"
	)
elseif(LINUX OR APPLE)
	target_sources(NetBase PRIVATE
		"src/sys/posix/PosixDynamicLibrary.h"
		"src/sys/posix/PosixDynamicLibrary.cpp"
	)
endif()

#if(LINUX)
#	target_sources(NetBase PRIVATE
#	)
#elseif(APPLE)
#	target_sources(NetBase PRIVATE
#	)
#endif()

target_include_directories(NetBase PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Add the ASIO library as an interface target
add_library(asio INTERFACE)

target_include_directories(asio SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/asio/include")

target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

if(WIN32)
	target_compile_definitions(asio INTERFACE ASIO_HAS_IOCP)
elseif(UNIX)
	target_compile_definitions(asio INTERFACE ASIO_HAS_IO_URING)
endif()

target_link_libraries(NetBase PRIVATE asio)

set_target_properties(NetBase PROPERTIES COMPILE_WARNING_AS_ERROR ON)

# Set up all the compiler options here
# MSVC compiler options
if(MSVC)
	target_compile_options(NetBase PRIVATE
		"/external:W0"
		"/external:anglebrackets"
		"/analyze:external-"
	)

	# Common options for all build types
	target_compile_options(NetBase PRIVATE
		"/W4"
		"/WX"
		"/permissive-"
		"/analyze"
		"/fp:fast"
		"/FAs"
		"/MP"
		"/Zc:__cplusplus"
	)

	# Debug build compiler options
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(NetBase PRIVATE
			"/Zi"
			"/fsanitize=address"
			"/Od"
			"/MDd"
			"/JMC"
		)

		# Ensure PDB file is generated (Win32)
		target_link_options(NetBase PRIVATE "/DEBUG")

	# Release build compiler options
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_compile_options(NetBase PRIVATE
			"/O2"
			"/MD"
			"/GL"
			"/LTCG"
			"/Gw"
			"/Z7"
		)
	endif()

# CLANG and GCC compiler options
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# Common options for all build types
	target_compile_options(NetBase PRIVATE
		"-Wall"
		"-Werror"
		"-Wpedantic"
		"-ffast-math"
		"-fanalyzer"
		"-fvisibility=hidden"
		"-g"
	)

	# Debug build compiler options
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(NetBase PRIVATE
			"-fsanitize=address"
			"-fsanitize=leak"
			"-fsanitize=undefined"
			"--fsanitize=thread"
			"-O0"
		)

		target_link_options(NetBase PRIVATE
			"-fsanitize=address"
			"-fsanitize=leak"
			"-fsanitize=undefined"
			"--fsanitize=thread"
		)

	# Release build compiler options
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_compile_options(NetBase PRIVATE
			"-O3"
			"-flto"
		)
	endif()
endif()

# TODO: Add tests and install targets if needed.
