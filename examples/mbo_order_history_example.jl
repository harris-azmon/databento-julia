# Julia Translation of Databento MBO Order History Example
# Demonstrates filtering MBO data by a specific order_id.

using Databento
using DataFrames
using Dates
using Printf
using CxxWrap

function format_action(action::Action)
    if action == ACTION_ADD; return "[A]dded"
    elseif action == ACTION_CANCEL; return "[C]anceled"
    elseif action == ACTION_MODIFY; return "[M]odified"
    elseif action == ACTION_CLEAR; return "clea[R]ed"
    elseif action == ACTION_TRADE; return "[T]raded"
    elseif action == ACTION_FILL; return "[F]illed"
    elseif action == ACTION_NONE; return "[N]one"
    else; return "Unknown"
    end
end

function main()
    # 1. Create a historical client
    builder = HistoricalBuilder()
    set_key_from_env!(builder)
    client = build(builder)

    # We'll inspect the history of a particular order
    target_order_id = 6410543150678
    output_file = "es_mbo.dbn"

    # 2. Request a minute of MBO data
    println("Fetching MBO data from GLBX.MDP3...")
    symbols = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(["ES.v.0"]))
    
    timeseries_get_range_to_file(
        client,
        "GLBX.MDP3",                # dataset
        symbols,                    # symbols
        MBO,                        # schema (MBO)
        "2022-08-26T10:59",         # start
        "2022-08-26T11:00",         # end
        CONTINUOUS,                 # stype_in
        INSTRUMENT_ID,              # stype_out
        0,                          # limit
        output_file                 # output_path
    )

    # 3. Load and filter data
    store = DbnFileStore(output_file)
    df = DataFrame(store)
    
    # Filter for the specific order_id
    df_filtered = filter(row -> row.order_id == target_order_id, df)

    # 4. Display results matching C++ format
    # ts_recv  price  size  action
    for row in eachrow(df_filtered)
        dt = unix2datetime(row.ts_recv / 1e9)
        # Price is fixed-point, divide by 1e9
        price_val = row.price / 1e9
        
        @printf("%s  %.2f  %2d  %s\n",
            Dates.format(dt, "yyyy-mm-ddTHH:MM:SS.s") * "Z",
            price_val,
            row.size,
            format_action(row.action)
        )
    end

    # Cleanup
    if isfile(output_file)
        rm(output_file)
    end
end

main()
