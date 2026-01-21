# Phases 5-6 Implementation Plan
## databento-julia C++ Wrapper Expansion

**Date:** 2026-01-20
**Status:** Planning
**Branch:** `claude/phase-2-julia-wrapper-CZxKh`

---

## Executive Summary

This document outlines the detailed implementation plan for Phases 5 and 6 of the databento-julia wrapper, based on:
1. Comprehensive analysis of databento-cpp v0.30.0 header files
2. Julia/CxxWrap best practices for callbacks, threading, and async operations
3. Architectural decisions for high-performance real-time and batch data access

### Timeline
- **Phase 5:** Live Streaming API (Real-time data)
- **Phase 6:** Historical/Batch API (Historical data queries)

### Estimated Code Additions
- **Phase 5:** 600-800 lines of C++
- **Phase 6:** 900-1200 lines of C++
- **Total wrapper size:** 1900-2400 lines for completed Phases 1-6

---

## Phase 5: Live Streaming API

### Purpose
Enable real-time market data streaming from Databento's live gateway with support for multiple subscription models and callback-based event handling.

### Key Classes & Structures

#### 5.1 LiveSubscription
**File:** `deps/databento_jl.cpp` (new section)

**Julia Wrapper Requirements:**
```cpp
// LiveSubscription - Subscription request specification
mod.add_type<databento::LiveSubscription>("LiveSubscription")
  .constructor<>()
  .method("set_symbols!", [](databento::LiveSubscription& sub,
                              const std::vector<std::string>& symbols) -> databento::LiveSubscription& {
    return sub.SetSymbols(symbols);
  })
  .method("set_schema!", [](databento::LiveSubscription& sub,
                             databento::Schema schema) -> databento::LiveSubscription& {
    return sub.SetSchema(schema);
  })
  .method("set_stype!", [](databento::LiveSubscription& sub,
                            databento::SType stype) -> databento::LiveSubscription& {
    return sub.SetSType(stype);
  })
  .method("symbols", [](const databento::LiveSubscription& sub) {
    return sub.symbols;
  })
  .method("schema", [](const databento::LiveSubscription& sub) {
    return sub.schema;
  })
  .method("stype_in", [](const databento::LiveSubscription& sub) {
    return sub.stype_in;
  });
```

**Design Rationale:**
- Builder pattern for subscription configuration
- Immutable once subscribed (allows method chaining with `!` suffix)
- Supports schema-specific subscriptions

#### 5.2 LiveBlocking
**File:** `deps/databento_jl.cpp`

**Julia Wrapper Requirements:**
```cpp
// LiveBlocking - Blocking/polling API for live data
mod.add_type<databento::LiveBlocking>("LiveBlocking")
  .constructor<>()
  .method("subscribe!", [](databento::LiveBlocking& live,
                            const databento::LiveSubscription& sub) {
    return live.Subscribe(sub);
  })
  .method("start!", [](databento::LiveBlocking& live) {
    return live.Start();
  })
  .method("next_record", [](databento::LiveBlocking& live) -> databento::Record {
    return live.NextRecord();
  })
  .method("next_record_timeout", [](databento::LiveBlocking& live,
                                     uint64_t timeout_ms) -> std::optional<databento::Record> {
    auto result = live.NextRecord(std::chrono::milliseconds(timeout_ms));
    if (result) return *result;
    return std::nullopt;
  })
  .method("stop!", [](databento::LiveBlocking& live) {
    return live.Stop();
  })
  .method("reconnect!", [](databento::LiveBlocking& live) {
    return live.Reconnect();
  })
  .method("is_subscribed", [](const databento::LiveBlocking& live) {
    return !live.Subscriptions().empty();
  });
```

**Design Rationale:**
- Blocking API for Julia's synchronous model
- `next_record()` blocks indefinitely (caller controls timeout via Julia-side async)
- `next_record_timeout()` for polling patterns
- Methods return `self` for chaining (mutation methods end with `!`)

#### 5.3 LiveThreaded
**File:** `deps/databento_jl.cpp`

