# databento-cpp v0.30.0 Complete API Inventory

**Analysis Date:** 2026-01-20
**Library Version:** v0.30.0
**Header Location:** `/home/user/databento-julia/deps/include/databento/`
**Scope:** Comprehensive inventory of all public APIs for future wrapper phases

---

## Summary Statistics

| Category | Count | Wrapped | Phase |
|----------|-------|---------|-------|
| Enum Types | 25+ | 6 | 1 |
| Record Types | 16+ | 13 | 2-4 |
| API Classes | 8 | 5 | 1-4 |
| Data Structures | 20+ | 12 | 2-4 |
| Utility Classes | 15+ | 2 | 1 |
| Exception Types | 8 | 0 | — |
| Total Public Types | 90+ | 38 | 1-4 |

---

## PART 1: API Classes (Primary Classes)

### 1.1 LiveBuilder
**Header:** `live.hpp`
**Status:** ⚠️ PHASE 5 (Planned)

```cpp
class LiveBuilder {
public:
  LiveBuilder& SetKeyFromEnv(const std::string& env_name = "DATABENTO_API_KEY");
  LiveBuilder& SetKey(const std::string& key);
  LiveBuilder& SetDataset(const std::string& dataset);
  LiveBuilder& SetSendTsOut(bool send);
  LiveBuilder& SetUpgradePolicy(VersionUpgradePolicy policy);
  LiveBuilder& SetLogReceiver(const std::shared_ptr<ILogReceiver>& receiver);
  LiveBuilder& SetHeartbeatInterval(std::chrono::milliseconds interval);
  LiveBuilder& SetAddress(const std::string& address, uint16_t port);

  // Factory methods
  LiveBlocking BuildBlocking();
  LiveThreaded BuildThreaded();
};
```

**Methods:** 10
**Estimated Julia Lines:** 60-80

---

### 1.2 LiveBlocking
**Header:** `live_blocking.hpp`
**Status:** ⚠️ PHASE 5 (Planned)

```cpp
class LiveBlocking {
public:
  void Subscribe(const LiveSubscription& subscription);
  void SubscribeWithSnapshot(const LiveSubscription& subscription,
                            const std::string& start_date);

  void Start();

  // Blocking methods
  Record NextRecord();
  std::optional<Record> NextRecord(std::chrono::milliseconds timeout);

  void Stop();
  void Reconnect();
  void Resubscribe();

  // Accessors
  const std::string& Key() const;
  const std::string& Dataset() const;
  const std::vector<LiveSubscription>& Subscriptions() const;
  // ... more getters
};
```

**Methods:** 12+
**Estimated Julia Lines:** 100-150

---

### 1.3 LiveThreaded
**Header:** `live_threaded.hpp`
**Status:** ⚠️ PHASE 5 (Planned)

```cpp
class LiveThreaded {
public:
  using ExceptionCallback = std::function<ExceptionAction(std::exception_ptr)>;
  using RecordCallback = std::function<KeepGoing(const Record&)>;
  using MetadataCallback = std::function<void(Metadata&&)>;

  void Subscribe(const LiveSubscription& subscription);
  void SubscribeWithSnapshot(const LiveSubscription& subscription,
                             const std::string& start_date);

  void Start(const RecordCallback& record_cb,
            const MetadataCallback& metadata_cb = nullptr,
            const ExceptionCallback& exception_cb = nullptr);

  void Reconnect();
  void Resubscribe();

  // Blocking methods
  void BlockForStop();
  bool BlockForStop(std::chrono::milliseconds timeout);

  // Accessors
  const std::vector<LiveSubscription>& Subscriptions() const;
  // ... more getters
};
```

**Methods:** 10+
**Estimated Julia Lines:** 80-120
**Complexity:** HIGH (callback handling)

---

### 1.4 Historical
**Header:** `historical.hpp`
**Status:** ✅ PHASE 3 (3 methods wrapped), ⚠️ PHASE 6 (expansion planned)

