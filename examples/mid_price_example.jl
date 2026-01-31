# Julia Translation of Databento TBBO Mid-Price Example
# Matches the logic and output of the C++ documentation example.

using Databento
using DataFrames
using Dates
using Printf
using CxxWrap

function main()
    # 1. Create a historical client with the provided inline key
    builder = HistoricalBuilder()
    set_key_from_env!(builder)
    client = build(builder)

    # 2. Request TBBO data
    output_file = "es_tbbo.dbn"
    symbols = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(["ESH3"]))
    
    println("Fetching TBBO data from GLBX.MDP3...")
    timeseries_get_range_to_file(
        client,
        "GLBX.MDP3",
        symbols,
        TBBO,                       # Schema TBBO
        "2022-10-28T20:30:00",      # start
        "2022-10-28T21:00:00",      # end
        RAW_SYMBOL,                 # stype_in
        INSTRUMENT_ID,              # stype_out
        0,                          # limit
        output_file
    )

    # 3. Process data
    store = DbnFileStore(output_file)
    
    # In Julia, we can iterate directly or use a DataFrame
    # Let's use iteration to match the callback style
    println("\nInstrumentID\tAsk Px\tMid Px\tBid Px")
    println("-" ^ 40)
    
    count = 0
    for row in store
        # Calculate mid price
        # bid_px and ask_px are already available via the Tables integration
        id = Tables.getcolumn(row, :instrument_id)
        bid = Tables.getcolumn(row, :bid_px)
        ask = Tables.getcolumn(row, :ask_px)
        
        if !isnothing(bid) && !isnothing(ask)
            mid = (ask + bid) / 2
            
            # Print matching C++ output format
            @printf("%d\t%d\t%.1f\t%d\n", 
                id,
                ask,
                mid,
                bid
            )
            
            count += 1
            if count >= 10
                println("...")
                break
            end
        end
    end

    # Cleanup
    if isfile(output_file)
        rm(output_file)
    end
end

main()