**Julia Wrapper Requirements:**
```cpp
// LiveThreaded - Event-driven threaded API
// Note: This is more complex - requires callback handling

mod.add_type<databento::LiveThreaded>("LiveThreaded")
  .constructor<>()
  .method("subscribe!", [](databento::LiveThreaded& live,
                            const databento::LiveSubscription& sub) {
    return live.Subscribe(sub);
  })
  // Callback registration methods require special handling
  .method("on_record", [](databento::LiveThreaded& live,
                          std::function<databento::KeepGoing(const databento::Record&)> cb) {
    live.SetRecordCallback(cb);
  })
  .method("on_metadata", [](databento::LiveThreaded& live,
                            std::function<void(databento::Metadata&&)> cb) {
    live.SetMetadataCallback(cb);
  })
  .method("start!", [](databento::LiveThreaded& live) {
    return live.Start();
  })
  .method("block_for_stop", [](databento::LiveThreaded& live) {
    return live.BlockForStop();
  })
  .method("block_for_stop_timeout", [](databento::LiveThreaded& live,
                                        uint64_t timeout_ms) {
    return live.BlockForStop(std::chrono::milliseconds(timeout_ms));
  });
```

**Design Rationale:**
- Event-driven model with callbacks for record/metadata
- Callbacks handled via `std::function` (auto-converted from Julia closures)
- Thread management delegated to databento-cpp
- Julia exception handling propagates through callbacks

#### 5.4 LiveBuilder
**File:** `deps/databento_jl.cpp`

**Julia Wrapper Requirements:**
```cpp
// LiveBuilder - Builder pattern for Live client
mod.add_type<databento::LiveBuilder>("LiveBuilder")
  .constructor<>()
  .method("set_key!", [](databento::LiveBuilder& builder,
                         const std::string& key) -> databento::LiveBuilder& {
    return builder.SetKey(key);
  })
  .method("set_key_from_env!", [](databento::LiveBuilder& builder) -> databento::LiveBuilder& {
    return builder.SetKeyFromEnv("DATABENTO_API_KEY");
  })
  .method("set_dataset!", [](databento::LiveBuilder& builder,
                             const std::string& dataset) -> databento::LiveBuilder& {
    return builder.SetDataset(dataset);
  })
  .method("set_send_ts_out!", [](databento::LiveBuilder& builder,
                                 bool send) -> databento::LiveBuilder& {
    return builder.SetSendTsOut(send);
  })
  .method("set_heartbeat_interval!", [](databento::LiveBuilder& builder,
                                       uint64_t interval_ms) -> databento::LiveBuilder& {
    return builder.SetHeartbeatInterval(std::chrono::milliseconds(interval_ms));
  })
  .method("build_blocking", [](databento::LiveBuilder& builder) -> databento::LiveBlocking {
    return builder.BuildBlocking();
  })
  .method("build_threaded", [](databento::LiveBuilder& builder) -> databento::LiveThreaded {
    return builder.BuildThreaded();
  });
```

**Design Rationale:**
- Builder pattern enables fluent API in Julia
- Methods return builder for chaining
- `build_blocking()` and `build_threaded()` create appropriate client variant

### 5.5 IsBits Specializations for Phase 5

```cpp
// Phase 5: Live Streaming
template<> struct IsBits<databento::LiveSubscription> : std::true_type {};
// LiveBlocking and LiveThreaded are non-POD (contain pointers/threads)
// Metadata is already wrapped in Phase 4
```

### 5.6 New Includes Required

```cpp
#include <databento/live.hpp>
#include <databento/live_blocking.hpp>
#include <databento/live_threaded.hpp>
#include <databento/live_subscription.hpp>
#include <functional>        // for std::function
#include <optional>         // for std::optional
#include <chrono>          // for duration types
```

### 5.7 Julia-Side API Usage Examples

