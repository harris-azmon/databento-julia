# Example: Using the Databento Historical Client in Julia
#
# This example demonstrates how to use the Historical client to:
# 1. Authenticate with the Databento API
# 2. Query metadata (datasets, schemas, fields)
# 3. Resolve symbols
# 4. Download historical market data
#
# Prerequisites:
# - Databento API key (get one at https://databento.com)
# - Set DATABENTO_API_KEY environment variable OR pass key explicitly
#
# Usage:
#   export DATABENTO_API_KEY="db-YourApiKeyHere"
#   julia examples/historical_client_example.jl

using Databento

# ============================================================================
# Example 1: Basic Authentication
# ============================================================================

println("=" ^ 70)
println("Example 1: Building a Historical Client")
println("=" ^ 70)

# Method 1: Using environment variable (recommended)
println("\nMethod 1: Using DATABENTO_API_KEY environment variable")
if haskey(ENV, "DATABENTO_API_KEY")
    builder = HistoricalBuilder()
    set_key_from_env!(builder)
    client = build(builder)
    println("✓ Client created successfully using environment variable")
else
    println("⚠ DATABENTO_API_KEY not set in environment")
    println("  Set it with: export DATABENTO_API_KEY=\"db-YourApiKeyHere\"")

    # Method 2: Explicit API key (for testing)
    println("\nMethod 2: Setting API key explicitly")
    builder = HistoricalBuilder()
    set_key!(builder, "db-TestKeyHere")  # Replace with actual key
    client = build(builder)
    println("✓ Client created with explicit key")
end

# ============================================================================
# Example 2: Querying Metadata - List Datasets
# ============================================================================

println("\n" * "=" ^ 70)
println("Example 2: Listing Available Datasets")
println("=" ^ 70)

try
    datasets = metadata_list_datasets(client)
    println("\nFound $(length(datasets)) datasets:")
    for (i, dataset) in enumerate(datasets[1:min(10, length(datasets))])
        println("  $i. $dataset")
    end
    if length(datasets) > 10
        println("  ... and $(length(datasets) - 10) more")
    end
catch e
    println("Error listing datasets: $e")
    println("Note: Requires valid API key with appropriate permissions")
end

# ============================================================================
# Example 3: Querying Schemas for a Dataset
# ============================================================================

println("\n" * "=" ^ 70)
println("Example 3: Listing Schemas for GLBX.MDP3 (CME MDP 3.0)")
println("=" ^ 70)

try
    schemas = metadata_list_schemas(client, "GLBX.MDP3")
    println("\nAvailable schemas for GLBX.MDP3:")
    for schema in schemas
        println("  • $(string(schema))")
    end
catch e
    println("Error listing schemas: $e")
end

# ============================================================================
# Example 4: Querying Fields for a Schema
# ============================================================================

println("\n" * "=" ^ 70)
println("Example 4: Listing Fields for TRADES Schema (DBN Encoding)")
println("=" ^ 70)

try
    fields = metadata_list_fields(client, DBN, TRADES)
    println("\nFields in TRADES schema (DBN encoding):")
    for (i, field) in enumerate(fields)
        println("  $(i). $field")
    end
catch e
    println("Error listing fields: $e")
end

# ============================================================================
# Example 5: Resolving Symbols
# ============================================================================

println("\n" * "=" ^ 70)
println("Example 5: Resolving Futures Symbols to Instrument IDs")
println("=" ^ 70)

try
    # Resolve E-mini S&P 500 and Nasdaq-100 futures symbols
    symbols_to_resolve = ["ESH4", "NQH4"]  # March 2024 contracts
    date = "2024-01-15"

    println("\nResolving symbols: $(join(symbols_to_resolve, ", "))")
    println("Dataset: GLBX.MDP3")
    println("Date: $date")

    result = symbology_resolve(
        client,
        "GLBX.MDP3",
        symbols_to_resolve,
        RAW_SYMBOL,
        date
    )

    println("\nResolution result:")
    println(result)
catch e
    println("Error resolving symbols: $e")
    println("Note: Symbols must be valid for the specified date")
end

# ============================================================================
# Example 6: Downloading Historical Data
# ============================================================================

println("\n" * "=" ^ 70)
println("Example 6: Downloading Historical Trade Data")
println("=" ^ 70)

