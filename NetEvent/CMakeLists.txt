# CMakeList.txt : CMake project for NetEvent, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library(NetEvent SHARED)

if(CMAKE_VERSION VERSION_GREATER 3.25)
	set_property(TARGET NetEvent PROPERTY CXX_STANDARD 23)
endif()

target_compile_definitions(NetEvent PRIVATE NETEVENT_SPEC_VERSION=1)

# Specify the common OS independent source files for the NetEvent project.
target_sources(NetEvent PRIVATE
	"src/Cmd.h"
	"src/Cmd.cpp"
)

include_directories(src)

# Set up linker options
if(WIN32)
	target_compile_definitions(NetEvent PRIVATE UNICODE _UNICODE)
endif()

# Set up all the compiler options here
if(MSVC)
	target_compile_options(NetEvent PRIVATE "/W4" "/WX" "/permissive-" "/analyze" "/fp:fast" "/FAs" "/MP")		# Common options for all build types
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")																		# Debug build compiler options
		target_compile_options(NetEvent PRIVATE
			"/Zi"
			"/fsanitize=address"
			"/Od"
			"/MDd"
			"/JMC"
		)
		target_link_options(NetEvent PRIVATE "/DEBUG")															# Ensure PDB file is generated
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")																	# Release build compiler options
		target_compile_options(NetEvent PRIVATE "/O2" "/MD" "/GL" "/Gw")
	endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")							# CLANG and GCC compiler options
	target_compile_options(NetEvent PRIVATE "-Wall" "-Werror" "-Wpedantic" "-ffast-math" "-fanalyzer")			# Common options for all build types
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")																		# Debug build compiler options
		target_compile_options(NetEvent PRIVATE
			"-g"
			#"-fsanitize=leak"
			#"-fsanitize=undefined"
			#"-fsanitize=address"
			#"-fsanitize=thread"
			"-O0"
		)
		target_link_libraries(NetEvent PRIVATE
			#"-fsanitize=leak"
			#"-fsanitize=undefined"
			#"-fsanitize=address"
			#"-fsanitize=thread"
		)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")																	# Release build compiler options
		target_compile_options(NetEvent PRIVATE "-O3" "-flto")
	endif()
endif()

# TODO: Add tests and install targets if needed.