```julia
# Create builder and configure
builder = Databento.LiveBuilder()
builder.set_key_from_env!()
builder.set_dataset!("GLBX")

# Create subscription
sub = Databento.LiveSubscription()
sub.set_symbols!(["ES", "NQ"])
sub.set_schema!(Databento.MBP1)
sub.set_stype!(Databento.RAW_SYMBOL)

# Blocking API
live = builder.build_blocking()
live.subscribe!(sub)
live.start!()

# Poll for records
while true
    record_opt = live.next_record_timeout(1000)
    if !isnothing(record_opt)
        record = record_opt
        # Process record
    end
    # Check for stop condition
end

# Threaded API (event-driven)
live_threaded = builder.build_threaded()
live_threaded.on_record() do record
    # Process record asynchronously
    println("Got record: $(record.rtype())")
    return Databento.KeepGoing  # or Databento.Stop
end
live_threaded.subscribe!(sub)
live_threaded.start!()
live_threaded.block_for_stop()
```

### 5.8 Testing Strategy

**Unit Tests (`test_phase5.jl`):**
- Builder initialization and configuration
- Subscription creation and modification
- Connection/reconnection logic
- Record iteration (with mock data if needed)
- Timeout handling

**Integration Tests (requires API key):**
- Real connection to live gateway
- Record type checking (schema validation)
- Callback execution
- Graceful shutdown

---

## Phase 6: Historical/Batch API

### Purpose
Enable historical data queries, batch processing, metadata access, and cost estimation through the Databento Historical API.

### Key Classes & Structures

#### 6.1 Batch Structures
**File:** `deps/databento_jl.cpp` (extend Phase 3 Historical section)

**Julia Wrapper Requirements:**
```cpp
// BatchJob - Representation of a submitted batch job
mod.add_bits<databento::BatchJob>("BatchJob");

// Numeric fields - Direct access
mod.method("id", [](const databento::BatchJob& job) { return job.id; });
mod.method("user_id", [](const databento::BatchJob& job) { return job.user_id; });
mod.method("bill_id", [](const databento::BatchJob& job) { return job.bill_id; });
mod.method("cost_usd", [](const databento::BatchJob& job) { return job.cost_usd; });

// String fields - Convert to Julia strings
mod.method("dataset", [](const databento::BatchJob& job) {
  return std::string(job.dataset);
});
mod.method("state", [](const databento::BatchJob& job) {
  return std::string(job.state);
});

// Enum fields
mod.method("schema", [](const databento::BatchJob& job) { return job.schema; });
mod.method("encoding", [](const databento::BatchJob& job) { return job.encoding; });
mod.method("compression", [](const databento::BatchJob& job) {
  return job.compression;
});
mod.method("stype_in", [](const databento::BatchJob& job) {
  return job.stype_in;
});
mod.method("stype_out", [](const databento::BatchJob& job) {
  return job.stype_out;
});

// Timestamp fields (as UnixNanos)
mod.method("ts_received", [](const databento::BatchJob& job) {
  return job.ts_received;
});
mod.method("ts_queued", [](const databento::BatchJob& job) {
  return job.ts_queued;
});
mod.method("ts_process_start", [](const databento::BatchJob& job) {
  return job.ts_process_start;
});
mod.method("ts_process_done", [](const databento::BatchJob& job) {
  return job.ts_process_done;
});
mod.method("ts_expiration", [](const databento::BatchJob& job) {
  return job.ts_expiration;
});

// Size fields
mod.method("record_count", [](const databento::BatchJob& job) {
  return job.record_count;
});
mod.method("billed_size", [](const databento::BatchJob& job) {
  return job.billed_size;
});
mod.method("actual_size", [](const databento::BatchJob& job) {
  return job.actual_size;
});
mod.method("package_size", [](const databento::BatchJob& job) {
  return job.package_size;
});

// Symbol arrays
mod.method("symbols", [](const databento::BatchJob& job) {
  std::vector<std::string> result;
  for (const auto& s : job.symbols) {
    result.push_back(std::string(s));
  }
  return result;
});

// BatchFileDesc - File metadata within batch job
mod.add_bits<databento::BatchFileDesc>("BatchFileDesc");
mod.method("filename", [](const databento::BatchFileDesc& desc) {
  return std::string(desc.filename);
});
mod.method("size", [](const databento::BatchFileDesc& desc) { return desc.size; });
mod.method("hash", [](const databento::BatchFileDesc& desc) {
  return std::string(desc.hash);
});
mod.method("https_url", [](const databento::BatchFileDesc& desc) {
  return std::string(desc.https_url);
});
mod.method("ftp_url", [](const databento::BatchFileDesc& desc) {
  return std::string(desc.ftp_url);
});
```

