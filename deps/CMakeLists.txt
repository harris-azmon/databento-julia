cmake_minimum_required(VERSION 3.21)

project(databento_jl CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find JlCxx for C++ wrapper support. This is provided by CxxWrap.jl
# and its location is passed via CMAKE_PREFIX_PATH in build.jl.
find_package(JlCxx REQUIRED)

# Include Julia directories
include_directories(${JULIA_INCLUDE_DIRS})

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

# Link against Julia, JlCxx, and databento
target_link_libraries(databento_jl
  PRIVATE
    ${JULIA_LIBRARY}
    JlCxx::cxxwrap_julia
    databento::databento
)

# Install the library
install(TARGETS databento_jl
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
