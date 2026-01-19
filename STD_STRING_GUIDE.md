# Using std::string in JlCxx Wrapper

## Overview

JlCxx has **built-in automatic conversion** for `std::string`. When you return `std::string` from a wrapped C++ method, JlCxx automatically converts it to Julia `String`. No manual marshalling needed.

---

## How It Works

### Basic Conversion

```cpp
// C++ side
mod.method("get_name", []() -> std::string {
  return "hello";  // C++ std::string
});

// Julia side
name = get_name()  # Julia String "hello"
```

### Automatic Direction Conversion

- **C++ → Julia**: `std::string` → `String`
- **Julia → C++**: `String` → `std::string` (when passed as parameter)

```cpp
// Accept Julia String, convert to std::string automatically
mod.method("process_name", [](const std::string& name) {
  // name is already converted from Julia String to std::string
  std::cout << name << std::endl;
});
```

### Collections

JlCxx also handles collections of strings:

```cpp
// std::vector<std::string> → Julia Vector{String}
mod.method("get_symbols", []() -> std::vector<std::string> {
  return {"BTC", "ETH", "XRP"};
});

// Result in Julia:
symbols = get_symbols()  # ["BTC", "ETH", "XRP"]
```

---

## Applications in Our Wrapper

### Phase 1: Optional Enhancement (ToString for Enums)

We could add string representations for enums:

```cpp
// Phase 1 enhancement
mod.method("string", [](databento::Schema s) -> std::string {
  return std::string(databento::ToString(s));
});

// Usage in Julia:
schema = Databento.MBO
str = string(schema)  # "Mbo"
```

**Decision**: Keep Phase 1 minimal without this. toString support can be added in Phase 2 if needed.

### Phase 3: Historical Client (Uses std::string Extensively)

The Historical client returns strings frequently:

```cpp
// MetadataListDatasets returns vector of dataset names
mod.add_type<databento::Historical>("Historical")
  .method("metadata_list_datasets",
    [](databento::Historical& client) -> std::vector<std::string> {
      return client.MetadataListDatasets();
      // Automatically converts: std::vector<std::string> → Julia Vector{String}
    });

// Usage in Julia:
historical = Historical()
datasets = historical.metadata_list_datasets()
# Result: ["glbx_mdp3", "xnas_itch", "bats_pitch", ...]
```

### Phase 4: DBN Reader (Uses std::string for Metadata)

```cpp
// Metadata structure uses strings for dataset name and symbols
mod.add_type<databento::Metadata>("Metadata")
  .method("dataset", [](const databento::Metadata& m) -> std::string {
    return m.dataset;  // Automatically converts to Julia String
  })
  .method("symbols", [](const databento::Metadata& m) -> std::vector<std::string> {
    return m.symbols;  // Automatically converts to Julia Vector{String}
  });

// Usage in Julia:
metadata = dbn_store.get_metadata()
dataset_name = metadata.dataset()  # "glbx_mdp3"
symbols = metadata.symbols()       # ["ES", "NQ", ...]
```

---

## Detailed Phase 3 Example

Historical client uses strings extensively:

```cpp
mod.add_type<databento::Historical>("Historical")
  // Get list of available datasets (each as string)
  .method("metadata_list_datasets",
    [](databento::Historical& client) -> std::vector<std::string> {
      return client.MetadataListDatasets();
    })

  // Get list of available schemas for a dataset
  .method("metadata_list_schemas",
    [](databento::Historical& client,
       const std::string& dataset) -> std::vector<databento::Schema> {
      // Note: std::string parameter automatically converted from Julia String
      return client.MetadataListSchemas(dataset);
    })

  // Get field names for encoding and schema combo
  .method("metadata_list_fields",
    [](databento::Historical& client,
       databento::Encoding encoding,
       databento::Schema schema) -> std::vector<std::string> {
      return client.MetadataListFields(encoding, schema);
      // Returns Julia Vector{String} automatically
    })

  // Resolve symbols to instrument IDs
  .method("symbology_resolve",
    [](databento::Historical& client,
       const std::string& dataset,
       const std::vector<std::string>& symbols,
       databento::SType stype_in,
       const std::string& date) -> std::string {
      // Multiple std::string parameters converted from Julia String
      // std::vector<std::string> converted from Julia Vector{String}
      // Returns Julia String automatically
      return client.SymbologyResolve(dataset, symbols, stype_in, date);
    })

  // Download data to file
  .method("timeseries_get_range_to_file",
    [](databento::Historical& client,
       const std::string& dataset,
       const std::vector<std::string>& symbols,
       databento::Schema schema,
       const std::string& start,
       const std::string& end,
       databento::SType stype_in,
       databento::SType stype_out,
       const std::string& output_file) -> std::string {
      // All strings converted automatically in both directions
      return client.TimeseriesGetRangeToFile(
        dataset, symbols, schema, start, end,
        stype_in, stype_out, output_file
      );
    });
```

