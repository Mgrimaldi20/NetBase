# CMakeList.txt : CMake project for NetBase, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.25)
project(NetBase LANGUAGES CXX)

# Add source to this project's executable
add_executable(NetBase)

set(CMAKE_CXX_STANDARD 23)
set_property(TARGET NetBase PROPERTY CXX_STANDARD 23)

# Set the NetBase version
target_compile_definitions(NetBase PRIVATE NETBASE_VERSION=1)

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

# Specify the common OS independent source files for the NetBase project
target_sources(NetBase PRIVATE
	"src/Main.cpp"
	"src/Asio.h"
	"src/Asio.cpp"
	"src/framework/Log.h"
	"src/framework/Log.cpp"
	"src/framework/Client.h"
	"src/framework/Session.h"
	"src/framework/Session.cpp"
	"src/framework/Server.h"
	"src/framework/Server.cpp"
	"src/framework/Channel.h"
	"src/framework/Channel.cpp"
	"src/framework/ChannelManager.h"
	"src/framework/ChannelManager.cpp"
)

include_directories("/src/framework")

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
	)

	# Debug build compiler options
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(NetBase PRIVATE
			"-g"
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
