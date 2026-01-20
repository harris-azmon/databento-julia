# Compilation Error Analysis - API Signature Mismatches

**Date:** 2026-01-20
**Status:** ✅ Type Registration FIXED → ⚠️ API Signature Mismatches REMAINING
**Branch:** `claude/phase-2-julia-wrapper-CZxKh`

---

## Fixed Issues

✅ **Type Registration (Commit 737afd6)**
- Removed `add_bits()` for complex types (FlagSet, Metadata, Record, LiveSubscription, Publisher*, Field*, Dataset*, Batch*)
- Kept `add_bits()` ONLY for true scalar types (enums and simple POD types)
- This follows the correct Phase 1 pattern

---

## Remaining Issues: API Signature Mismatches

The wrapper code is calling databento-cpp APIs with incorrect parameter types or expecting wrong return types. These need to be verified against the actual C++ headers.

### Category 1: Historical Metadata Methods Return Type Mismatch

**Error Examples:**
```
error: could not convert 'databento::Historical::MetadataListSchemas(...)'
from 'vector<databento::Schema>' to 'vector<std::__cxx11::basic_string<char>>'

error: could not convert 'databento::Historical::MetadataListFields(...)'
from 'vector<databento::FieldDetail>' to 'vector<std::__cxx11::basic_string<char>>'
```

**Root Cause:** Wrapper assumes these methods return `vector<string>`, but they actually return:
- `MetadataListSchemas` → `vector<Schema>` (enum values, not strings)
- `MetadataListFields` → `vector<FieldDetail>` (complex types, not strings)

**Lines:** 381, 384, 719, 730

**Fix:** Register FieldDetail type properly and return actual types, or create wrapper functions that convert to strings

---

### Category 2: SymbologyResolve Method Signature Mismatch

**Error Examples:**
```
error: no matching function for call to
'databento::Historical::SymbologyResolve(
  const std::string&,
  const std::vector<std::__cxx11::basic_string<char> >&,
  databento::SType&,
  const std::string&)'
```

**Root Cause:** Actual method signature differs from wrapper's assumptions

**Lines:** 391, 795, 857, 873

**Fix:** Check actual SymbologyResolve signature in databento-cpp headers

---

### Category 3: TimeseriesGetRangeToFile Missing Parameters

**Error Examples:**
```
error: no matching function for call to
'databento::Historical::TimeseriesGetRangeToFile(
  const std::string&,
  const std::vector<...>&,
  databento::Schema&,
  const std::string&,
  const std::string&,
  databento::SType&,
  databento::SType&,
  const std::string&)'
```

**Root Cause:** Wrapper is missing required parameters or has wrong parameter types

**Lines:** 402, 841, 870

**Fix:** Check actual TimeseriesGetRangeToFile signature - likely needs DateTimeRange object instead of string start/end

---

### Category 4: LiveBlocking/LiveThreaded API Mismatches

**Error Examples:**
```
error: no matching function for call to
'databento::LiveBlocking::Subscribe(const databento::LiveSubscription&)'

error: no matching function for call to
'databento::LiveThreaded::Subscribe(const databento::LiveSubscription&)'

error: no matching function for call to
'databento::LiveThreaded::Start(std::function<...>&, std::function<...>&)'
```

**Root Cause:** Live streaming API is different from what wrapper assumes

**Lines:** 520, 553, 563

**Fix:** Check actual Live API - Subscribe may not exist or may have different signature

---

### Category 5: Type Member Access Errors

**Error Examples:**
```
error: 'const struct databento::DatasetRange' has no member named 'start_date'
error: 'const struct databento::DatasetRange' has no member named 'end_date'
error: 'const struct databento::DatasetConditionDetail::condition',
which is of non-class type 'const databento::DatasetCondition'
error: request for member 'data' in 'job.databento::BatchJob::state',
which is of non-class type 'const databento::JobState'
```

**Root Cause:** Wrapper is accessing members that don't exist or are wrong types

**Lines:** 604, 607, 615, 644

**Fix:** Check actual DatasetRange, DatasetConditionDetail, BatchJob structure definitions

---

### Category 6: DbnFileStore Return Type Mismatch

**Error Example:**
```
error: could not convert '(& store)->databento::DbnFileStore::NextRecord()'
from 'const databento::Record*' to 'databento::Record'
```

**Root Cause:** NextRecord() returns pointer, wrapper expects value

**Line:** 453

**Fix:** Handle pointer return, dereference or use optional pattern

---

### Category 7: Duration Type Mismatch

**Error Example:**
```
error: cannot convert 'duration<[...],ratio<[...],1000>>'
to 'duration<[...],ratio<[...],1>''
```

**Root Cause:** Millisecond vs nanosecond duration units

**Line:** 506

**Fix:** Convert time units properly

---

## Debugging Strategy

To fix these properly, need to:

1. **Read databento-cpp headers** for actual signatures:
   - `databento/historical.hpp` - Historical client methods
   - `databento/live.hpp`, `databento/live_subscription.hpp` - Live API
   - `databento/batch.hpp` - Batch job types
   - `databento/metadata.hpp` - Metadata types

2. **Create wrapper functions** for incompatible return types:
   ```cpp
   // Convert vector<Schema> to vector<string>
   std::vector<std::string> schemas_to_strings(const std::vector<databento::Schema>& schemas) {
     // conversion logic
   }
   ```

3. **Test incrementally** - fix and compile one category at a time

---

## Critical Insight

The original Phase 1-4 implementation likely worked by chance or was never tested with CxxWrap. The API mismatches suggest the wrapper was written without careful reference to the actual C++ API signatures.

Before continuing, **verify the actual databento-cpp v0.30.0 API** by examining the headers directly.

---

## Next Steps

1. Read databento-cpp headers to get actual method signatures
2. Fix Historical metadata method wrappers to handle correct return types
3. Fix Live API method calls with correct signatures
4. Test compilation incrementally after each fix

This is the correct approach - not trying to guess API signatures, but reading the actual headers.

