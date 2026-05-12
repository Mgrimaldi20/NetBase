# CMakeList.txt : CMake project for SandboxProtocol, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library(SandboxProtocol SHARED)

if(CMAKE_VERSION VERSION_GREATER 3.25)
	set_property(TARGET SandboxProtocol PROPERTY CXX_STANDARD 23)
endif()

target_compile_definitions(SandboxProtocol PRIVATE SandboxProtocol_SPEC_VERSION=1)

# Specify the common OS independent source files for the SandboxProtocol project.
target_sources(SandboxProtocol PRIVATE
	"src/Cmd.h"
	"src/Cmd.cpp"
)

include_directories(src)

# Set up linker options
if(WIN32)
	target_compile_definitions(SandboxProtocol PRIVATE UNICODE _UNICODE)
endif()

# Set up all the compiler options here
# MSVC compiler options
if(MSVC)
	# Common options for all build types
	target_compile_options(SandboxProtocol PRIVATE
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
	)

	# Debug build compiler options
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(SandboxProtocol PRIVATE
			"-g"
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
		target_compile_options(NetBase PRIVATE
			"-O3"
			"-flto"
		)
	endif()
endif()

# TODO: Add tests and install targets if needed.