```cpp
class Historical {
public:
  // ===== METADATA API =====
  std::vector<PublisherDetail> MetadataListPublishers();
  std::vector<std::string> MetadataListDatasets();
  std::vector<std::string> MetadataListDatasets(const DateRange& date_range);
  std::vector<std::string> MetadataListSchemas(const std::string& dataset);
  std::vector<FieldDetail> MetadataListFields(Encoding encoding, Schema schema);
  std::vector<std::string> MetadataListUnitPrices(const std::string& dataset);

  DatasetRange MetadataGetDatasetRange(const std::string& dataset);
  std::vector<DatasetConditionDetail> MetadataGetDatasetCondition(
    const std::string& dataset,
    const DateRange& date_range = {});

  // ===== RECORD COUNT API =====
  template<class T>
  uint64_t MetadataGetRecordCount(
    const std::string& dataset,
    const std::vector<std::string>& symbols,
    Schema schema,
    const DateTimeRange<T>& date_time_range,
    SType stype_in,
    SType stype_out);

  // ===== BILLING API =====
  template<class T>
  uint64_t MetadataGetBillableSize(
    const std::string& dataset,
    const std::vector<std::string>& symbols,
    Schema schema,
    const DateTimeRange<T>& date_time_range,
    SType stype_in,
    SType stype_out,
    Encoding encoding);

  template<class T>
  double MetadataGetCost(
    const std::string& dataset,
    const std::vector<std::string>& symbols,
    Schema schema,
    const DateTimeRange<T>& date_time_range,
    SType stype_in,
    SType stype_out,
    Encoding encoding,
    FeedMode feed_mode = FeedMode::OTC);

  // ===== SYMBOLOGY API =====
  SymbologyResolution SymbologyResolve(
    const std::string& dataset,
    const std::vector<std::string>& symbols,
    SType stype_in,
    const std::string& date);

  // ===== BATCH API =====
  BatchJob BatchSubmitJob(const BatchJobRequest& request);
  std::vector<BatchJob> BatchListJobs(
    const DateRange& date_range = {},
    const std::string& state = "");
  std::vector<BatchFileDesc> BatchListFiles(const std::string& job_id);
  std::string BatchDownload(const std::string& job_id,
                           const std::string& filename,
                           const std::string& output_path);

  // ===== TIMESERIES API =====
  template<class T>
  void TimeseriesGetRange(
    const std::string& dataset,
    const std::vector<std::string>& symbols,
    Schema schema,
    const DateTimeRange<T>& date_time_range,
    SType stype_in,
    SType stype_out,
    Encoding encoding,
    const RecordCallback& callback);

  template<class T>
  std::string TimeseriesGetRangeToFile(
    const std::string& dataset,
    const std::vector<std::string>& symbols,
    Schema schema,
    const DateTimeRange<T>& date_time_range,
    SType stype_in,
    SType stype_out,
    Encoding encoding,
    const std::string& output_file);
};
```

**Methods:** 30+
**Already Wrapped:** 3 methods
**Planned Phase 6:** 27 additional methods
**Estimated Julia Lines (total):** 600-800

---

### 1.5 DbnFileStore
**Header:** `dbn_file_store.hpp`
**Status:** ✅ PHASE 4 (fully wrapped)

```cpp
class DbnFileStore : public IReadable {
public:
  explicit DbnFileStore(const std::string& path);

  const Metadata& GetMetadata() const;
  Record NextRecord();

  template<typename RecordCallbackT>
  void Replay(const RecordCallbackT& callback);
};
```

**Methods:** 3 (+ template Replay)
**Status:** ✅ Complete

---

### 1.6 HistoricalBuilder
**Header:** `historical.hpp`
**Status:** ✅ PHASE 3 (fully wrapped)

```cpp
class HistoricalBuilder {
public:
  HistoricalBuilder& SetKeyFromEnv(const std::string& env_name = "DATABENTO_API_KEY");
  HistoricalBuilder& SetKey(const std::string& key);
  HistoricalBuilder& SetGateway(HistoricalGateway gateway);
  HistoricalBuilder& SetLogReceiver(const std::shared_ptr<ILogReceiver>& receiver);

  Historical Build();
};
```

