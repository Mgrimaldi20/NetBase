# CMakeList.txt : CMake project for NetBase, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable(NetBase)

if(CMAKE_VERSION VERSION_GREATER 3.25)
	set_property(TARGET NetBase PROPERTY CXX_STANDARD 23)
endif()

target_compile_definitions(NetBase PRIVATE NETBASE_VERSION=1)

# Check the Operating System and include the appropriate file for execution
if(WIN32)
	target_sources(NetBase PRIVATE
		"src/sys/win32/Main.cpp"
		"src/sys/win32/net/WinSockAPI.h"
		"src/sys/win32/net/WinSockAPI.cpp"
		"src/sys/win32/io/IOContext.h"
		"src/sys/win32/io/IOContext.cpp"
		"src/sys/win32/net/Socket.h"
		"src/sys/win32/net/Socket.cpp"
		"src/sys/win32/io/OverlappedIO.h"
		"src/sys/win32/io/OverlappedIO.cpp"
		"src/sys/win32/io/IOCompletionPort.h"
		"src/sys/win32/io/IOCompletionPort.cpp"
	)
elseif(LINUX OR APPLE)
	target_sources(NetBase PRIVATE
		"src/sys/posix/Main.cpp"
	)
endif()

# Specify the common OS independent source files for the NetBase project.
target_sources(NetBase PRIVATE
	"src/sys/Socket.h"
	"src/sys/IOContext.h"
	"src/framework/Bitmask.h"
	"src/framework/ByteBuffer.h"
	"src/framework/ByteBuffer.cpp"
	"src/framework/Log.h"
	"src/framework/Log.cpp"
	"src/framework/UUID.h"
	"src/framework/UUID.cpp"
	"src/framework/IOContextManager.h"
	"src/framework/IOContextManager.cpp"
	"src/framework/CmdSystem.h"
	"src/framework/CmdSystem.cpp"
)

include_directories(src)

# Set up linker options
if(WIN32)
	target_compile_definitions(NetBase PRIVATE UNICODE _UNICODE)
	target_link_libraries(NetBase PUBLIC ws2_32)
	target_link_libraries(NetBase PUBLIC mswsock)
endif()

# Set up all the compiler options here
if(MSVC)																										# MSVC compiler options
	target_compile_options(NetBase PRIVATE																		# Common options for all build types
		"/W4"
		"/WX"
		"/permissive-"
		"/analyze"
		"/fp:fast"
		"/FAs"
		"/MP"
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")																		# Debug build compiler options
		target_compile_options(NetBase PRIVATE
			"/Zi"
			"/fsanitize=address"
			"/Od"
			"/MDd"
			"/JMC"
		)
		target_link_options(NetBase PRIVATE "/DEBUG")															# Ensure PDB file is generated (Win32)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")																	# Release build compiler options
		target_compile_options(NetBase PRIVATE
			"/O2"
			"/MD"
			"/GL"
			"/Gw"
		)
	endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")							# CLANG and GCC compiler options
	target_compile_options(NetBase PRIVATE																		# Common options for all build types
		"-Wall"
		"-Werror"
		"-Wpedantic"
		"-ffast-math"
		"-fanalyzer"
		"-fvisibility=hidden"
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")																		# Debug build compiler options
		target_compile_options(NetBase PRIVATE
			"-g"
			#"-fsanitize=address"
			#"-fsanitize=leak"
			#"-fsanitize=undefined"
			#"-fsanitize=thread"
			"-O0"
		)
		target_link_libraries(NetBase PRIVATE
			#"-fsanitize=address"
			#"-fsanitize=leak"
			#"-fsanitize=undefined"
			#"-fsanitize=thread"
		)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")																	# Release build compiler options
		target_compile_options(NetBase PRIVATE
			"-O3"
			"-flto"
		)
	endif()
endif()

# TODO: Add tests and install targets if needed.
