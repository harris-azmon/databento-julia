# Databento.jl

High-performance Julia bindings for the [databento-cpp](https://github.com/databento/databento-cpp) library using [CxxWrap.jl](https://github.com/JuliaInterop/CxxWrap.jl).

## Features

**Phase 1: Core Enums** ✅
- Full mapping of `Schema`, `Encoding`, `SType`, and `Dataset` enums
- Zero-copy enum representation using `add_bits`
- String conversion and pretty-printing support

## Installation

### Prerequisites

- Julia 1.9 or later
- CMake 3.24 or later
- C++17 compatible compiler
- OpenSSL 3.0+
- Libcrypto
- Zstandard (zstd)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/databento-julia.git
cd databento-julia

# Start Julia and install dependencies
julia --project=. -e 'using Pkg; Pkg.instantiate()'

# Build the C++ extension
julia --project=. -e 'using Pkg; Pkg.build("Databento")'

# Run tests
julia --project=. -e 'using Pkg; Pkg.test("Databento")'
```

## Quick Start

```julia
using Databento

# Use Schema enums
schema = TRADES
println(schema)  # Schema::trades

# Use Dataset enums
dataset = GLBX_MDP3
println(dataset)  # Dataset::GLBX.MDP3

# Use SType (symbology type) enums
stype = RAW_SYMBOL
println(stype)  # SType::raw_symbol

# Use Encoding enums
encoding = DBN
println(encoding)  # Encoding::dbn
```

## Development Roadmap

### Phase 1: Environment & Core Enums ✅
- [x] CMake setup with FetchContent for databento-cpp
- [x] Module entry point (databento_jl.cpp)
- [x] Schema enum mapping (Mbo, Mbp1, Trades, etc.)
- [x] Encoding enum mapping (Dbn, Csv, Json)
- [x] SType enum mapping (RawSymbol, InstrumentId, Parent)
- [x] Dataset enum mapping (GlbxMdp3, XnasItch, etc.)
- [x] Unit tests for all enums

### Phase 2: Data Record Protocol (In Progress)
- [ ] Fixed-width string handling
- [ ] RecordHeader mapping
- [ ] MboMsg (Market by Order)
- [ ] TradeMsg (Trades)
- [ ] InstrumentDefMsg (Reference Data)
- [ ] ImbalanceMsg
- [ ] ToString() methods for structs

### Phase 3: Historical Client
- [ ] Historical client wrapper
- [ ] Builder pattern (SetKeyFromEnv, SetDataset, SetOutputFormat)
- [ ] Exception handling

### Phase 4: Fetching Data
- [ ] DBN Reader
- [ ] Record Iterator
- [ ] Julia Iterator interface (Base.iterate)

### Phase 5: Reference Data & Symbology
- [ ] Symbology resolution
- [ ] GetFields() for metadata

### Phase 6: Tables.jl Integration
- [ ] Tables.rows and Tables.columns interfaces
- [ ] Zero-copy DataFrame construction

## Architecture

### Zero-Copy Design

The library uses `CxxWrap.jl`'s `add_bits` feature to map C++ POD (Plain Old Data) structs directly to Julia, enabling zero-copy data access:

```cpp
// In C++: Mark type as bits-compatible
template<> struct IsBits<databento::Schema> : std::true_type {};

// Register with CxxWrap
mod.add_bits<databento::Schema>("Schema", jlcxx::julia_type("CppEnum"));
```

This allows Julia to reinterpret C++ memory directly without copying, crucial for high-frequency market data.

### File Structure

```
databento-julia/
├── deps/
│   ├── CMakeLists.txt       # CMake configuration
│   ├── databento_jl.cpp     # C++ wrapper implementation
│   └── build.jl             # Julia build script
├── src/
│   └── Databento.jl         # Main Julia module
├── test/
│   └── runtests.jl          # Test suite
├── Project.toml             # Julia package manifest
└── README.md
```

## Testing Strategy (TDD)

This project follows Test-Driven Development:

1. **Write tests first** - Define expected behavior
2. **Implement feature** - Map C++ types/functions
3. **Build often** - Verify compilation after each change
4. **Run tests** - Ensure behavior matches expectations

Example test structure:

```julia
@testset "Schema Enum" begin
    @test string(MBO) == "mbo"
    @test string(TRADES) == "trades"
end
```

## Performance Considerations

- **Zero-copy access**: Direct memory mapping for records
- **Bits types**: Enums and structs use bits representation
- **No intermediate allocations**: Data flows directly from C++ to Julia
- **Column-oriented access**: Compatible with DataFrame zero-copy views

## Contributing

Contributions are welcome! Please:

1. Follow the TDD approach
2. Build and test before submitting
3. Update documentation for new features
4. Use the established phase structure

## License

See [LICENSE](LICENSE) file.

## Resources

- [Databento API Documentation](https://databento.com/docs)
- [databento-cpp GitHub](https://github.com/databento/databento-cpp)
- [CxxWrap.jl Documentation](https://juliainterop.github.io/CxxWrap.jl/)