**Methods:** 4
**Status:** ✅ Complete

---

## PART 2: Enums (Value Types)

### 2.1 Schema
**Header:** `enums.hpp`
**Status:** ✅ PHASE 1

Constants (17 total):
```
Mbo, Mbp1, Mbp10, Tbbo, Trades
Ohlcv1S, Ohlcv1M, Ohlcv1H, Ohlcv1D
Definition, Statistics, Status, Imbalance
Cmbp1, Cbbo1S, Cbbo1M, Tcbbo, Bbo1S, Bbo1M
```

---

### 2.2 Encoding
**Header:** `enums.hpp`
**Status:** ✅ PHASE 1

Constants (3 total):
```
Dbn, Csv, Json
```

---

### 2.3 SType
**Header:** `enums.hpp`
**Status:** ✅ PHASE 1

Constants (6 total):
```
InstrumentId, RawSymbol, Continuous, Parent, NasdaqSymbol, CmsSymbol
```

---

### 2.4 RType
**Header:** `enums.hpp`
**Status:** ✅ PHASE 2

Constants (22 total):
```
Mbp0, Mbp1, Mbp10
OhlcvDeprecated, Ohlcv1S, Ohlcv1M, Ohlcv1H, Ohlcv1D
Status, InstrumentDef, Imbalance, Error, SymbolMapping, System, Statistics
Mbo, Cmbp1, Cbbo1S, Cbbo1M, Tcbbo, Bbo1S, Bbo1M
```

---

### 2.5 Action
**Header:** `enums.hpp`
**Status:** ✅ PHASE 2

Constants (7 total):
```
Modify, Trade, Fill, Cancel, Add, Clear, None
```

---

### 2.6 Side
**Header:** `enums.hpp`
**Status:** ✅ PHASE 2

Constants (3 total):
```
Ask, Bid, None
```

---

### 2.7 Additional Enums (Not Yet Wrapped)
**Status:** ⚠️ Future phases

- **Compression** - DBN encoding compression (3 values: None, Zstd)
- **SplitDuration** - Batch job split duration (4 values)
- **Delivery** - Delivery method for batch (3 values)
- **JobState** - Batch job states (5 values: Submitted, Accepted, Queued, Processing, Done)
- **DatasetCondition** - Dataset quality status (4 values)
- **Venue** - Exchange venue enum
- **Dataset** - Dataset identifier enum
- **Publisher** - Publisher identifier enum
- **InstrumentClass** - Instrument classification
- **MatchAlgorithm** - Order matching algorithm
- **SecurityUpdateAction** - Security update type
- **UserDefinedInstrument** - Instrument definition mode
- **StatType** - Statistics type
- **StatUpdateAction** - Stats update action
- **StatusAction** - Status action type
- **StatusReason** - Status reason codes
- **TradingEvent** - Trading event enum
- **TriState** - Three-valued logic enum
- **VersionUpgradePolicy** - DBN version upgrade strategy
- **LogLevel** - Logging severity (Debug, Info, Warning, Error)
- **FeedMode** - Feed mode for pricing (OTC, Intraday, etc.)
- **HistoricalGateway** - Historical API gateway endpoint

---

## PART 3: Record Types

### 3.1 Phase 2 Records (✅ Wrapped)
```
RecordHeader, MboMsg, TradeMsg, Mbp1Msg, Mbp10Msg, InstrumentDefMsg, ImbalanceMsg
```

### 3.2 Phase 4 Additional Records (✅ Wrapped)
```
StatusMsg, OhlcvMsg, StatMsg, ErrorMsg, SymbolMappingMsg, SystemMsg
BboMsg, Cmbp1Msg, CbboMsg
```

### 3.3 Record Variants (Available but not explicitly wrapped)
```
v1::InstrumentDefMsg - Legacy version
v2::InstrumentDefMsg - Current standard (all v2 types)
v3::InstrumentDefMsg - Extended version with additional fields
```

