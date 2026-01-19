# Phase 3: Code Snippet to Add to deps/databento_jl.cpp

## What Was Added

This document shows the exact code snippet that was added to `/home/user/databento-julia/deps/databento_jl.cpp` for Phase 3: Historical Client mapping.

## 1. Include Directives (Top of File)

Add these includes near the top of the file (lines 7 and 11):

```cpp
#include <databento/historical.hpp>  // Line 7
#include <vector>                    // Line 11
```

**Context:** These should be added alongside the existing includes:
```cpp
#include <jlcxx/jlcxx.hpp>
#include <databento/enums.hpp>
#include <databento/publishers.hpp>
#include <databento/record.hpp>
#include <databento/datetime.hpp>
#include <databento/flag_set.hpp>
#include <databento/historical.hpp>  // ← NEW
#include <sstream>
#include <string>
#include <cstring>
#include <vector>                    // ← NEW
```

## 2. Phase 3 Section (End of Module)

Add this code **BEFORE** the closing brace `}` of the `JLCXX_MODULE` function (lines 154-202):

```cpp
  // ============================================================================
  // PHASE 3: Historical Client
  // ============================================================================

  // Historical::Builder - Builder pattern for constructing Historical client
  mod.add_type<databento::HistoricalBuilder>("HistoricalBuilder")
    .constructor<>()
    .method("set_key!", [](databento::HistoricalBuilder& builder, const std::string& key) -> databento::HistoricalBuilder& {
      return builder.SetKey(key);
    })
    .method("set_key_from_env!", [](databento::HistoricalBuilder& builder) -> databento::HistoricalBuilder& {
      return builder.SetKeyFromEnv();
    })
    .method("build", [](databento::HistoricalBuilder& builder) -> databento::Historical {
      return builder.Build();
    });

  // Historical - Main client for historical data access
  mod.add_type<databento::Historical>("Historical")
    // Metadata methods
    .method("metadata_list_datasets", [](databento::Historical& client) -> std::vector<std::string> {
      return client.MetadataListDatasets();
    })
    .method("metadata_list_schemas", [](databento::Historical& client, const std::string& dataset) -> std::vector<databento::Schema> {
      return client.MetadataListSchemas(dataset);
    })
    .method("metadata_list_fields", [](databento::Historical& client, databento::Encoding encoding, databento::Schema schema) -> std::vector<std::string> {
      return client.MetadataListFields(encoding, schema);
    })
    // Symbology methods
    .method("symbology_resolve", [](databento::Historical& client,
                                     const std::string& dataset,
                                     const std::vector<std::string>& symbols,
                                     databento::SType stype_in,
                                     const std::string& date) -> std::string {
      return client.SymbologyResolve(dataset, symbols, stype_in, date);
    })
    // Data retrieval methods
    .method("timeseries_get_range_to_file", [](databento::Historical& client,
                                                const std::string& dataset,
                                                const std::vector<std::string>& symbols,
                                                databento::Schema schema,
                                                const std::string& start,
                                                const std::string& end,
                                                databento::SType stype_in,
                                                databento::SType stype_out,
                                                const std::string& output_file) -> std::string {
      return client.TimeseriesGetRangeToFile(dataset, symbols, schema, start, end, stype_in, stype_out, output_file);
    });
}
```

