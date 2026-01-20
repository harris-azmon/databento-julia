# Phase 5-6 Architecture & System Design

## System Architecture Overview

### High-Level Component Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         JULIA APPLICATION                        │
│  (User code accessing databento-julia wrapper)                   │
└─────────────────────────────────┬───────────────────────────────┘
                                  │
                    ┌─────────────┴─────────────┐
                    │                           │
         ┌──────────▼─────────────┐  ┌────────▼──────────────┐
         │ Phase 5: Live API     │  │ Phase 6: Historical  │
         │                       │  │                      │
         │ • LiveBlocking        │  │ • Historical         │
         │ • LiveThreaded        │  │ • Batch              │
         │ • Callbacks           │  │ • Metadata           │
         │ • Real-time Records   │  │ • Symbology          │
         └──────────┬────────────┘  └────────┬──────────────┘
                    │                        │
         ┌──────────┴────────────────────────┴──────────┐
         │                                              │
     ┌───▼───────────────────────────────────────────┬─┴────┐
     │         Phase 1-4: Core Infrastructure         │ │
     │  ┌─────────────────────────────────────────┐  │ │
     │  │ Enums (Schema, Encoding, SType, etc.)   │  │ │
     │  │ Record Types (Mbo, Trade, Mbp1, etc.)   │  │ │
     │  │ Supporting Types (FlagSet, UnixNanos)   │  │ │
     │  │ DBN File Reader (Metadata, Record)      │  │ │
     │  └─────────────────────────────────────────┘  │ │
     │                                                 │ │
     └─────────────────────────────────────────────────┘ │
                                                          │
                              ┌───────────────────────────┘
                              │
                  ┌───────────▼──────────────┐
                  │   C++ Wrapper Layer      │
                  │   (databento_jl.cpp)     │
                  │   2000+ lines of CxxWrap │
                  │   binding code           │
                  └───────────┬──────────────┘
                              │
                  ┌───────────▼──────────────┐
                  │   CxxWrap.jl Framework   │
                  │   Type marshalling       │
                  │   Memory management      │
                  │   Callback handling      │
                  └───────────┬──────────────┘
                              │
                  ┌───────────▼──────────────┐
                  │  databento-cpp v0.30.0   │
                  │  C++ Library             │
                  │  • Live API              │
                  │  • Historical API        │
                  │  • DBN codec             │
                  │  • HTTP client           │
                  └──────────────────────────┘
```

### Data Flow Diagrams

#### Phase 5: Live Streaming Data Flow

```
┌─────────────┐
│ Databento   │
│ Live Gate   │
│  WebSocket  │
└──────┬──────┘
       │ TCP/TLS Connection
       │
┌──────▼────────────────────────────┐
│ databento-cpp LiveBlocking/Threaded│
│                                   │
│ ┌──────────────────────────────┐  │
│ │ LiveBlocking                 │  │
│ │ ├─ NextRecord() [blocking]   │  │
│ │ └─ NextRecord(timeout_ms)    │  │
│ └──────────────────────────────┘  │
│                                   │
│ ┌──────────────────────────────┐  │
│ │ LiveThreaded                 │  │
│ │ ├─ OnRecord(callback)        │  │
│ │ └─ OnMetadata(callback)      │  │
│ └──────────────────────────────┘  │
└──────┬─────────────────────────────┘
       │
       │ Record objects
       │ (zero-copy via add_bits<T>)
       │
┌──────▼────────────────────────────┐
│ C++ Wrapper (databento_jl.cpp)    │
│                                   │
│ ┌──────────────────────────────┐  │
│ │ LiveBlocking wrapper         │  │
│ │ ├─ subscribe!(sub)           │  │
│ │ ├─ next_record()             │  │
│ │ └─ next_record_timeout(ms)   │  │
│ └──────────────────────────────┘  │
│                                   │
│ ┌──────────────────────────────┐  │
│ │ LiveThreaded wrapper         │  │
│ │ ├─ on_record(callback)       │  │
│ │ └─ block_for_stop()          │  │
│ └──────────────────────────────┘  │
└──────┬─────────────────────────────┘
       │
       │ CxxWrap type conversion
       │
┌──────▼────────────────────────────┐
│ Julia Application Code             │
│                                   │
│ client = build_blocking()         │
│ for record in stream              │
│    process(record)                │
└──────────────────────────────────┘
```

#### Phase 6: Historical Query Data Flow

```
┌──────────────┐         ┌──────────────┐
│ Metadata API │         │ Batch API    │
└────┬─────────┘         └────┬─────────┘
     │                        │
     │ HTTP REST Calls        │ HTTP REST Calls
     │                        │
