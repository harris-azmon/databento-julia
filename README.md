# Databento.jl

High-performance Julia bindings for the [databento-cpp](https://github.com/databento/databento-cpp) library using [CxxWrap.jl](https://github.com/JuliaInterop/CxxWrap.jl).

## Features

- **Core Enums** ✅: Full mapping of `Schema`, `Encoding`, `SType`, `Dataset`, `Publisher`, `RType`, `Action`, and `Side`.
- **Data Records** ✅: Support for `MboMsg`, `TradeMsg`, `Mbp1Msg`, `Mbp10Msg`, `InstrumentDefMsg`, and `ImbalanceMsg`.
- **Historical Client** ✅: Full metadata queries, symbology resolution, and historical data downloads.
- **DBN Reader** ✅: High-performance DBN file reading with metadata support.
- **Zero-Copy Design**: Direct access to C++ market data records from Julia.

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

### Phase 2: Data Record Protocol ✅
- [x] Fixed-width string handling (via C++ mapping)
- [x] RecordHeader mapping
- [x] MboMsg (Market by Order)
- [x] TradeMsg (Trades)
- [x] Mbp1Msg and Mbp10Msg
- [x] InstrumentDefMsg (Reference Data)
- [x] ImbalanceMsg
- [x] ToString() methods for structs

### Phase 3: Historical Client ✅
- [x] Historical client wrapper
- [x] Builder pattern (SetKeyFromEnv, SetKey)
- [x] Metadata queries (ListDatasets, ListSchemas, ListFields)
- [x] Exception handling

### Phase 4: Fetching Data & DBN Reader ✅
- [x] DBN File Reader (DbnFileStore)
- [x] Record access and header parsing
- [x] Metadata retrieval from DBN files
- [x] Timeseries range queries to file

### Phase 5: Reference Data & Symbology ✅
- [x] Symbology resolution (symbology_resolve)
- [x] Instrument definitions mapping
- [x] Field detail metadata

### Phase 6: Tables.jl Integration ✅
- [x] Tables.rows and Tables.columns interfaces
- [x] Zero-copy DataFrame construction
- [x] Base.iterate for Record streams

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
