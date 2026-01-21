using CxxWrap

# Get the path to the CxxWrap prefix (where JlCxx is installed)
prefix = CxxWrap.prefix_path()

# Get Julia executable and library paths
julia_executable = Sys.BINDIR |> dirname |> x -> joinpath(x, "bin", "julia")
julia_home = dirname(Sys.BINDIR)

@info "Julia executable: $julia_executable"
@info "Julia home: $julia_home"

# Copy the fixed FindJulia.cmake to deps directory to override the buggy JlCxx version
src_dir = @__DIR__
fixed_find_julia = joinpath(dirname(src_dir), "FindJulia_Fixed.cmake")
deps_find_julia = joinpath(src_dir, "FindJulia.cmake")

if isfile(fixed_find_julia)
    @info "Using fixed FindJulia.cmake from project root"
    cp(fixed_find_julia, deps_find_julia; force=true)
else
    @warn "Fixed FindJulia.cmake not found at $fixed_find_julia"
end

# Build using CMake
build_dir = joinpath(src_dir, "build")

# Create build directory if it doesn't exist
mkpath(build_dir)

# Configure with CMake
# Use the fixed FindJulia.cmake and pass Julia paths explicitly
cmake_args = [
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_PREFIX_PATH=$prefix",
    "-DCMAKE_MODULE_PATH=$src_dir",  # Use our fixed FindJulia.cmake
    "-DCMAKE_INSTALL_PREFIX=$src_dir",
    "-DJulia_PREFIX=$julia_home",
    "-DJulia_EXECUTABLE=$julia_executable",
    "-S", "$src_dir",
    "-B", "$build_dir"
]

@info "Using CxxWrap prefix: $prefix"
@info "Using fixed FindJulia.cmake from: $deps_find_julia"
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