┌────▼────────────────────────▼──────────┐
│ databento-cpp Historical Client         │
│                                        │
│ ┌─────────────────────────────────┐   │
│ │ Metadata Queries                │   │
│ │ ├─ ListDatasets()               │   │
│ │ ├─ ListSchemas(dataset)         │   │
│ │ ├─ GetRecordCount(...)          │   │
│ │ └─ GetCost(...)                 │   │
│ └─────────────────────────────────┘   │
│                                        │
│ ┌─────────────────────────────────┐   │
│ │ Batch Operations                │   │
│ │ ├─ SubmitJob(...)               │   │
│ │ ├─ ListJobs()                   │   │
│ │ ├─ ListFiles(job_id)            │   │
│ │ └─ Download(job_id, file)       │   │
│ └─────────────────────────────────┘   │
│                                        │
│ ┌─────────────────────────────────┐   │
│ │ Symbology                       │   │
│ │ └─ Resolve(symbols, date)       │   │
│ └─────────────────────────────────┘   │
│                                        │
│ ┌─────────────────────────────────┐   │
│ │ Timeseries                      │   │
│ │ └─ GetRangeToFile(...)          │   │
│ └─────────────────────────────────┘   │
└────┬────────────────────────────────┬──┘
     │                                │
     │ JSON/REST responses            │ DBN files
     │                                │
┌────▼────────────────────────────────▼──────┐
│ C++ Wrapper (databento_jl.cpp)             │
│                                           │
│ ┌─────────────────────────────────────┐  │
│ │ JSON → Struct Conversion             │  │
│ │ • BatchJob                           │  │
│ │ • PublisherDetail                    │  │
│ │ • DatasetRange                       │  │
│ │ • SymbologyResolution                │  │
│ └─────────────────────────────────────┘  │
│                                           │
│ ┌─────────────────────────────────────┐  │
│ │ File Handling                        │  │
│ │ • Download to disk                   │  │
│ │ • Return local paths                 │  │
│ └─────────────────────────────────────┘  │
└────┬──────────────────────────────────────┘
     │
     │ CxxWrap conversion
     │
┌────▼──────────────────────────────────────┐
│ Julia Application Code                   │
│                                          │
│ cost = client.metadata_get_cost(...)     │
│ job = client.batch_submit_job(...)       │
│ resolution = client.symbology_resolve(.)│
│ client.timeseries_get_range_to_file(...) │
└───────────────────────────────────────────┘
```

---

## Type Mapping Strategy

### IsBits Types (Zero-Copy)
```
C++ POD Type              Phase  → Julia Zero-Copy Access
─────────────────────────────────────────────────────
Schema                     1   ✓  Direct array indexing
Encoding                   1   ✓
SType                      1   ✓
RType                      2   ✓
Action                     2   ✓
Side                       2   ✓
FlagSet                    2   ✓
UnixNanos                  2   ✓
TimeDeltaNanos             2   ✓
BidAskPair                 2   ✓
RecordHeader               2   ✓
MboMsg                     2   ✓
TradeMsg                   2   ✓
Mbp1Msg                    2   ✓
Mbp10Msg                   2   ✓
InstrumentDefMsg           2   ✓
ImbalanceMsg               2   ✓
StatusMsg                  4   ✓
OhlcvMsg                   4   ✓
... (28 total)
LiveSubscription           5   ✓  Lightweight config struct
PublisherDetail            6   ✓  API metadata
FieldDetail                6   ✓
DatasetRange               6   ✓
DatasetConditionDetail     6   ✓
BatchFileDesc              6   ✓
BatchJob                   6   ✓  Batch job status
```

### Non-POD Types (Object References)
```
C++ Class Type            Phase  Julia Access Model
──────────────────────────────────────────────────
LiveBlocking               5   ✓  Reference to C++ object
LiveThreaded               5   ✓  Reference to C++ object
Historical                 3   ✓  Reference to C++ object
HistoricalBuilder          3   ✓  Reference to C++ object
DbnFileStore               4   ✓  Reference to C++ object
SymbologyResolution        6   ✓  Converted to Julia dict/struct
Record                     4   ✓  Reference with type dispatch
Metadata                   4   ✓  Reference to file metadata
```

### String & Collection Handling
```
C++ Type                          → Julia Type
─────────────────────────────────────────────
const char[N]                     → String (conversion)
std::string                       → String (conversion)
std::vector<std::string>          → Vector{String}
std::vector<T>                    → Vector{T}
unordered_map<K, V>              → Vector{Tuple{K,V}}
std::optional<T>                  → Union{T, Nothing}
std::function<R(Args)>           → Julia Function
```

---

## Method Organization in Phase 5

```cpp
// Phase 5: Live Streaming (600-800 lines total)

