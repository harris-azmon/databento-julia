# Phase 6: Tables.jl Integration Plan

## Objective
Enable seamless integration with the Julia data science ecosystem by making `DbnFileStore` and Databento record streams compatible with the `Tables.jl` interface. This will allow users to perform operations like:
```julia
using DataFrames, Databento
df = DataFrame(DbnFileStore("data.dbn"))
```

## Technical Approach

### 1. Iteration Interface
To support tabular access, we first need to make `DbnFileStore` an iterable that yields records.
- Implement `Base.iterate(store::DbnFileStore)`
- Implement `Base.eltype(::Type{DbnFileStore})`
- Ensure the iterator handles the underlying C++ pointer state correctly.

### 2. Tables.jl Interface Implementation
- **Table traits**:
  - `Tables.istable(::Type{<:DbnFileStore}) = true`
  - `Tables.rowaccess(::Type{<:DbnFileStore}) = true`
  - `Tables.rows(store::DbnFileStore) = store`
- **Row-level interface**:
  - Each `Record` yielded by the iterator must act as a `Tables.AbstractRow`.
  - Implement `Tables.columnnames(record::Record)` based on its `RType`.
  - Implement `Tables.getcolumn(record::Record, nm::Symbol)` by dispatching to the appropriate C++ message getters (`price()`, `size()`, etc.).

### 3. Dynamic Schema Handling
DBN files are heterogeneous but usually contain a primary schema (e.g., all `TradeMsg`).
- Use the `Metadata` from `DbnFileStore` to provide a consistent `Tables.Schema`.
- Handle cases where `has_mixed_schema` is true by providing a union or fallback schema.

## Implementation Steps

### Step 1: Dependencies
- Add `Tables.jl` to `Project.toml` under `[deps]`.
- Add `DataFrames.jl` to `[extras]` for testing.

### Step 2: Iterator Support
- Modify `src/Databento.jl` to include `Base.iterate`.
- Ensure it calls `next_record(store)` and handles the `NULL` termination.

### Step 3: Column Mapping
- Create a mapping between Julia Symbols and C++ field accessors for:
  - `MboMsg`
  - `TradeMsg`
  - `Mbp1Msg` / `Mbp10Msg`
  - `InstrumentDefMsg`
  - `ImbalanceMsg`

### Step 4: Tables Interface
- Implement `Tables.AbstractRow` for a wrapper type (e.g., `DatabentoRow`) or directly for `Record` if possible.
- Implement `Tables.Schema` to allow efficient `DataFrame` construction.

### Step 5: Verification
- Add `test/test_tables.jl`.
- Verify that `DataFrame(store)` correctly populates columns with the expected types and values.
- Check performance to ensure zero-copy principles are maintained where possible.

## Future Considerations
- **Columnar Access**: Investigate `Tables.columnaccess` for even faster `DataFrame` creation if we can batch read records.
- **Lazy Loading**: Ensure that we don't load the entire DBN file into memory unless explicitly requested.
