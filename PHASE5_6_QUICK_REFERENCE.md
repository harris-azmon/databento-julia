# Phase 5-6 Quick Reference Guide

## Phase 5: Live Streaming API

### What to Add

#### Includes
```cpp
#include <databento/live.hpp>
#include <databento/live_blocking.hpp>
#include <databento/live_threaded.hpp>
#include <databento/live_subscription.hpp>
#include <functional>
#include <optional>
#include <chrono>
```

#### IsBits Specializations
```cpp
template<> struct IsBits<databento::LiveSubscription> : std::true_type {};
```

#### Classes to Wrap (in order)
1. **LiveSubscription** - Subscription configuration
   - `set_symbols!()`, `set_schema!()`, `set_stype!()`
   - Getters: `symbols()`, `schema()`, `stype_in()`

2. **LiveBlocking** - Blocking real-time API
   - `subscribe!()`, `start!()`, `stop!()`
   - `next_record()` - blocking
   - `next_record_timeout(ms)` - with timeout
   - `reconnect!()`, `is_subscribed()`

3. **LiveBuilder** - Client factory
   - `set_key!()`, `set_key_from_env!()`
   - `set_dataset!()`, `set_send_ts_out!()`
   - `set_heartbeat_interval!()`
   - `build_blocking()`, `build_threaded()`

4. **LiveThreaded** - Event-driven API
   - `subscribe!()`, `start!()`
   - `on_record(callback)`, `on_metadata(callback)`
   - `block_for_stop()`, `block_for_stop_timeout(ms)`

### Estimated Lines
- Phase 5 total: **600-800 lines**
- Includes + IsBits: ~20 lines
- LiveSubscription: ~25 lines
- LiveBlocking: ~120 lines
- LiveBuilder: ~60 lines
- LiveThreaded: ~80 lines
- Comments/formatting: ~100 lines

### Key Implementation Details
- Use `std::function` for callbacks (auto-converted from Julia closures)
- `std::optional<Record>` for timeout case
- Chrono durations for timeouts
- Return `self` for method chaining
- All methods end with `!` for mutations

---

## Phase 6: Historical/Batch API

### What to Add

#### Includes
```cpp
#include <databento/batch.hpp>
#include <databento/symbology.hpp>
#include <unordered_map>
```

#### IsBits Specializations
```cpp
template<> struct IsBits<databento::BatchJob> : std::true_type {};
template<> struct IsBits<databento::BatchFileDesc> : std::true_type {};
template<> struct IsBits<databento::PublisherDetail> : std::true_type {};
template<> struct IsBits<databento::FieldDetail> : std::true_type {};
template<> struct IsBits<databento::DatasetRange> : std::true_type {};
template<> struct IsBits<databento::DatasetConditionDetail> : std::true_type {};
// SymbologyResolution is non-POD (contains maps)
```

#### Structures to Wrap (in order)
1. **PublisherDetail** (4 methods)
   - `publisher_id()`, `dataset()`, `venue()`, `description()`

2. **FieldDetail** (2 methods)
   - `name()`, `type()`

3. **DatasetRange** (2 methods)
   - `start_date()`, `end_date()`

4. **DatasetConditionDetail** (3 methods)
   - `date()`, `condition()`, `last_modified_date()`

5. **BatchFileDesc** (5 methods)
   - `filename()`, `size()`, `hash()`, `https_url()`, `ftp_url()`

6. **BatchJob** (20+ accessors)
   - IDs: `id()`, `user_id()`, `bill_id()`
   - Metadata: `dataset()`, `state()`, `schema()`, `encoding()`, etc.
   - Timestamps: `ts_received()`, `ts_queued()`, `ts_process_start()`, etc.
   - Sizes: `record_count()`, `billed_size()`, `actual_size()`, `package_size()`
   - `symbols()` - returns vector

7. **SymbologyResolution** (3 methods)
   - `mappings()` - map converted to vector<pair<string, vector<string>>>
   - `partial()` - vector of symbols
   - `not_found()` - vector of symbols

#### Enhanced Historical Client (Phase 3 → Phase 6)

Replace the Phase 3 Historical section with **30+ methods** organized by category:

**Metadata API (10 methods)**
- `metadata_list_publishers()` → vector
- `metadata_list_datasets()` → vector
- `metadata_list_datasets_with_range(start, end)` → vector
- `metadata_list_schemas(dataset)` → vector
- `metadata_list_fields(encoding, schema)` → vector
- `metadata_list_unit_prices(dataset)` → vector
- `metadata_get_dataset_range(dataset)` → DatasetRange
- `metadata_get_dataset_condition(dataset, start, end)` → vector

**Record Count & Billing (3 methods)**
- `metadata_get_record_count(dataset, symbols, schema, start, end, stype_in, stype_out)` → uint64
- `metadata_get_billable_size(dataset, symbols, schema, start, end, stype_in, stype_out, encoding)` → uint64
- `metadata_get_cost(dataset, symbols, schema, start, end, stype_in, stype_out, encoding)` → double

**Symbology (1 method)**
- `symbology_resolve(dataset, symbols, stype_in, date)` → SymbologyResolution

**Batch API (4 methods)**
- `batch_submit_job(dataset, symbols, schema, start, end, stype_in, stype_out, encoding)` → BatchJob
- `batch_list_jobs()` → vector<BatchJob>
- `batch_list_files(job_id)` → vector<BatchFileDesc>
- `batch_download_file(job_id, filename, output_path)` → string (local path)

**Timeseries (1 method)**
- `timeseries_get_range_to_file(dataset, symbols, schema, start, end, stype_in, stype_out, encoding, output_file)` → string

