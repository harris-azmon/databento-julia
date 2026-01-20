# Fixed FindJulia.cmake that works correctly with CMake 3.15+
# This replaces the buggy JlCxx FindJulia.cmake

if(Julia_FOUND)
    return()
endif()

# Julia Executable
if(NOT Julia_EXECUTABLE)
    find_program(Julia_EXECUTABLE julia DOC "Julia executable")
endif()

if(Julia_EXECUTABLE)
    message(STATUS "Found Julia executable: ${Julia_EXECUTABLE}")

    # Julia Version
    execute_process(
        COMMAND "${Julia_EXECUTABLE}" --startup-file=no --version
        OUTPUT_VARIABLE Julia_VERSION_STRING
    )
    string(REGEX REPLACE ".*([0-9]+\\.[0-9]+\\.[0-9]+).*" "\\1" Julia_VERSION_STRING "${Julia_VERSION_STRING}")
else()
    message(FATAL_ERROR "Julia executable not found")
endif()

message(STATUS "Julia_VERSION_STRING: ${Julia_VERSION_STRING}")

# Julia Includes
if(NOT Julia_INCLUDE_DIRS)
    set(USING_LIBDL "using Libdl")
    execute_process(
        COMMAND ${Julia_EXECUTABLE} --startup-file=no -E "joinpath(dirname(Sys.BINDIR), \"include\", \"julia\")"
        OUTPUT_VARIABLE Julia_INCLUDE_DIRS
    )
    string(REGEX REPLACE "\"" "" Julia_INCLUDE_DIRS "${Julia_INCLUDE_DIRS}")
    string(REGEX REPLACE "\n" "" Julia_INCLUDE_DIRS "${Julia_INCLUDE_DIRS}")
endif()

message(STATUS "Julia_INCLUDE_DIRS: ${Julia_INCLUDE_DIRS}")

# Julia Libraries
if(NOT Julia_LIBRARY)
    execute_process(
        COMMAND ${Julia_EXECUTABLE} --startup-file=no -E "using Libdl; abspath(Libdl.dlpath((ccall(:jl_is_debugbuild, Cint, ()) != 0) ? \"libjulia-debug\" : \"libjulia\"))"
        OUTPUT_VARIABLE Julia_LIBRARY
    )
    string(REGEX REPLACE "\"" "" Julia_LIBRARY "${Julia_LIBRARY}")
    string(REGEX REPLACE "\n" "" Julia_LIBRARY "${Julia_LIBRARY}")
    string(STRIP "${Julia_LIBRARY}" Julia_LIBRARY)
endif()

if(Julia_LIBRARY)
    message(STATUS "Julia_LIBRARY: ${Julia_LIBRARY}")
    # Get directory from library path - use string operations to be safe
    string(REGEX REPLACE "^(.+)/[^/]+\\.so.*$" "\\1" Julia_LIBRARY_DIR "${Julia_LIBRARY}")
    if(NOT Julia_LIBRARY_DIR STREQUAL "")
        message(STATUS "Julia_LIBRARY_DIR: ${Julia_LIBRARY_DIR}")
    endif()
else()
    message(FATAL_ERROR "Julia library not found")
endif()

# Julia Headers
find_path(Julia_MAIN_HEADER julia.h HINTS ${Julia_INCLUDE_DIRS})

# Julia Word Size
execute_process(
    COMMAND ${Julia_EXECUTABLE} --startup-file=no -E "Sys.WORD_SIZE"
    OUTPUT_VARIABLE Julia_WORD_SIZE
)
string(REGEX REPLACE "\n" "" Julia_WORD_SIZE "${Julia_WORD_SIZE}")
message(STATUS "Julia_WORD_SIZE: ${Julia_WORD_SIZE}")

# Standard find package stuff
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Julia
    REQUIRED_VARS   Julia_LIBRARY Julia_INCLUDE_DIRS Julia_MAIN_HEADER
    VERSION_VAR     Julia_VERSION_STRING
    FAIL_MESSAGE    "Julia not found"
)
