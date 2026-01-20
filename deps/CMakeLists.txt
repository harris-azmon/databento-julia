cmake_minimum_required(VERSION 3.15)

project(databento_jl CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find JlCxx - use the artifact path provided by CMake
# JlCxx provides everything we need
find_package(JlCxx QUIET CONFIG)

if(NOT JlCxx_FOUND)
    message(FATAL_ERROR "JlCxx not found. Make sure CxxWrap.jl is properly installed.")
endif()

# Fetch databento-cpp library
include(FetchContent)
FetchContent_Declare(
  databento
  GIT_REPOSITORY https://github.com/databento/databento-cpp
  GIT_TAG v0.30.0
)
FetchContent_MakeAvailable(databento)

# Create the Julia extension library
add_library(databento_jl SHARED databento_jl.cpp)

# Link against JlCxx and databento
target_link_libraries(databento_jl
  PRIVATE
    JlCxx::cxxwrap_julia
    databento::databento
)

# Get Julia information from JlCxx
message(STATUS "JlCxx found at: ${JlCxx_PREFIX}")
message(STATUS "Julia include dirs: ${Julia_INCLUDE_DIRS}")
message(STATUS "Julia library dir: ${Julia_LIBRARY_DIR}")

# Install the library
install(TARGETS databento_jl
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
