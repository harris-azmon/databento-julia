#!/usr/bin/env julia

"""
DBN File Reader Example

This example demonstrates how to read DBN (Databento Binary) files and iterate
through market data records using the databento-julia wrapper.

Phase 4 Implementation: DBN Reader and Record Iteration
"""

using Databento

function example1_basic_file_reading()
    println("\n=== Example 1: Basic DBN File Reading ===\n")

    # Open a DBN file
    dbn_file = "data/trades.dbn"  # Replace with your DBN file path
    reader = DbnFileStore(dbn_file)

    # Get file metadata
    metadata = get_metadata(reader)

    println("Dataset: ", dataset(metadata))
    println("Schema: ", schema(metadata))
    println("Start: ", start(metadata))
    println("End: ", end(metadata))
    println("Symbols: ", symbols(metadata))

    # Read records
    record_count = 0
    while true
        record_ptr = next_record(reader)
        if record_ptr == C_NULL
            break  # End of file
        end

        record = unsafe_load(record_ptr)
        record_count += 1
    end

    println("Total records: $record_count")
end

function example2_iterate_and_dispatch()
    println("\n=== Example 2: Iterate and Dispatch by Type ===\n")

    dbn_file = "data/mixed_schema.dbn"
    reader = DbnFileStore(dbn_file)

    # Counters for different message types
    mbo_count = 0
    trade_count = 0
    mbp_count = 0
    other_count = 0

    while true
        record_ptr = next_record(reader)
        if record_ptr == C_NULL
            break
        end

        record = unsafe_load(record_ptr)

        # Dispatch based on record type
        if holds_mbo(record)
            mbo_count += 1
            # Get MBO message
            mbo_ptr = get_mbo_if(record)
            if mbo_ptr != C_NULL
                mbo = unsafe_load(mbo_ptr)
                # Process MBO message
                # println("MBO: order_id=$(mbo.order_id), price=$(mbo.price)")
            end

        elseif holds_trade(record)
            trade_count += 1
            trade_ptr = get_trade_if(record)
            if trade_ptr != C_NULL
                trade = unsafe_load(trade_ptr)
                # Process trade message
                # println("Trade: price=$(trade.price), size=$(trade.size)")
            end

        elseif holds_mbp1(record) || holds_mbp10(record)
            mbp_count += 1
            # Process MBP message

        else
            other_count += 1
        end
    end

    println("Message Type Counts:")
    println("  MBO: $mbo_count")
    println("  Trades: $trade_count")
    println("  MBP: $mbp_count")
    println("  Other: $other_count")
end

function example3_process_trades()
    println("\n=== Example 3: Process Trade Messages ===\n")

    dbn_file = "data/trades.dbn"
    reader = DbnFileStore(dbn_file)
    metadata = get_metadata(reader)

    println("Processing trades for: ", symbols(metadata))

    trade_volumes = Dict{Int64, Int64}()  # instrument_id => total volume

    count = 0
    max_records = 1000  # Limit for example

    while count < max_records
        record_ptr = next_record(reader)
        if record_ptr == C_NULL
            break
        end

        record = unsafe_load(record_ptr)

        if holds_trade(record)
            trade_ptr = get_trade_if(record)
            if trade_ptr != C_NULL
                trade = unsafe_load(trade_ptr)
                hdr = trade.hd

                # Accumulate volume by instrument
                instrument_id = hdr.instrument_id
                trade_volumes[instrument_id] = get(trade_volumes, instrument_id, 0) + trade.size

                # Print first few trades
                if count < 10
                    ts_event_ns = hdr.ts_event
                    println("Trade #$count:")
                    println("  Instrument: $instrument_id")
                    println("  Price: $(trade.price)")
                    println("  Size: $(trade.size)")
                    println("  Side: $(trade.side)")
                    println("  Timestamp: $ts_event_ns ns")
                    println()
                end
            end
        end

        count += 1
    end

    println("\nTotal Volume by Instrument:")
    for (instr_id, volume) in sort(collect(trade_volumes))
        println("  Instrument $instr_id: $volume")
    end
end

