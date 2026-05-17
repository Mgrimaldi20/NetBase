# CMakeList.txt : CMake project for SandboxProtocol, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.25)

# Add source to this project's executable.
add_library(SandboxProtocol SHARED)

project(SandboxProtocol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set_property(TARGET SandboxProtocol PROPERTY CXX_STANDARD 23)

target_compile_definitions(SandboxProtocol PRIVATE SANDBOXPROTOCOL_SPEC_VERSION=1)

# Specify the common OS independent source files for the SandboxProtocol project.
target_sources(SandboxProtocol PRIVATE
	"src/SandboxProtocol.cpp"
	"src/ClientAPIImpl.h"
	"src/ClientAPIImpl.cpp"
	"src/ParserImpl.h"
	"src/ParserImpl.cpp"
)

include_directories(src)

# Set up linker options
if(WIN32)
	target_compile_definitions(SandboxProtocol PRIVATE UNICODE _UNICODE)
endif()

# Set up all the compiler options here
# MSVC compiler options
if(MSVC)
	target_compile_options(SandboxProtocol PRIVATE
		"/external:W0"
		"/external:anglebrackets"
		"/analyze:external-"
	)

	# Common options for all build types
	target_compile_options(SandboxProtocol PRIVATE
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
		target_compile_options(SandboxProtocol PRIVATE
			"/Zi"
			"/fsanitize=address"
			"/Od"
			"/MDd"
			"/JMC"
		)

		# Ensure PDB file is generated (Win32)
		target_link_options(SandboxProtocol PRIVATE "/DEBUG")

	# Release build compiler options
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_compile_options(SandboxProtocol PRIVATE
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
	target_compile_options(SandboxProtocol PRIVATE
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
		target_compile_options(SandboxProtocol PRIVATE
			"-fsanitize=address"
			"-fsanitize=leak"
			"-fsanitize=undefined"
			"--fsanitize=thread"
			"-O0"
		)

		target_link_options(SandboxProtocol PRIVATE
			"-fsanitize=address"
			"-fsanitize=leak"
			"-fsanitize=undefined"
			"--fsanitize=thread"
		)

	# Release build compiler options
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_compile_options(SandboxProtocol PRIVATE
			"-O3"
			"-flto"
		)
	endif()
endif()

# TODO: Add tests and install targets if needed.
