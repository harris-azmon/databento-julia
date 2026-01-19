using CxxWrap

# Get the path to the CxxWrap prefix (where JlCxx is installed)
prefix = CxxWrap.prefix_path()

# Build using CMake
src_dir = @__DIR__
build_dir = joinpath(src_dir, "build")

# Create build directory if it doesn't exist
mkpath(build_dir)

# Configure with CMake
run(`cmake -DCMAKE_BUILD_TYPE=Release
           -DCMAKE_PREFIX_PATH=$prefix
           -DCMAKE_INSTALL_PREFIX=$src_dir
           -S $src_dir
           -B $build_dir`)

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