function example4_market_by_order()
    println("\n=== Example 4: Market-by-Order (MBO) Processing ===\n")

    dbn_file = "data/mbo.dbn"
    reader = DbnFileStore(dbn_file)

    # Track order book updates
    add_count = 0
    modify_count = 0
    cancel_count = 0
    fill_count = 0

    count = 0
    max_records = 1000

    while count < max_records
        record_ptr = next_record(reader)
        if record_ptr == C_NULL
            break
        end

        record = unsafe_load(record_ptr)

        if holds_mbo(record)
            mbo_ptr = get_mbo_if(record)
            if mbo_ptr != C_NULL
                mbo = unsafe_load(mbo_ptr)

                # Classify by action
                action_char = Char(mbo.action)
                if action_char == 'A'
                    add_count += 1
                elseif action_char == 'M'
                    modify_count += 1
                elseif action_char == 'C'
                    cancel_count += 1
                elseif action_char == 'F'
                    fill_count += 1
                end

                # Print first few MBO messages
                if count < 5
                    side_char = Char(mbo.side)
                    println("MBO #$count:")
                    println("  Order ID: $(mbo.order_id)")
                    println("  Action: $action_char")
                    println("  Side: $side_char")
                    println("  Price: $(mbo.price)")
                    println("  Size: $(mbo.size)")
                    println()
                end
            end
        end

        count += 1
    end

    println("Order Book Action Counts:")
    println("  Adds: $add_count")
    println("  Modifies: $modify_count")
    println("  Cancels: $cancel_count")
    println("  Fills: $fill_count")
end

function example5_instrument_definitions()
    println("\n=== Example 5: Instrument Definitions ===\n")

    dbn_file = "data/definitions.dbn"
    reader = DbnFileStore(dbn_file)

    instruments = Dict{Int64, String}()  # instrument_id => raw_symbol

    while true
        record_ptr = next_record(reader)
        if record_ptr == C_NULL
            break
        end

        record = unsafe_load(record_ptr)

        if holds_instrument_def(record)
            def_ptr = get_instrument_def_if(record)
            if def_ptr != C_NULL
                def = unsafe_load(def_ptr)
                hdr = def.hd

                # Extract raw symbol (fixed-length char array)
                raw_symbol = String(def.raw_symbol[1:findfirst(==(0), def.raw_symbol)-1])
                instruments[hdr.instrument_id] = raw_symbol

                println("Instrument Definition:")
                println("  ID: $(hdr.instrument_id)")
                println("  Symbol: $raw_symbol")
                println("  Min Price Increment: $(def.min_price_increment)")
                println()
            end
        end
    end

    println("Total instruments defined: $(length(instruments))")
end

function example6_download_and_read()
    println("\n=== Example 6: Download and Read DBN File ===\n")

    # First, download data using Historical client
    builder = HistoricalBuilder()
    set_key_from_env!(builder)
    client = build(builder)

    output_file = "/tmp/es_trades.dbn"

    println("Downloading data...")
    result = timeseries_get_range_to_file(
        client,
        "GLBX.MDP3",
        ["ESH4"],
        TRADES,
        "2024-01-15T09:30",
        "2024-01-15T10:00",
        RAW_SYMBOL,
        INSTRUMENT_ID,
        output_file
    )
    println("Downloaded to: $result")

    # Now read the downloaded file
    println("\nReading downloaded DBN file...")
    reader = DbnFileStore(output_file)
    metadata = get_metadata(reader)

    println("Dataset: ", dataset(metadata))
    println("Schema: ", schema(metadata))
    println("Symbols: ", symbols(metadata))

    # Count records
    record_count = 0
    while next_record(reader) != C_NULL
        record_count += 1
    end

    println("Records in file: $record_count")
end

function example7_metadata_inspection()
    println("\n=== Example 7: Metadata Inspection ===\n")

    dbn_file = "data/sample.dbn"
    reader = DbnFileStore(dbn_file)
    metadata = get_metadata(reader)

    println("DBN File Metadata:")
    println("  Version: ", version(metadata))
    println("  Dataset: ", dataset(metadata))
    println("  Schema: ", schema(metadata))
    println("  Mixed Schema: ", has_mixed_schema(metadata))
    println("  Start Time (ns): ", start(metadata))
    println("  End Time (ns): ", end(metadata))
    println("  Record Limit: ", limit(metadata))
    println("  Input SType: ", stype_in(metadata))
    println("  Output SType: ", stype_out(metadata))
    println("  Has Timestamp Out: ", ts_out(metadata))
    println("  Symbols: ", join(symbols(metadata), ", "))

    partial_syms = partial(metadata)
    if !isempty(partial_syms)
        println("  Partial Data: ", join(partial_syms, ", "))
    end

    not_found_syms = not_found(metadata)
    if !isempty(not_found_syms)
        println("  Not Found: ", join(not_found_syms, ", "))
    end
end

# Main execution
function main()
    println("=" ^ 60)
    println("Databento Julia - DBN Reader Examples")
    println("=" ^ 60)

    # Run examples (uncomment as needed)
    try
        # example1_basic_file_reading()
        # example2_iterate_and_dispatch()
        # example3_process_trades()
        # example4_market_by_order()
        # example5_instrument_definitions()
        # example6_download_and_read()
        # example7_metadata_inspection()

        println("\n✓ Examples completed successfully!")
        println("\nNote: Uncomment specific examples in main() to run them.")
        println("Make sure you have valid DBN files or set DATABENTO_API_KEY for downloads.")

    catch e
        println("\n✗ Error running examples:")
        println(e)
        rethrow(e)
    end
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end