**Design Rationale:**
- POD types use `add_bits<T>()` for zero-copy
- String fields explicitly converted to Julia strings
- URL fields accessible for direct download
- All timestamps as `UnixNanos` for consistency

#### 6.2 Metadata Structures
**File:** `deps/databento_jl.cpp`

**Julia Wrapper Requirements:**
```cpp
// PublisherDetail - Publisher information
mod.add_bits<databento::PublisherDetail>("PublisherDetail");
mod.method("publisher_id", [](const databento::PublisherDetail& p) {
  return p.publisher_id;
});
mod.method("dataset", [](const databento::PublisherDetail& p) {
  return std::string(p.dataset);
});
mod.method("venue", [](const databento::PublisherDetail& p) {
  return std::string(p.venue);
});
mod.method("description", [](const databento::PublisherDetail& p) {
  return std::string(p.description);
});

// FieldDetail - Record field metadata
mod.add_bits<databento::FieldDetail>("FieldDetail");
mod.method("name", [](const databento::FieldDetail& f) {
  return std::string(f.name);
});
mod.method("type", [](const databento::FieldDetail& f) {
  return std::string(f.type);
});

// DatasetRange - Date range availability
mod.add_bits<databento::DatasetRange>("DatasetRange");
mod.method("start_date", [](const databento::DatasetRange& r) {
  return std::string(r.start_date);
});
mod.method("end_date", [](const databento::DatasetRange& r) {
  return std::string(r.end_date);
});

// DatasetConditionDetail - Dataset quality metadata
mod.add_bits<databento::DatasetConditionDetail>("DatasetConditionDetail");
mod.method("date", [](const databento::DatasetConditionDetail& d) {
  return std::string(d.date);
});
mod.method("condition", [](const databento::DatasetConditionDetail& d) {
  return std::string(d.condition);
});
mod.method("last_modified_date", [](const databento::DatasetConditionDetail& d) {
  return std::string(d.last_modified_date);
});
```

#### 6.3 Enhanced Historical Client Methods
**File:** `deps/databento_jl.cpp` (replace Phase 3 Historical section)

