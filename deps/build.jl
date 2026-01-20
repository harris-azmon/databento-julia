using CxxWrap

# Get the path to the CxxWrap prefix (where JlCxx is installed)
prefix = CxxWrap.prefix_path()

# Find and patch the problematic FindJulia.cmake if it exists
function patch_find_julia()
    find_julia_paths = [
        joinpath(prefix, "lib", "cmake", "JlCxx", "FindJulia.cmake"),
    ]

    for fj_path in find_julia_paths
        if isfile(fj_path)
            @info "Patching FindJulia.cmake at $fj_path"
            content = read(fj_path, String)

            # Comment out the problematic line 115 that fails when Julia_LIBRARY is empty
            # Replace: get_filename_component(Julia_LIBRARY_DIR ${Julia_LIBRARY} DIRECTORY)
            # With safe version that checks if Julia_LIBRARY is set
            content = replace(content,
                "get_filename_component(Julia_LIBRARY_DIR \${Julia_LIBRARY} DIRECTORY)" =>
                "if(DEFINED Julia_LIBRARY)\n    get_filename_component(Julia_LIBRARY_DIR \${Julia_LIBRARY} DIRECTORY)\nendif()"
            )

            write(fj_path, content)
            @info "Successfully patched FindJulia.cmake"
            return
        end
    end

    @warn "FindJulia.cmake not found in expected locations"
end

patch_find_julia()

# Build using CMake
src_dir = @__DIR__
build_dir = joinpath(src_dir, "build")

# Create build directory if it doesn't exist
mkpath(build_dir)

# Configure with CMake
# Use CxxWrap's prefix path to find JlCxx via CONFIG mode
cmake_args = [
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_PREFIX_PATH=$prefix",
    "-DCMAKE_INSTALL_PREFIX=$src_dir",
    "-S", "$src_dir",
    "-B", "$build_dir"
]

@info "Using CxxWrap prefix: $prefix"
@info "Executing: cmake $(join(cmake_args, " "))"

# Build cmake command using Cmd constructor to ensure proper argument passing
cmd = Cmd(vcat(["cmake"], cmake_args))
run(cmd)

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

