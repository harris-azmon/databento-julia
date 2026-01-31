# Julia Translation of Databento Parent Symbology Example
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

    # 2. Request definition data
    # In Julia, we download to a DBN file first. 
    # This replaces the C++ callback approach with an idiomatic Julia file-to-table flow.
    output_file = "es_definitions.dbn"
    
    println("Fetching range from GLBX.MDP3...")
    # Note: C++ method expects 10 arguments including limit and StdVector for symbols
    symbols = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(["ES.FUT"]))
    
    timeseries_get_range_to_file(
        client,
        "GLBX.MDP3",                # dataset
        symbols,                    # symbols (StdVector)
        DEFINITION,                 # schema
        "2022-08-26",               # start
        "2022-08-27",               # end
        PARENT,                     # stype_in
        INSTRUMENT_ID,              # stype_out
        0,                          # limit (0 = no limit)
        output_file                 # output_path
    )

    # 3. Load data into a DataFrame
    # DbnFileStore provides the metadata and record iteration
    store = DbnFileStore(output_file)
    df = DataFrame(store)

    # 4. Sort by expiration and raw_symbol (matching the C++ sort logic)
    # The C++ example uses a tuple {lhs.expiration, lhs.RawSymbol()}
    sort!(df, [:expiration, :raw_symbol])

    # 5. Print results matching the C++ output format:
    # definition.hd.instrument_id << '\t' << symbol_mappings.At(definition) << '\t' << ToIso8601(definition.expiration)
    println("\n$(rpad("ID", 10))\t$(rpad("Symbol", 9))\tExpiration (ISO8601)")
    println("-" ^ 50)
    
    for row in eachrow(df)
        # Convert nanoseconds to DateTime for printing
        # Databento uses Unix nanos since epoch
        dt = unix2datetime(row.expiration / 1e9)
        
        @printf("%-10d\t%-9s\t%s\n", 
            row.instrument_id, 
            row.raw_symbol,
            Dates.format(dt, "yyyy-mm-ddTHH:MM:SS") * "Z"
        )
    end

    # Cleanup
    if isfile(output_file)
        rm(output_file)
    end
end

main()