#!/usr/bin/env julia
"""
find_julia_lib.jl - Find Julia library path using Libdl

This script uses Julia's Libdl module to locate the Julia library,
avoiding the CMake FindJulia.cmake issues with Libdl in build sandboxes.

Usage:
  julia find_julia_lib.jl                 # Prints Julia library path
  julia find_julia_lib.jl --dir           # Prints Julia library directory
  julia find_julia_lib.jl --cmake         # Prints CMake format for variable
"""

using Libdl

try
    # Get the Julia library path using Libdl (this works even in build context)
    libpath = Libdl.dlpath("libjulia")
    libdir = dirname(libpath)

    if length(ARGS) > 0
        arg = ARGS[1]
        if arg == "--dir"
            # Print just the directory
            println(libdir)
        elseif arg == "--cmake"
            # Print in CMake format
            println("-DJulia_LIBRARY=$(libpath)")
            println("-DJulia_LIBRARY_DIR=$(libdir)")
        else
            println(libpath)
        end
    else
        # Default: print full path
        println(libpath)
    end
catch e
    @error "Failed to find Julia library: $e"
    exit(1)
end