**Julia usage** (when Phase 3 is implemented):

```julia
using Databento

# Create Historical client
historical = Databento.HistoricalBuilder()
    |> h -> set_key_from_env!(h)
    |> h -> build(h)

# All these return Julia String/Vector{String} automatically
datasets = historical.metadata_list_datasets()
schemas = historical.metadata_list_schemas("glbx_mdp3")
fields = historical.metadata_list_fields(DBN, MBO)

# String parameters work directly (no conversion needed)
resolved = historical.symbology_resolve(
    "glbx_mdp3",
    ["ES", "NQ"],
    RAW_SYMBOL,
    "2024-01-19"
)

# Download data
result = historical.timeseries_get_range_to_file(
    "glbx_mdp3",
    ["ES"],
    MBO,
    "2024-01-01",
    "2024-01-31",
    RAW_SYMBOL,
    RAW_SYMBOL,
    "data.dbn"
)
```

---

## Detailed Phase 4 Example

DBN Reader uses strings for metadata:

```cpp
mod.add_type<databento::Metadata>("Metadata")
  .method("version", [](const databento::Metadata& m) -> std::uint8_t {
    return m.version;
  })
  .method("dataset", [](const databento::Metadata& m) -> std::string {
    // Return std::string from field
    return m.dataset;  // Julia receives String
  })
  .method("symbols", [](const databento::Metadata& m) -> std::vector<std::string> {
    // Return collection of strings
    return m.symbols;  // Julia receives Vector{String}
  })
  .method("partial", [](const databento::Metadata& m) -> std::vector<std::string> {
    return m.partial;
  })
  .method("not_found", [](const databento::Metadata& m) -> std::vector<std::string> {
    return m.not_found;
  });
```

**Julia usage** (when Phase 4 is implemented):

```julia
using Databento

# Open DBN file
store = Databento.DbnFileStore("data.dbn")

# Get metadata - strings come back as Julia String/Vector{String}
metadata = store.get_metadata()

# All of these are now Julia types:
dataset = metadata.dataset()        # String
version = metadata.version()         # UInt8
symbols = metadata.symbols()         # Vector{String}
partial = metadata.partial()         # Vector{String}
not_found = metadata.not_found()     # Vector{String}

# Iterate through symbols
for sym in symbols
    println("Symbol: $sym")
end
```

---

## Key Takeaways

✅ **std::string is fully supported in JlCxx**
- No manual marshalling code needed
- Automatic bidirectional conversion
- Works with collections (vector<string>)

✅ **Phase 1**: Keep it simple (enums only, no strings needed)

✅ **Phase 3 & 4**: Use std::string extensively for:
- Method parameters
- Return values
- Vector collections
- Metadata fields

⚠️ **Performance**: String copying is involved, but acceptable for:
- Configuration strings (API keys, dates)
- Metadata lists (symbols, datasets)
- File paths

⚠️ **Large Data**: For high-throughput data (record streaming), avoid unnecessary string allocations

---

## Current Status in Our Wrapper

- **Phase 1** (`databento_jl.cpp`): No strings used (enums only)
- **Phase 2** (Not implemented): Minimal strings
- **Phase 3** (Not implemented): Extensive std::string usage
- **Phase 4** (Not implemented): Extensive std::string usage

---

## Implementation Checklist for Future Phases

- [ ] Phase 2: Add ToString() support for additional enums (optional)
- [ ] Phase 3: Wrap Historical client with full string parameter/return support
- [ ] Phase 4: Wrap Metadata type with string field accessors
- [ ] Phase 4: Wrap DbnFileStore returning strings from accessors
- [ ] Test: Verify Vector{String} marshalling works both directions
- [ ] Test: Verify large symbol lists pass correctly
- [ ] Performance: Benchmark string copies for large metadata