### Estimated Lines
- Phase 6 total: **900-1200 lines**
- Includes + IsBits: ~20 lines
- PublisherDetail: ~15 lines
- FieldDetail: ~10 lines
- DatasetRange: ~10 lines
- DatasetConditionDetail: ~15 lines
- BatchFileDesc: ~25 lines
- BatchJob: ~60 lines
- SymbologyResolution: ~40 lines
- Enhanced Historical client: ~600 lines (30+ methods)
- Comments/formatting: ~150 lines

### Key Implementation Details
- Use `DateTimeRange<std::string>` for date ranges
- Convert maps/sets to vectors for Julia
- All strings explicitly converted to `std::string`
- Timestamps returned as `UnixNanos`
- API methods organized by functionality (metadata, batch, etc.)

---

## Combined Statistics (Phases 1-6)

| Phase | Component | Lines | Types | Methods |
|-------|-----------|-------|-------|---------|
| 1 | Core Enums | 41 | 3 | 26 |
| 2 | Data Records | 241 | 14 | 155+ |
| 3 | Historical Client | 45 | 2 | 5 |
| 4 | DBN File Reader | 52 | 3 | 20 |
| 5 | Live Streaming | 600-800 | 4 | 30+ |
| 6 | Historical Batch | 900-1200 | 7 | 30+ |
| **Total** | **Complete Client** | **1900-2400** | **33** | **260+** |

---

## Testing Checklist

### Phase 5 Tests
- [ ] LiveBuilder initialization
- [ ] LiveSubscription configuration
- [ ] LiveBlocking connection
- [ ] Record streaming
- [ ] Timeout handling
- [ ] Reconnection logic
- [ ] LiveThreaded callbacks
- [ ] Subscription modification

### Phase 6 Tests
- [ ] Builder initialization
- [ ] Metadata queries
- [ ] Cost estimation
- [ ] Record counting
- [ ] Batch job submission
- [ ] Job status tracking
- [ ] File listing
- [ ] Download functionality
- [ ] Symbol resolution
- [ ] Timeseries queries

---

## File Structure After Phase 6

```cpp
// databento_jl.cpp (2000-2400 lines)

// PHASE 1: Core Enums (lines 59-100)
Schema, Encoding, SType enums + 26 constants

// PHASE 2: Data Record Protocol (lines 101-342)
RType, Action, Side enums + 32 constants
FlagSet, UnixNanos, TimeDeltaNanos, BidAskPair types
RecordHeader, MboMsg, TradeMsg, Mbp1Msg, Mbp10Msg, InstrumentDefMsg, ImbalanceMsg
+ 155 methods

// PHASE 3: Historical Client (lines 344-389)
HistoricalBuilder, Historical
+ 5 methods

// PHASE 4: DBN File Reader (lines 391-443)
Metadata, Record, DbnFileStore
+ 20 methods

// PHASE 5: Live Streaming (lines 444-1244)
LiveSubscription, LiveBlocking, LiveBuilder, LiveThreaded
+ 30+ methods

// PHASE 6: Historical/Batch API (lines 1245-2443)
PublisherDetail, FieldDetail, DatasetRange, DatasetConditionDetail,
BatchFileDesc, BatchJob, SymbologyResolution
Enhanced Historical with 30+ methods
+ 30+ methods

// IsBits Specializations: 34 total (28 → 34 after Phase 6)
```

---

## Code Generation Strategy

### Phase 5 Implementation
1. Copy Phase 3 Historical section as starting point
2. Add new includes (live*.hpp, functional, optional, chrono)
3. Add LiveSubscription IsBits and wrapper
4. Add LiveBlocking wrapper with overloads
5. Add LiveBuilder wrapper with build methods
6. Add LiveThreaded wrapper (most complex)

### Phase 6 Implementation
1. Add new includes (batch.hpp, symbology.hpp)
2. Add data structure IsBits + wrappers (PublisherDetail, etc.)
3. Replace Phase 3 Historical section with extended version
4. Add ~25 new Historical methods, organized by API category
5. Add SymbologyResolution wrapper (complex map conversion)

---

## Common Pitfalls to Avoid

1. **Forget method chaining** - Remember `!` suffix and return `self` for mutations
2. **String conversion** - Always convert char arrays and std::string explicitly
3. **Date ranges** - Use `DateTimeRange<std::string>` not raw strings
4. **Map conversion** - Convert unordered_map to vector<pair> for Julia
5. **Optional handling** - Use `std::optional` and check properly
6. **Const references** - Use `const T&` for getters to avoid copying
7. **Thread safety** - LiveThreaded callbacks need careful synchronization
8. **Error handling** - Let C++ exceptions propagate to Julia

---

## References

### Key Headers
- `databento/live.hpp` - LiveBuilder
- `databento/live_blocking.hpp` - LiveBlocking
- `databento/live_threaded.hpp` - LiveThreaded
- `databento/live_subscription.hpp` - LiveSubscription
- `databento/historical.hpp` - Extended Historical API
- `databento/batch.hpp` - Batch structures
- `databento/symbology.hpp` - SymbologyResolution

### Julia/CxxWrap Patterns
- Use `add_bits<T>()` for POD types (zero-copy)
- Use `add_type<T>()` for classes/objects
- Use `std::function` for callbacks (auto-converted)
- Use `std::optional<T>` for nullable returns
- Use `std::vector<T>` for collections
- Strings: Always convert to `std::string` explicitly

### Related Documentation
- [CxxWrap.jl](https://github.com/JuliaInterop/CxxWrap.jl) - Type wrapping framework
- [Phase 1-4 Implementation](./COMPREHENSIVE_PHASE_REPORT.md) - Existing patterns
- [Phase 5-6 Full Plan](./PHASE5_6_IMPLEMENTATION_PLAN.md) - Detailed specification