**Total Record Types:** 16+ across all versions

---

## PART 4: Data Structures

### 4.1 Supporting Types (✅ Phase 2 Wrapped)
```
FlagSet           - Bit flags for records
UnixNanos         - Timestamp in nanoseconds
TimeDeltaNanos    - Time delta in nanoseconds
BidAskPair        - Bid/Ask price level data
```

### 4.2 Live Subscription (⚠️ Phase 5 Planned)
```cpp
struct LiveSubscription {
  std::vector<std::string> symbols;
  Schema schema;
  SType stype_in;

  variant<Snapshot, UnixNanos, std::string, NoStart> start;
};
```

### 4.3 Batch & Metadata Structures (⚠️ Phase 6 Planned)
```
BatchJob          - Batch job metadata (20+ fields)
BatchFileDesc     - File metadata within batch (5 fields)
PublisherDetail   - Publisher information (4 fields)
FieldDetail       - Field metadata (2 fields)
DatasetRange      - Date range availability (2 fields)
DatasetConditionDetail - Quality metadata (3 fields)
SymbologyResolution - Symbol mapping results (maps + vectors)
```

### 4.4 File Reader (✅ Phase 4 Wrapped)
```
Metadata          - DBN file metadata
Record            - Universal record wrapper
```

### 4.5 Symbol Mapping (⚠️ Future Phase 7)
```cpp
struct MappingInterval {
  std::string start_date;
  std::string end_date;
  std::string symbol;
};

struct SymbolMapping {
  std::string raw_symbol;
  std::vector<MappingInterval> intervals;
};

class TsSymbolMap { /* ... */ };
class PitSymbolMap { /* ... */ };
```

---

## PART 5: Exception Classes

### 5.1 Exception Hierarchy
**Status:** ⚠️ Not yet wrapped (for Phase 7+)

```cpp
class Exception : public std::exception {
public:
  const char* what() const noexcept override;
  const std::string& Message() const;
};

class HttpRequestError : public Exception {
  const std::string& RequestPath() const;
  uint32_t ErrorCode() const;
};

class HttpResponseError : public Exception {
  const std::string& RequestPath() const;
  uint16_t StatusCode() const;
  const std::string& ResponseBody() const;
};

class TcpError : public Exception {
  int ErrNum() const;
};

class InvalidArgumentError : public Exception {
  const std::string& MethodName() const;
  const std::string& ArgumentName() const;
  const std::string& Details() const;
};

class JsonResponseError : public Exception {
  static std::unique_ptr<JsonResponseError> ParseError(...);
  static std::unique_ptr<JsonResponseError> MissingKey(...);
  static std::unique_ptr<JsonResponseError> TypeMismatch(...);
};

class DbnResponseError : public Exception { /* DBN codec errors */ };
class LiveApiError : public Exception { /* Live API errors */ };
```

**Methods:** 15+
**Status:** Currently propagate as C++ exceptions

---

## PART 6: Utility Classes & Interfaces

### 6.1 I/O Interfaces
**Status:** ⚠️ Infrastructure only

```cpp
class IReadable {
public:
  virtual std::size_t ReadExact(char* buffer, std::size_t size) = 0;
  virtual std::size_t ReadSome(char* buffer, std::size_t max_size) = 0;
};

class IWritable {
public:
  virtual void WriteAll(const char* buffer, std::size_t size) = 0;
};

class InFileStream : public IReadable { /* ... */ };
class OutFileStream : public IWritable { /* ... */ };
```

### 6.2 Logging Interface
**Status:** ⚠️ Future Phase 7

```cpp
enum class LogLevel { Debug, Info, Warning, Error };

class ILogReceiver {
public:
  virtual void Receive(LogLevel level, const std::string& message) = 0;
  static std::shared_ptr<ILogReceiver> Default();
};

class NullLogReceiver : public ILogReceiver { /* No-op */ };
class ConsoleLogReceiver : public ILogReceiver { /* stdout */ };
```

