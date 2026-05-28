# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required(VERSION 3.25)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	#set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("NetBase")

# Include sub-projects.
add_subdirectory("NetBase")

add_subdirectory("SandboxProtocol")

# Create a main output directory under the build directory where all exe and dll files will be placed
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Binary output directory set: ${CMAKE_BINARY_DIR}")
set_target_properties(NetBase PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(SandboxProtocol PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(SandboxProtocol PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

############################################################################################################
############################################## Install Targets #############################################
############################################################################################################
# Install the executable and DLLs to the bin directory
if(WIN32)
	install(TARGETS NetBase
		RUNTIME DESTINATION NetBase/bin
	)
endif()

# Install the shared library to the bin directory, Linux doesnt do this by default
if(UNIX)
	install(TARGETS NetBase
		LIBRARY DESTINATION NetBase/bin
		RUNTIME DESTINATION NetBase/bin
	)
endif()

# Install header files to the sdk directory, preserving the directory structure
# There is prolly a wayyyy better way to do this, but this is working for now
install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/NetBaseAPI.h"
	DESTINATION NetBase/sdk
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/ChannelManager.h"
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/CmdDispatcher.h"
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/Client.h"
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/Channel.h"
	DESTINATION NetBase/sdk/framework
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/Log.h"
	DESTINATION NetBase/sdk/framework/log
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/driver/Driver.h"
	DESTINATION NetBase/sdk/framework/log/driver
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/entry/Entry.h"
	DESTINATION NetBase/sdk/framework/log/entry
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/formatter/text/TextFormatter.h"
	DESTINATION NetBase/sdk/framework/log/formatter/text
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/formatter/text/basic/BasicTextFormatter.h"
	DESTINATION NetBase/sdk/framework/log/formatter/text/basic
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/policy/Policy.h"
	DESTINATION NetBase/sdk/framework/log/policy
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/policy/trace/StacktracePolicy.h"
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/policy/trace/SourceLocationPolicy.h"
	DESTINATION NetBase/sdk/framework/log/policy/trace
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/sink/Sink.h"
	DESTINATION NetBase/sdk/framework/log/sink
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/sink/text/TextSink.h"
	DESTINATION NetBase/sdk/framework/log/sink/text
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/sink/text/console/ConsoleSink.h"
	DESTINATION NetBase/sdk/framework/log/sink/text/console
)

install(FILES
	"${CMAKE_SOURCE_DIR}/NetBase/src/framework/log/sink/text/file/FileSink.h"
	DESTINATION NetBase/sdk/framework/log/sink/text/file
)
