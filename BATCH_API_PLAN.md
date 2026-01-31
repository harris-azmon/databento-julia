# Batch API Implementation Plan

This plan outlines the steps required to achieve 100% support for the Databento Batch API in `Databento.jl`.

## Objectives
- Support submission of batch download jobs.
- List existing batch jobs and their statuses.
- List and download files associated with completed batch jobs.
- Provide full tabular access to batch job metadata.

## Step 1: Batch Enums
Map the remaining enums required by the Batch API in `deps/databento_jl.cpp` and `src/Databento.jl`.
- [ ] `Compression` (None, Zstd)
- [ ] `SplitDuration` (Day, Week, Month, None)
- [ ] `Delivery` (Download, S3, Disk)
- [ ] `JobState` (Received, Queued, Processing, Done, Expired)
- [ ] Add `to_string_*` methods and Julia `show` methods for each.

## Step 2: Batch Structures
Map the core batch data structures.
- [ ] `BatchJob`: Map all fields including ID, cost, state, and timestamps.
- [ ] `BatchFileDesc`: Map filename, size, hash, and URLs.
- [ ] Disable mirroring for these structs to allow custom methods/getters.
- [ ] Implement `to_string` for these structs in C++.

## Step 3: Vector Support
Register the `std::vector` templates required for Batch method returns.
- [ ] `std::vector<databento::BatchJob>`
- [ ] `std::vector<databento::BatchFileDesc>`
- [ ] `std::vector<databento::JobState>` (for filtering `BatchListJobs`)

## Step 4: Historical Client Methods
Expose the Batch API methods on the `Historical` client in `deps/databento_jl.cpp`.
- [ ] `BatchSubmitJob`: Implement at least the simple string-range and full-parameter overloads.
- [ ] `BatchListJobs`: Implement overloads for listing all and filtered by state/time.
- [ ] `BatchListFiles`: Get descriptions of files for a specific job ID.
- [ ] `BatchDownload`: Support downloading all files or a specific file to a directory.

## Step 5: Julia Interface & Tables.jl
- [ ] Add Julia wrappers in `src/Databento.jl` for idiomatic usage (e.g., keyword arguments for submission).
- [ ] Integrate `BatchJob` and `BatchFileDesc` with `Tables.jl` so they can be viewed as DataFrames.
- [ ] Export all new types and methods.

## Step 6: Verification & Testing
- [ ] Create `test/test_batch.jl`.
- [ ] Test listing jobs (can be done without active job submission).
- [ ] Mock or test job submission if a valid API key is available in the environment.
- [ ] Verify `DataFrame` conversion for job lists.

## Progress Tracking
- **Phase 1: Batch Enums**: 0%
- **Phase 2: Batch Structures**: 0%
- **Phase 3: Client Methods**: 0%
- **Phase 4: Tables Integration**: 0%