**Julia Wrapper Requirements:**
```cpp
// Replace the Phase 3 Historical with this expanded version
mod.add_type<databento::Historical>("Historical")
  // ========== METADATA API ==========
  .method("metadata_list_publishers", [](databento::Historical& client)
    -> std::vector<databento::PublisherDetail> {
    return client.MetadataListPublishers();
  })
  .method("metadata_list_datasets", [](databento::Historical& client)
    -> std::vector<std::string> {
    return client.MetadataListDatasets();
  })
  .method("metadata_list_datasets_with_range", [](databento::Historical& client,
                                                   const std::string& start_date,
                                                   const std::string& end_date)
    -> std::vector<std::string> {
    databento::DateRange range{start_date, end_date};
    return client.MetadataListDatasets(range);
  })
  .method("metadata_list_schemas", [](databento::Historical& client,
                                      const std::string& dataset)
    -> std::vector<std::string> {
    return client.MetadataListSchemas(dataset);
  })
  .method("metadata_list_fields", [](databento::Historical& client,
                                     databento::Encoding encoding,
                                     databento::Schema schema)
    -> std::vector<databento::FieldDetail> {
    return client.MetadataListFields(encoding, schema);
  })
  .method("metadata_list_unit_prices", [](databento::Historical& client,
                                          const std::string& dataset)
    -> std::vector<std::string> {
    return client.MetadataListUnitPrices(dataset);
  })
  .method("metadata_get_dataset_range", [](databento::Historical& client,
                                           const std::string& dataset)
    -> databento::DatasetRange {
    return client.MetadataGetDatasetRange(dataset);
  })
  .method("metadata_get_dataset_condition", [](databento::Historical& client,
                                               const std::string& dataset,
                                               const std::string& start_date,
                                               const std::string& end_date)
    -> std::vector<databento::DatasetConditionDetail> {
    databento::DateRange range{start_date, end_date};
    return client.MetadataGetDatasetCondition(dataset, range);
  })

  // ========== RECORD COUNT API ==========
  .method("metadata_get_record_count", [](databento::Historical& client,
                                          const std::string& dataset,
                                          const std::vector<std::string>& symbols,
                                          databento::Schema schema,
                                          const std::string& start_date,
                                          const std::string& end_date,
                                          databento::SType stype_in,
                                          databento::SType stype_out)
    -> uint64_t {
    databento::DateTimeRange<std::string> range{start_date, end_date};
    return client.MetadataGetRecordCount(dataset, symbols, schema, range,
                                        stype_in, stype_out);
  })

  // ========== BILLING API ==========
  .method("metadata_get_billable_size", [](databento::Historical& client,
                                           const std::string& dataset,
                                           const std::vector<std::string>& symbols,
                                           databento::Schema schema,
                                           const std::string& start_date,
                                           const std::string& end_date,
                                           databento::SType stype_in,
                                           databento::SType stype_out,
                                           databento::Encoding encoding)
    -> uint64_t {
    databento::DateTimeRange<std::string> range{start_date, end_date};
    return client.MetadataGetBillableSize(dataset, symbols, schema, range,
                                         stype_in, stype_out, encoding);
  })
  .method("metadata_get_cost", [](databento::Historical& client,
                                  const std::string& dataset,
                                  const std::vector<std::string>& symbols,
                                  databento::Schema schema,
                                  const std::string& start_date,
                                  const std::string& end_date,
                                  databento::SType stype_in,
                                  databento::SType stype_out,
                                  databento::Encoding encoding)
    -> double {
    databento::DateTimeRange<std::string> range{start_date, end_date};
    return client.MetadataGetCost(dataset, symbols, schema, range,
                                 stype_in, stype_out, encoding);
  })

  // ========== SYMBOLOGY API ==========
  .method("symbology_resolve", [](databento::Historical& client,
                                  const std::string& dataset,
                                  const std::vector<std::string>& symbols,
                                  databento::SType stype_in,
                                  const std::string& date)
    -> databento::SymbologyResolution {
    return client.SymbologyResolve(dataset, symbols, stype_in, date);
  })

  // ========== BATCH API ==========
  .method("batch_submit_job", [](databento::Historical& client,
                                 const std::string& dataset,
                                 const std::vector<std::string>& symbols,
                                 databento::Schema schema,
                                 const std::string& start_date,
                                 const std::string& end_date,
                                 databento::SType stype_in,
                                 databento::SType stype_out,
                                 databento::Encoding encoding)
    -> databento::BatchJob {
    databento::DateTimeRange<std::string> range{start_date, end_date};
    return client.BatchSubmitJob(dataset, symbols, schema, range,
                               stype_in, stype_out, encoding);
  })
  .method("batch_list_jobs", [](databento::Historical& client)
    -> std::vector<databento::BatchJob> {
    return client.BatchListJobs();
  })
  .method("batch_list_jobs_with_state", [](databento::Historical& client,
                                           const std::string& state)
    -> std::vector<databento::BatchJob> {
    // Note: May need to create state enum or handle as string
    return client.BatchListJobs();  // Simplified
  })
  .method("batch_list_files", [](databento::Historical& client,
                                 const std::string& job_id)
    -> std::vector<databento::BatchFileDesc> {
    return client.BatchListFiles(job_id);
  })
  .method("batch_download_file", [](databento::Historical& client,
                                    const std::string& job_id,
                                    const std::string& filename,
                                    const std::string& output_path)
    -> std::string {
    return client.BatchDownload(job_id, filename, output_path);
  })

  // ========== TIMESERIES API ==========
  .method("timeseries_get_range_to_file", [](databento::Historical& client,
                                             const std::string& dataset,
                                             const std::vector<std::string>& symbols,
                                             databento::Schema schema,
                                             const std::string& start_date,
                                             const std::string& end_date,
                                             databento::SType stype_in,
                                             databento::SType stype_out,
                                             databento::Encoding encoding,
                                             const std::string& output_file)
    -> std::string {
    databento::DateTimeRange<std::string> range{start_date, end_date};
    return client.TimeseriesGetRangeToFile(dataset, symbols, schema, range,
                                         stype_in, stype_out, encoding,
                                         output_file);
  });
```