### 6.3 DateTime Utilities
**Status:** ⚠️ Partially wrapped (UnixNanos, TimeDeltaNanos)

```cpp
template<typename T>
class DateTimeRange {
public:
  T start;
  T end;
};

using DateRange = DateTimeRange<std::string>;

// Functions
std::string ToString(UnixNanos nanos);
std::string ToIso8601(UnixNanos nanos);
uint32_t DateFromIso8601Int(const std::string& date_str);
```

### 6.4 Price Utilities
**Status:** ⚠️ Not wrapped

```cpp
struct FixPx {
  static constexpr int64_t kUndefined = std::numeric_limits<int64_t>::min();

  int64_t mantissa;
  int8_t exponent;

  bool IsUndefined() const;
  std::string operator<<(std::ostream& os, const FixPx& px);
  std::string PxToString(int64_t px);
};
```

### 6.5 FlagSet Utilities
**Status:** ✅ PHASE 2 (Partially wrapped)

```cpp
class FlagSet {
public:
  static constexpr uint8_t kLast = 0x01;
  static constexpr uint8_t kTob = 0x02;
  static constexpr uint8_t kSnapshot = 0x04;
  static constexpr uint8_t kMbp = 0x08;
  static constexpr uint8_t kBadTsRecv = 0x10;
  static constexpr uint8_t kMaybeBadBook = 0x20;

  bool IsLast() const;
  bool IsTob() const;
  bool IsSnapshot() const;
  bool IsMbp() const;
  bool IsBadTsRecv() const;
  bool IsMaybeBadBook() const;

  void SetLast();
  void SetTob();
  // ... more setters

  void Clear();
  bool Any() const;
  bool IsEmpty() const;
  uint8_t Raw() const;
  void SetRaw(uint8_t value);
};
```

**Wrapped Methods:** 8 (getters only)
**Unwrapped Methods:** 6 (setters)

---

## PART 7: Codec Classes

### 7.1 DbnDecoder
**Status:** ⚠️ Infrastructure

```cpp
class DbnDecoder {
public:
  static std::pair<uint32_t, std::size_t> DecodeMetadataVersionAndSize(
    const char* buffer,
    std::size_t buffer_size);

  static Metadata DecodeMetadataFields(
    uint32_t version,
    const char* buffer);

  // Version compatibility methods
  static Record DecodeRecordCompat(...);
};
```

### 7.2 DbnEncoder
**Status:** ⚠️ Infrastructure

```cpp
class DbnEncoder {
public:
  template<typename R>
  static std::size_t EncodeRecord(const R& record, char* buffer);

  template<typename R, typename T>
  static std::size_t WithTsOut(const R& record,
                               T ts_out_val,
                               char* buffer);
};
```

---

## PART 8: Version Handling

### 8.1 V1 Records
**Status:** ⚠️ Version compatibility (Phase 8+)

```cpp
namespace v1 {
  struct InstrumentDefMsg { /* V1 layout */ };

  Record ToV2(const v1::InstrumentDefMsg& v1);
  Record ToV3(const v1::InstrumentDefMsg& v1);
}
```

### 8.2 V2 Records (Current Standard)
**Status:** ✅ All wrapped in Phase 2-4

### 8.3 V3 Records
**Status:** ⚠️ Version compatibility (Phase 8+)

```cpp
namespace v3 {
  struct InstrumentDefMsg {
    // All v2 fields +
    char leg_raw_symbol[20];
    int64_t leg_price;
    int32_t leg_delta;
    uint64_t raw_instrument_id;
  };
}
```

---

## PART 9: Callback & Type Traits

### 9.1 Enums
```cpp
enum class KeepGoing { Continue, Stop };
enum class ExceptionAction { Restart, Stop };
enum class TriState { Yes, No, Unknown };
```

### 9.2 Type Traits
```cpp
namespace jlcxx {
  template<typename T>
  struct IsBits { /* ... */ };
}
```