try
    # Download 1 hour of E-mini S&P 500 trade data
    dataset = "GLBX.MDP3"
    symbols = ["ESH4"]
    schema = TRADES
    start_time = "2024-01-15T09:30"  # Market open
    end_time = "2024-01-15T10:30"    # 1 hour
    output_file = "/tmp/databento_trades_example.dbn"

    println("\nDownloading data:")
    println("  Dataset: $dataset")
    println("  Symbols: $(join(symbols, ", "))")
    println("  Schema: $(string(schema))")
    println("  Time range: $start_time to $end_time")
    println("  Output file: $output_file")

    result_path = timeseries_get_range_to_file(
        client,
        dataset,
        symbols,
        schema,
        start_time,
        end_time,
        RAW_SYMBOL,      # Input symbol type
        INSTRUMENT_ID,   # Output symbol type
        output_file
    )

    println("\n✓ Data downloaded successfully!")
    println("  File: $result_path")

    # Check file size
    if isfile(result_path)
        file_size = filesize(result_path)
        println("  Size: $(round(file_size / 1024 / 1024, digits=2)) MB")
    end

catch e
    println("Error downloading data: $e")
    println("\nNote: This requires:")
    println("  • Valid API key with data access permissions")
    println("  • Sufficient usage quota")
    println("  • Valid symbols for the specified date range")
end

# ============================================================================
# Example 7: Complete Workflow
# ============================================================================

println("\n" * "=" ^ 70)
println("Example 7: Complete Workflow - Query and Download")
println("=" ^ 70)

function download_market_data(api_key::String, dataset::String, symbols::Vector{String},
                             date::String, output_dir::String)
    """
    Complete workflow: authenticate, verify, and download data
    """
    # Step 1: Create client
    builder = HistoricalBuilder()
    set_key!(builder, api_key)
    client = build(builder)
    println("✓ Client authenticated")

    # Step 2: Verify dataset exists
    datasets = metadata_list_datasets(client)
    if !(dataset in datasets)
        error("Dataset '$dataset' not available. Available: $(join(datasets[1:5], ", ")), ...")
    end
    println("✓ Dataset '$dataset' verified")

    # Step 3: Check available schemas
    schemas = metadata_list_schemas(client, dataset)
    if !(TRADES in schemas)
        error("TRADES schema not available for $dataset")
    end
    println("✓ TRADES schema available")

    # Step 4: Resolve symbols
    println("Resolving symbols: $(join(symbols, ", "))")
    resolution = symbology_resolve(client, dataset, symbols, RAW_SYMBOL, date)
    println("✓ Symbols resolved")

    # Step 5: Download data
    output_file = joinpath(output_dir, "$(dataset)_$(date)_trades.dbn")
    println("Downloading to: $output_file")

    result = timeseries_get_range_to_file(
        client,
        dataset,
        symbols,
        TRADES,
        "$(date)T09:30",
        "$(date)T16:00",
        RAW_SYMBOL,
        INSTRUMENT_ID,
        output_file
    )

    println("✓ Download complete: $result")
    return result
end

# Uncomment to run the complete workflow:
# if haskey(ENV, "DATABENTO_API_KEY")
#     try
#         result = download_market_data(
#             ENV["DATABENTO_API_KEY"],
#             "GLBX.MDP3",
#             ["ESH4"],
#             "2024-01-15",
#             "/tmp"
#         )
#         println("\n✅ Complete workflow succeeded: $result")
#     catch e
#         println("\n❌ Workflow failed: $e")
#     end
# end

# ============================================================================
# Summary
# ============================================================================

println("\n" * "=" ^ 70)
println("Summary: Historical Client Features")
println("=" ^ 70)
println("""
The Databento Historical client provides:

✓ Authentication
  • Environment variable (DATABENTO_API_KEY)
  • Explicit API key setting

✓ Metadata Queries
  • metadata_list_datasets() - Available datasets
  • metadata_list_schemas(dataset) - Schemas per dataset
  • metadata_list_fields(encoding, schema) - Field names

✓ Symbology
  • symbology_resolve() - Resolve tickers to instrument IDs

✓ Data Download
  • timeseries_get_range_to_file() - Download historical data to DBN files

Next Steps:
1. Phase 4: Implement DBN reader to parse downloaded files
2. Phase 5: Add streaming data support with callbacks
3. Phase 6: Integrate with Tables.jl for DataFrames

For more information:
• Databento Docs: https://databento.com/docs
• databento-cpp: https://github.com/databento/databento-cpp
• databento-julia: [Your repository URL]
""")

println("=" ^ 70)
println("Example completed!")
println("=" ^ 70)
