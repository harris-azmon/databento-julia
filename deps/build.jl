using CxxWrap

# Get the path to the CxxWrap prefix (where JlCxx is installed)
prefix = CxxWrap.prefix_path()

# Find Julia library and executable by deriving from Julia's installation directory
# This avoids issues with Libdl not being available in build sandbox
global julia_libpath = ""
global julia_libdir = ""
global julia_executable = ""

try
    # Derive Julia library path from Sys.BINDIR
    # Julia is typically installed as: /path/to/julia/bin/julia
    # And libraries are at: /path/to/julia/lib/
    julia_bindir = dirname(Sys.BINDIR)  # Parent of bin directory
    julia_libdir_candidate = joinpath(julia_bindir, "lib")

    # Find Julia executable
    julia_bin_candidate = joinpath(dirname(Sys.BINDIR), "bin", "julia")
    if isfile(julia_bin_candidate)
        global julia_executable = julia_bin_candidate
        @info "Found Julia executable: $julia_executable"
    end

    # Try common library filenames in order of preference
    lib_candidates = [
        joinpath(julia_libdir_candidate, "libjulia.so.1"),   # Linux versioned
        joinpath(julia_libdir_candidate, "libjulia.so"),     # Linux unversioned
        joinpath(julia_libdir_candidate, "libjulia.dylib"),  # macOS
        joinpath(julia_libdir_candidate, "libjulia.dll"),    # Windows
    ]

    for candidate in lib_candidates
        if isfile(candidate)
            global julia_libpath = candidate
            global julia_libdir = dirname(julia_libpath)
            @info "Found Julia library: $julia_libpath"
            break
        end
    end

    if isempty(julia_libpath)
        @warn "Julia library not found in candidates: $lib_candidates"
    end

catch e
    @warn "Error finding Julia: $e"
    global julia_libpath = ""
    global julia_libdir = ""
    global julia_executable = ""
end

# Build using CMake
src_dir = @__DIR__
build_dir = joinpath(src_dir, "build")

# Create build directory if it doesn't exist
mkpath(build_dir)

# Configure with CMake
# Pass Julia library info explicitly to bypass FindJulia.cmake issues
cmake_args = [
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_PREFIX_PATH=$prefix",
    "-DCMAKE_INSTALL_PREFIX=$src_dir",
    "-S", "$src_dir",
    "-B", "$build_dir"
]

# Add Julia paths if found
if !isempty(julia_executable)
    push!(cmake_args, "-DJulia_EXECUTABLE=$julia_executable")
    @info "Passing Julia executable to CMake: $julia_executable"
end

if !isempty(julia_libpath)
    push!(cmake_args, "-DJulia_LIBRARY=$julia_libpath")
    push!(cmake_args, "-DJulia_LIBRARY_DIR=$julia_libdir")
    @info "Passing Julia library to CMake: $julia_libpath"
else
    @warn "Julia library path is empty - CMake may fail to find Julia"
end

# Debug: Print the full cmake command
@info "Executing: cmake $(join(cmake_args, " "))"

# Build cmake command using Cmd constructor to ensure proper argument passing
cmd = Cmd(vcat(["cmake"], cmake_args))
@info "Cmd object: $cmd"
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