// 1. IsBits & Includes (20 lines)
#include <databento/live*.hpp>
#include <functional>, <optional>, <chrono>
template<> struct IsBits<LiveSubscription> : std::true_type {};

// 2. LiveSubscription (25 lines)
mod.add_type<LiveSubscription>("LiveSubscription")
  .constructor<>()
  .method("set_symbols!", ...)
  .method("set_schema!", ...)
  .method("set_stype!", ...)
  .method("symbols", ...)
  .method("schema", ...)
  .method("stype_in", ...);

// 3. LiveBlocking (120 lines)
mod.add_type<LiveBlocking>("LiveBlocking")
  .constructor<>()
  .method("subscribe!", ...)              // 5 lines
  .method("start!", ...)                  // 3 lines
  .method("stop!", ...)                   // 3 lines
  .method("reconnect!", ...)              // 3 lines
  .method("next_record", ...)             // 8 lines (blocking)
  .method("next_record_timeout", ...)     // 10 lines (with timeout)
  .method("is_subscribed", ...);          // 3 lines

// 4. LiveBuilder (60 lines)
mod.add_type<LiveBuilder>("LiveBuilder")
  .constructor<>()
  .method("set_key!", ...)                // 5 lines
  .method("set_key_from_env!", ...)       // 5 lines
  .method("set_dataset!", ...)            // 5 lines
  .method("set_send_ts_out!", ...)        // 5 lines
  .method("set_heartbeat_interval!", ...) // 8 lines
  .method("build_blocking", ...)          // 5 lines
  .method("build_threaded", ...);         // 5 lines

// 5. LiveThreaded (80 lines)
mod.add_type<LiveThreaded>("LiveThreaded")
  .constructor<>()
  .method("subscribe!", ...)              // 5 lines
  .method("on_record", ...)               // 10 lines (callback)
  .method("on_metadata", ...)             // 10 lines (callback)
  .method("start!", ...)                  // 5 lines
  .method("block_for_stop", ...)          // 8 lines (blocking)
  .method("block_for_stop_timeout", ...); // 10 lines (timeout)

// 6. Comments & Formatting (100 lines)
```

---

## Method Organization in Phase 6

```cpp
// Phase 6: Historical/Batch API (900-1200 lines total)

// 1. Includes & IsBits (20 lines)
#include <databento/batch.hpp>
#include <databento/symbology.hpp>
template<> struct IsBits<BatchJob> : std::true_type {};
// ... 6 more IsBits specs

// 2. PublisherDetail (15 lines)
mod.add_bits<PublisherDetail>("PublisherDetail")
  .method("publisher_id", ...)
  .method("dataset", ...)
  .method("venue", ...)
  .method("description", ...);

// 3. FieldDetail (10 lines)
mod.add_bits<FieldDetail>("FieldDetail")
  .method("name", ...)
  .method("type", ...);

// 4. DatasetRange (10 lines)
mod.add_bits<DatasetRange>("DatasetRange")
  .method("start_date", ...)
  .method("end_date", ...);

// 5. DatasetConditionDetail (15 lines)
mod.add_bits<DatasetConditionDetail>("DatasetConditionDetail")
  .method("date", ...)
  .method("condition", ...)
  .method("last_modified_date", ...);

// 6. BatchFileDesc (25 lines)
mod.add_bits<BatchFileDesc>("BatchFileDesc")
  .method("filename", ...)
  .method("size", ...)
  .method("hash", ...)
  .method("https_url", ...)
  .method("ftp_url", ...);

// 7. BatchJob (60 lines)
mod.add_bits<BatchJob>("BatchJob")
  .method("id", ...)
  .method("user_id", ...)
  // ... 20+ more accessors

// 8. SymbologyResolution (40 lines)
mod.add_type<SymbologyResolution>("SymbologyResolution")
  .method("stype_in", ...)
  .method("stype_out", ...)
  .method("mappings", ...)           // Complex map conversion
  .method("partial", ...)
  .method("not_found", ...);

// 9. Enhanced Historical Client (600 lines)

// 9a. Metadata API (70 lines)
.method("metadata_list_publishers", ...)
.method("metadata_list_datasets", ...)
.method("metadata_list_datasets_with_range", ...)
.method("metadata_list_schemas", ...)
.method("metadata_list_fields", ...)
// ... more metadata methods

// 9b. Record Count & Billing (50 lines)
.method("metadata_get_record_count", ...)
.method("metadata_get_billable_size", ...)
.method("metadata_get_cost", ...)

// 9c. Symbology (20 lines)
.method("symbology_resolve", ...)

// 9d. Batch API (100 lines)
.method("batch_submit_job", ...)
.method("batch_list_jobs", ...)
.method("batch_list_files", ...)
.method("batch_download_file", ...)