#### 6.4 SymbologyResolution Wrapper
**File:** `deps/databento_jl.cpp`

**Julia Wrapper Requirements:**
```cpp
// SymbologyResolution - Result of symbology resolution
mod.add_type<databento::SymbologyResolution>("SymbologyResolution")
  .method("stype_in", [](const databento::SymbologyResolution& res) {
    return res.stype_in;
  })
  .method("stype_out", [](const databento::SymbologyResolution& res) {
    return res.stype_out;
  })
  .method("mappings", [](const databento::SymbologyResolution& res)
    -> std::vector<std::pair<std::string, std::vector<std::string>>> {
    // Convert map to vector of pairs for Julia
    std::vector<std::pair<std::string, std::vector<std::string>>> result;
    for (const auto& [symbol, intervals] : res.mappings) {
      std::vector<std::string> interval_strs;
      for (const auto& interval : intervals) {
        interval_strs.push_back(interval.symbol);
      }
      result.push_back({symbol, interval_strs});
    }
    return result;
  })
  .method("partial", [](const databento::SymbologyResolution& res)
    -> std::vector<std::string> {
    std::vector<std::string> result(res.partial.begin(), res.partial.end());
    return result;
  })
  .method("not_found", [](const databento::SymbologyResolution& res)
    -> std::vector<std::string> {
    std::vector<std::string> result(res.not_found.begin(), res.not_found.end());
    return result;
  });
```

**Design Rationale:**
- Maps converted to vector of pairs (more natural for Julia)
- Sets converted to vectors
- All strings explicitly converted to std::string

### 6.5 IsBits Specializations for Phase 6

```cpp
// Phase 6: Historical/Batch API
template<> struct IsBits<databento::BatchJob> : std::true_type {};
template<> struct IsBits<databento::BatchFileDesc> : std::true_type {};
template<> struct IsBits<databento::PublisherDetail> : std::true_type {};
template<> struct IsBits<databento::FieldDetail> : std::true_type {};
template<> struct IsBits<databento::DatasetRange> : std::true_type {};
template<> struct IsBits<databento::DatasetConditionDetail> : std::true_type {};
// SymbologyResolution is non-POD (contains maps/sets)
```

### 6.6 New Includes Required

```cpp
#include <databento/batch.hpp>
#include <databento/symbology.hpp>
#include <unordered_map>   // for SymbologyResolution conversion
```

### 6.7 Julia-Side API Usage Examples