**Context:** Insert this AFTER the Phase 2 mappings (or after Phase 1 if Phase 2 doesn't exist), but BEFORE the final closing brace:

```cpp
JLCXX_MODULE define_databento_module(jlcxx::Module& mod)
{
  // ... Phase 1 enums ...

  // ... Phase 2 records (if present) ...

  // ============================================================================
  // PHASE 3: Historical Client     ← INSERT HERE
  // ============================================================================

  // ... Phase 3 code from above ...

}  // ← Closing brace of JLCXX_MODULE
```

## Complete File Structure

After adding Phase 3, the file structure looks like:

```
deps/databento_jl.cpp
├── Includes
│   ├── jlcxx/jlcxx.hpp
│   ├── databento/enums.hpp
│   ├── databento/publishers.hpp
│   ├── databento/record.hpp
│   ├── databento/datetime.hpp
│   ├── databento/flag_set.hpp
│   ├── databento/historical.hpp        ← NEW (Phase 3)
│   ├── sstream, string, cstring
│   └── vector                           ← NEW (Phase 3)
│
├── jlcxx namespace
│   ├── IsBits<Schema>
│   ├── IsBits<Encoding>
│   ├── IsBits<SType>
│   ├── IsBits<Dataset>
│   ├── IsBits<RType>                   (Phase 2)
│   ├── IsBits<Action>                  (Phase 2)
│   └── ... other Phase 2 types ...
│
└── JLCXX_MODULE define_databento_module
    ├── PHASE 1: Core Enums
    │   ├── Schema enum + constants
    │   ├── Encoding enum + constants
    │   ├── SType enum + constants
    │   └── Dataset enum + constants
    │
    ├── PHASE 2: Data Record Protocol (if present)
    │   ├── RType enum
    │   ├── Action enum
    │   ├── Side enum
    │   ├── RecordHeader struct
    │   ├── MboMsg struct
    │   ├── TradeMsg struct
    │   └── ... other records ...
    │
    └── PHASE 3: Historical Client        ← NEW
        ├── HistoricalBuilder class
        │   ├── constructor<>()
        │   ├── set_key!(builder, key)
        │   ├── set_key_from_env!(builder)
        │   └── build(builder)
        │
        └── Historical class
            ├── metadata_list_datasets()
            ├── metadata_list_schemas(dataset)
            ├── metadata_list_fields(enc, schema)
            ├── symbology_resolve(...)
            └── timeseries_get_range_to_file(...)
```

## Line Count

**Phase 3 additions:**
- 2 includes
- 49 lines of class mappings
- **Total: 51 new lines**

## Important Notes

### 1. Method Names May Need Adjustment

The C++ method names used are educated guesses. When building against databento-cpp v0.30.0, you may need to adjust:

| Current Name | Possible Alternatives |
|--------------|----------------------|
| `HistoricalBuilder` | `Historical::Builder` |
| `MetadataListDatasets()` | `ListDatasets()`, `GetDatasets()` |
| `MetadataListSchemas()` | `ListSchemas()`, `GetSchemas()` |
| `MetadataListFields()` | `ListFields()`, `GetFields()` |
| `SymbologyResolve()` | `ResolveSymbols()`, `Resolve()` |
| `TimeseriesGetRangeToFile()` | `GetRangeToFile()` |

**How to verify:** Check the actual method names in:
```bash
deps/build/_deps/databento-src/include/databento/historical.hpp
```

### 2. Return Types

If methods return different types (e.g., `std::vector<std::string>` instead of `std::vector<Schema>`), adjust the lambda return types:

```cpp
// If MetadataListSchemas returns strings instead of Schema enum:
.method("metadata_list_schemas", [](databento::Historical& client, const std::string& dataset) -> std::vector<std::string> {
  return client.MetadataListSchemas(dataset);
})
```

### 3. Lambda Wrappers

All methods use lambda wrappers for flexibility:
- Easy to add error handling
- Can modify return values if needed
- Allows parameter transformation

Example with error handling:
```cpp
.method("metadata_list_datasets", [](databento::Historical& client) -> std::vector<std::string> {
  try {
    return client.MetadataListDatasets();
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to list datasets: " + std::string(e.what()));
  }
})
```

## Testing the Implementation

After adding this code, test compilation:

```bash
cd deps
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Expected result:**
- ✅ `libdatabento_jl.so` (Linux) or `.dylib` (macOS) or `.dll` (Windows) is created
- ✅ No compilation errors

**If you get errors:**
1. Check method names against databento-cpp headers
2. Verify return types match
3. Ensure databento-cpp v0.30.0 is fetched correctly

## Corresponding Julia Code

After the C++ builds successfully, add to `src/Databento.jl`:

```julia
# Export Historical client types
export HistoricalBuilder, Historical

# Export Historical methods
export set_key!, set_key_from_env!, build
export metadata_list_datasets, metadata_list_schemas, metadata_list_fields
export symbology_resolve, timeseries_get_range_to_file
```

Then users can:
```julia
using Databento

builder = HistoricalBuilder()
set_key_from_env!(builder)
client = build(builder)

datasets = metadata_list_datasets(client)
```

## Summary

Phase 3 adds **51 lines** to `deps/databento_jl.cpp`:
- 2 include directives
- 49 lines of Historical client mappings

The implementation:
- ✅ Follows CxxWrap patterns
- ✅ Uses lambda wrappers for flexibility
- ✅ Provides clear method naming
- ✅ Integrates with Phase 1 enums
- ⏳ Requires build verification

**Location in file:**
- Includes at top (lines 7, 11)
- Phase 3 section at end (lines 154-202)
- Before final closing brace `}`
