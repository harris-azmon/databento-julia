using CxxWrap
using Libdl

# Get the path to the CxxWrap prefix (where JlCxx is installed)
prefix = CxxWrap.prefix_path()

# Build using CMake
src_dir = @__DIR__
build_dir = joinpath(src_dir, "build")

# Unset conflicting environment variables for the duration of this script
delete!(ENV, "JULIA_BINDIR")
delete!(ENV, "JULIA_DEPOT_PATH")

# Create build directory if it doesn't exist
mkpath(build_dir)

# Define Julia paths dynamically
julia_executable = joinpath(Sys.BINDIR, "julia")
julia_root = dirname(Sys.BINDIR)
julia_include_dirs = joinpath(julia_root, "include", "julia")
julia_library = ""
if Sys.islinux()
    julia_library = joinpath(julia_root, "lib", "libjulia.so")
elseif Sys.isapple()
    julia_library = joinpath(julia_root, "lib", "libjulia.dylib")
elseif Sys.iswindows()
    julia_library = joinpath(julia_root, "bin", "libjulia.dll")
end

@info "Julia paths:" julia_executable julia_root julia_include_dirs julia_library

# Configure with CMake, passing Julia paths explicitly.
run(`cmake -DCMAKE_BUILD_TYPE=Release
           -DCMAKE_PREFIX_PATH="$prefix;$julia_root"
           -DJULIA_EXECUTABLE="$julia_executable"
           -DJULIA_LIBRARY="$julia_library"
           -DJULIA_ROOT="$julia_root"
           -DJULIA_INCLUDE_DIRS="$julia_include_dirs"
           -DCMAKE_INSTALL_PREFIX="$src_dir"
           -S "$src_dir"
           -B "$build_dir"`)

# Build the library
run(`cmake --build $build_dir --config Release`)

# Install the library to deps/
run(`cmake --install $build_dir --prefix $src_dir`)

# Create deps.jl file that tells Julia where to find the library
open(joinpath(@__DIR__, "deps.jl"), "w") do io
    # Try common library extensions
    lib_candidates = [
        joinpath(@__DIR__, "lib", "libdatabento_jl.so"),      # Linux
        joinpath(@__DIR__, "lib", "libdatabento_jl.dylib"),   # macOS
        joinpath(@__DIR__, "bin", "libdatabento_jl.dll"),     # Windows
        joinpath(@__DIR__, "libdatabento_jl.so"),             # Alternative Linux
        joinpath(@__DIR__, "libdatabento_jl.dylib"),          # Alternative macOS
    ]

    lib_path = nothing
    for candidate in lib_candidates
        if isfile(candidate)
            lib_path = candidate
            break
        end
    end

    if lib_path === nothing
        error("Could not find compiled library. Checked: ", join(lib_candidates, ", "))
    end

    println(io, "const libdatabento_jl = \"$lib_path\"")
end

println("Build completed successfully!")