```julia
# Initialize Historical client
builder = Databento.HistoricalBuilder()
builder.set_key_from_env!()
client = builder.build()

# ========= METADATA QUERIES =========

# Get available datasets
datasets = client.metadata_list_datasets()
println("Available datasets: $datasets")

# Get dataset availability
dataset_range = client.metadata_get_dataset_range("GLBX")
println("GLBX available from $(dataset_range.start_date()) to $(dataset_range.end_date())")

# Get record count (for cost estimation)
record_count = client.metadata_get_record_count(
    "GLBX",
    ["ES", "NQ"],
    Databento.MBP1,
    "2025-01-01",
    "2025-01-31",
    Databento.RAW_SYMBOL,
    Databento.INSTRUMENT_ID
)
println("Record count: $record_count")

# Estimate cost
cost_usd = client.metadata_get_cost(
    "GLBX",
    ["ES", "NQ"],
    Databento.MBP1,
    "2025-01-01",
    "2025-01-31",
    Databento.RAW_SYMBOL,
    Databento.INSTRUMENT_ID,
    Databento.DBN
)
println("Estimated cost: \$$cost_usd")

# ========= SYMBOLOGY RESOLUTION =========

resolution = client.symbology_resolve(
    "GLBX",
    ["ES", "NQ", "UNKNOWN"],
    Databento.RAW_SYMBOL,
    "2025-01-15"
)

println("Mappings: $(resolution.mappings())")
println("Partial: $(resolution.partial())")
println("Not found: $(resolution.not_found())")

# ========= BATCH API =========

# Submit batch job
job = client.batch_submit_job(
    "GLBX",
    ["ES", "NQ"],
    Databento.MBP1,
    "2025-01-01",
    "2025-01-31",
    Databento.RAW_SYMBOL,
    Databento.INSTRUMENT_ID,
    Databento.DBN
)
println("Job submitted: $(job.id())")
println("Cost: \$$(job.cost_usd())")

# List batch jobs
jobs = client.batch_list_jobs()
for job in jobs
    println("Job $(job.id()): $(job.state())")
end

# List files in job
files = client.batch_list_files(job.id())
for file in files
    println("$(file.filename()): $(file.size()) bytes")
    println("  HTTPS: $(file.https_url())")
    println("  FTP: $(file.ftp_url())")
end

# Download file
local_path = client.batch_download_file(
    job.id(),
    files[1].filename(),
    "/tmp/market_data.dbn"
)
println("Downloaded to: $local_path")

# ========= TIMESERIES API (Direct Download) =========

output_file = client.timeseries_get_range_to_file(
    "GLBX",
    ["ES", "NQ"],
    Databento.MBP1,
    "2025-01-01",
    "2025-01-31",
    Databento.RAW_SYMBOL,
    Databento.INSTRUMENT_ID,
    Databento.DBN,
    "/tmp/direct_download.dbn"
)
println("Downloaded to: $output_file")

# Process downloaded DBN file
store = Databento.DbnFileStore(output_file)
metadata = store.get_metadata()

while true
    try
        record = store.next_record()

        # Type dispatch
        if record.holds_mbo()
            mbo = record.get_mbo_if()
            println("MBO: Price=$(mbo.price()), Size=$(mbo.size())")
        elseif record.holds_mbp1()
            mbp1 = record.get_mbp1_if()
            println("MBP1: Price=$(mbp1.price())")
        end
    catch e
        break  # End of file
    end
end
```

### 6.8 Testing Strategy

**Unit Tests (`test_phase6.jl`):**
- Builder initialization and key management
- Metadata query parsing
- Cost estimation accuracy
- Batch job submission format
- Symbol resolution result handling
- File URL generation

**Integration Tests (requires API key):**
- Real metadata queries
- Cost estimation validation
- Batch job submission and status tracking
- File list retrieval
- Download functionality
- Timeseries query execution

---

## Implementation Order

### Phase 5 Implementation Steps

1. **Add includes and IsBits** (lines 1-50)
   - `#include <databento/live*.hpp>`
   - `#include <functional>`, `<optional>`, `<chrono>`
   - LiveSubscription IsBits

2. **LiveSubscription wrapper** (lines 50-80)
   - Constructor, symbol/schema/stype setters
   - Getters for each field

3. **LiveBlocking wrapper** (lines 80-150)
   - Subscribe, start, stop, reconnect methods
   - NextRecord variants (blocking, with timeout)
   - Status queries

4. **LiveBuilder wrapper** (lines 150-200)
   - Config methods (key, dataset, heartbeat)
   - build_blocking, build_threaded factories

5. **LiveThreaded wrapper** (lines 200-250)
   - Callback registration (on_record, on_metadata)
   - Start, block_for_stop methods
   - Exception handling integration

### Phase 6 Implementation Steps

1. **Add includes** (lines 1-10)
   - `#include <databento/batch.hpp>`
   - `#include <databento/symbology.hpp>`
   - `#include <unordered_map>`

2. **Metadata structures** (lines 10-100)
   - PublisherDetail, FieldDetail
   - DatasetRange, DatasetConditionDetail
   - BatchJob, BatchFileDesc

3. **SymbologyResolution wrapper** (lines 100-150)
   - Mapping conversion (map → vector of pairs)
   - Partial/not_found vectors

4. **Enhanced Historical client** (lines 150-400)
   - Metadata API methods (10 methods)
   - Batch API methods (6 methods)
   - Symbology API (1 method)
   - Timeseries API (1 method)
   - Record count & cost estimation (3 methods)

---

## Architecture Decisions