### 9.3 Callbacks
```cpp
using RecordCallback = std::function<KeepGoing(const Record&)>;
using MetadataCallback = std::function<void(Metadata&&)>;
using ExceptionCallback = std::function<ExceptionAction(std::exception_ptr)>;
```

---

## PART 10: Conversion Functions

### 10.1 Enum Conversions
**Status:** ⚠️ Not wrapped

```cpp
template<typename E>
std::string ToString(E enum_value);

template<typename E>
E FromString(const std::string& string_value);

template<typename E>
std::ostream& operator<<(std::ostream& os, E enum_value);
```

Available for all 25+ enum types.

### 10.2 Utility Functions
```cpp
// Symbology
std::string JoinSymbolStrings(const std::vector<std::string>& symbols);

// Price utilities
std::string PxToString(int64_t px);
FixPx::IsUndefined(int64_t px);

// Symbol mapping
SymbolMapping::CreateSymbolMapForDate(const std::string& date);
SymbolMapping::CreateSymbolMap();
SymbolMapping::Upgrade(VersionUpgradePolicy policy);

// Publisher info
Venue PublisherVenue(uint16_t publisher_id);
std::string PublisherDataset(uint16_t publisher_id);

// Gateway URLs
std::string UrlFromGateway(HistoricalGateway gateway);
```

**Count:** 20+
**Wrapped:** 0

---

## Implementation Roadmap Summary

### ✅ Completed (Phases 1-4)
- **38 types** wrapped
- **~443 lines** of C++ code
- **6 enums** (58 constants)
- **13 record types** (200+ methods)
- **3 client classes** (8 methods)
- **2 file I/O classes** (5 methods)

### ⚠️ Planned (Phases 5-6)
- **Phase 5:** 4 live streaming classes (600-800 lines)
- **Phase 6:** 7 batch/metadata structures + 25 client methods (900-1200 lines)
- **Total phases 5-6:** 52 new types, 1500-2000 new lines

### 🔮 Future (Phases 7-8+)
- **Phase 7:** Symbol mapping, logging, exception handling (500-700 lines)
- **Phase 8:** Version handling, codec utilities, advanced features (400-600 lines)
- **Total phases 7+:** 20+ more types, 1000-1500 more lines

### Final State (All Phases)
- **~90 types** wrapped
- **~2500-3000 lines** of C++ wrapper code
- **350+ methods** exposed to Julia
- **Complete databento client library** for Julia

---

## Key Implementation Insights

### Type Categories
1. **POD Types** (28) - Use `add_bits<T>()` for zero-copy
2. **Classes/Objects** (12) - Use `add_type<T>()` with reference semantics
3. **Enums** (25) - Use `add_bits<T>()` with `set_const()` for values

### Method Patterns
1. **Builders** - Return `self` for chaining
2. **Getters** - Return by value, accept const ref
3. **Mutations** - Suffix with `!`, return self
4. **Factories** - Return constructed objects
5. **Callbacks** - Use `std::function<>` for conversion

### Data Conversion Strategy
1. **Strings:** Always explicit `std::string()` conversion
2. **Collections:** `std::vector<T>` maps to Julia `Vector{T}`
3. **Maps:** Convert to `Vector{Tuple{K,V}}`
4. **Timestamps:** Keep as `UnixNanos` (auto-converts)
5. **Enums:** Direct mapping via constants

---

## References

- databento-cpp v0.30.0 headers: `/home/user/databento-julia/deps/include/databento/`
- Phase 1-4 implementation: `/home/user/databento-julia/deps/databento_jl.cpp`
- Phase 1-4 reports: `COMPREHENSIVE_PHASE_REPORT.md`
- Phase 5-6 detailed plan: `PHASE5_6_IMPLEMENTATION_PLAN.md`
- Phase 5-6 quick reference: `PHASE5_6_QUICK_REFERENCE.md`
- Phase 5-6 architecture: `PHASE5_6_ARCHITECTURE.md`