// 9e. Timeseries (40 lines)
.method("timeseries_get_range_to_file", ...)

// 10. Comments & Formatting (150 lines)
```

---

## Integration Points with Phases 1-4

### Phase 1 Dependencies (Already Satisfied)
```
Phase 5: LiveSubscription
  └─ uses databento::Schema (Phase 1)
  └─ uses databento::SType (Phase 1)

Phase 6: Enhanced Historical
  └─ uses databento::Schema (Phase 1)
  └─ uses databento::Encoding (Phase 1)
  └─ uses databento::SType (Phase 1)
```

### Phase 2 Dependencies (Already Satisfied)
```
Phase 5: Live Records
  └─ returns databento::Record (Phase 4)
     ├─ contains Phase 2 message types

Phase 6: DBN File Handling
  └─ timeseries_get_range_to_file returns DBN
     → readable by Phase 4 DbnFileStore
```

### Phase 3 Dependencies (Already Satisfied)
```
Phase 6: Enhanced Historical
  └─ extends Phase 3 Historical class
  └─ adds 25 new methods to existing class
```

### Phase 4 Dependencies (Already Satisfied)
```
Phase 5: Live Record Processing
  └─ Records are databento::Record type
  └─ Can be processed with Phase 4 Record methods

Phase 6: File Processing
  └─ Downloaded files are standard DBN format
  └─ Compatible with Phase 4 DbnFileStore
```

---

## Testing Architecture

### Unit Test Structure
```
test_phase5.jl
├─ LiveBuilder Tests
│  ├─ Initialization
│  ├─ Configuration methods
│  └─ Factory methods (build_blocking, build_threaded)
├─ LiveSubscription Tests
│  ├─ Construction
│  ├─ Configuration
│  └─ Getter verification
├─ LiveBlocking Tests
│  ├─ Connection establishment
│  ├─ Subscription
│  ├─ Record retrieval
│  ├─ Timeout handling
│  └─ Reconnection
└─ LiveThreaded Tests
   ├─ Callback registration
   ├─ Async event handling
   └─ Graceful shutdown

test_phase6.jl
├─ Metadata API Tests
│  ├─ List queries
│  ├─ Date ranges
│  └─ Record counts
├─ Batch API Tests
│  ├─ Job submission
│  ├─ Job listing
│  ├─ File retrieval
│  └─ Download functionality
├─ Billing API Tests
│  ├─ Cost estimation
│  └─ Billable size calculation
├─ Symbology Tests
│  ├─ Symbol resolution
│  ├─ Date handling
│  └─ Partial matches
└─ Timeseries Tests
   ├─ Direct downloads
   └─ File format validation
```

---

## Deployment Checklist

### Before Phase 5 Implementation
- [ ] Review databento-cpp live.hpp API
- [ ] Test LiveSubscription configuration patterns
- [ ] Verify callback std::function handling in CxxWrap
- [ ] Determine threading model implications for Julia
- [ ] Plan exception propagation strategy

### Before Phase 6 Implementation
- [ ] Review databento-cpp batch.hpp API
- [ ] Test date range handling patterns
- [ ] Verify map/set conversion to Julia types
- [ ] Plan HTTP error handling
- [ ] Test file download integration with Phase 4

### Deployment
- [ ] All Phase 5 unit tests pass
- [ ] All Phase 6 unit tests pass
- [ ] Integration tests with real API key
- [ ] Performance benchmarks
- [ ] Documentation complete
- [ ] Examples functional
- [ ] Code review approved
- [ ] Tagged release ready

---

## Performance Targets

### Phase 5 (Live Streaming)
- Record latency: < 1ms (after receipt)
- Subscription setup: < 100ms
- Connection overhead: < 500ms
- Memory per connection: < 50MB

### Phase 6 (Historical/Batch)
- Metadata query: < 100ms
- Cost estimation: < 50ms
- Batch submission: < 500ms
- File listing: < 200ms
- Symbology resolution: < 200ms

### Overall
- Zero-copy overhead: < 1% (via add_bits<T>)
- Total wrapper size: 2000-2400 lines
- Compile time: < 30 seconds
- Load time: < 2 seconds

---

## References & Links

- [databento-cpp live.hpp](../deps/include/databento/live.hpp)
- [databento-cpp historical.hpp](../deps/include/databento/historical.hpp)
- [databento-cpp batch.hpp](../deps/include/databento/batch.hpp)
- [CxxWrap.jl Docs](https://github.com/JuliaInterop/CxxWrap.jl)
- [Phase 1-4 Implementation](./COMPREHENSIVE_PHASE_REPORT.md)