### Design Pattern: Builder Pattern
- Used for LiveBuilder and HistoricalBuilder
- Enables fluent configuration in Julia
- Methods return `self` for chaining

### Design Pattern: Type Dispatch for Records
- Live API returns `databento::Record` type
- Julia code uses `record.holds_*()` checks
- Safe casting via `record.get_*_if()` methods

### Threading Model
- **LiveBlocking:** Synchronous (blocking calls)
  - Fits Julia's execution model
  - Caller controls async via @async, Threads.@spawn

- **LiveThreaded:** Asynchronous (event-driven)
  - Callbacks executed in C++ thread
  - Requires careful synchronization
  - Consider Julia threading restrictions

### Error Handling
- Delegate to databento-cpp exceptions
- Let C++ exceptions propagate to Julia
- Julia-side try/catch blocks

### Memory Management
- Zero-copy via `add_bits<T>()` for POD types
- Non-POD types (records, objects) are references
- Lifetime managed by CxxWrap

### String Handling
- All C++ string fields explicitly converted to `std::string`
- std::vector<std::string> for collections
- Char arrays handled with std::string conversion

---

## Performance Considerations

### Phase 5 (Live Streaming)
- **Zero-copy for records** - Via `add_bits<Record>`
- **Minimal allocation** - Live subscription reused
- **Callback overhead** - Accept for event-driven model
- **Blocking API** - Preferred for most use cases (lower complexity)

### Phase 6 (Historical/Batch)
- **Lazy API calls** - Metadata queries on-demand
- **Stream large results** - Via file downloads vs. in-memory
- **Batch processing** - Enables large dataset handling
- **Cost visibility** - Estimate before expensive operations

---

## Compatibility Notes

### Julia Version
- Minimum: Julia 1.6 (CxxWrap.jl support)
- Tested on: Julia 1.9+
- Features: Unicode strings, threading support

### CxxWrap Version
- Requires: CxxWrap.jl >= 0.14
- Uses: STL support, optional types, std::function

### databento-cpp Version
- Requires: v0.30.0+ (currently targeting v0.30.0)
- Compatibility: Enums align with library versions

---

## Future Enhancements (Phase 7+)

### Symbol Mapping
- `TsSymbolMap`, `PitSymbolMap` classes
- Symbol mapping callbacks
- Version upgrade support

### Logging
- `ILogReceiver` interface wrapper
- `ConsoleLogReceiver`, `NullLogReceiver`
- Custom log filtering in Julia

### Exception Handling
- Full exception class hierarchy
- Julia-side exception translation
- Error details accessibility

### Advanced Codecs
- DBN encoder/decoder advanced methods
- Custom encoding configurations
- Version compatibility handling

---

## Estimated Effort

### Phase 5 Implementation
- **C++ code:** 600-800 lines
- **Julia tests:** 150-200 lines
- **Documentation:** 100-150 lines
- **Total effort:** ~3-4 days (with testing)

### Phase 6 Implementation
- **C++ code:** 900-1200 lines
- **Julia tests:** 200-250 lines
- **Documentation:** 150-200 lines
- **Total effort:** ~4-5 days (with testing)

### Combined Phases 5-6
- **Total C++ wrapper:** 1900-2400 lines (vs. current 443)
- **Total estimated effort:** ~1 week
- **Result:** Complete databento client library in Julia

---

## Success Criteria

### Phase 5 Success
- ✓ Live streaming connections succeed
- ✓ Records received match expected schema
- ✓ Both blocking and threaded APIs functional
- ✓ Subscriptions can be modified
- ✓ Reconnection works gracefully

### Phase 6 Success
- ✓ All metadata queries return correct types
- ✓ Cost estimation matches API
- ✓ Batch jobs submit and track correctly
- ✓ File downloads succeed
- ✓ Symbol resolution handles partial matches

---

## References

- [CxxWrap.jl GitHub](https://github.com/JuliaInterop/CxxWrap.jl)
- [Julia C++ Integration Guide](https://juliabloggers.com/introduction-to-the-packages-cxx-jl-and-cxxwrap-jl/)
- databento-cpp v0.30.0 headers (analyzed above)
- Phase 1-4 implementation patterns

