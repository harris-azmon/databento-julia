using Test
using Databento
using DataFrames
using CxxWrap

@testset "Historical Metadata Diagnostics" begin
    builder = HistoricalBuilder()
    set_key_from_env!(builder)
    client = build(builder)
    
    # 1. List Publishers
    println("Testing metadata_list_publishers...")
    pubs = metadata_list_publishers(client)
    @test length(pubs) > 0
    df_pubs = DataFrame(pubs)
    @test hasproperty(df_pubs, :publisher_id)
    @test hasproperty(df_pubs, :dataset)
    println("Found $(length(pubs)) publishers")

    # 2. List Unit Prices
    println("Testing metadata_list_unit_prices...")
    prices = metadata_list_unit_prices(client, "GLBX.MDP3")
    @test length(prices) > 0
    println("Found $(length(prices)) unit price modes")

    # 3. Get Dataset Condition
    println("Testing metadata_get_dataset_condition...")
    conds = metadata_get_dataset_condition(client, "GLBX.MDP3")
    @test length(conds) > 0
    df_conds = DataFrame(conds)
    @test hasproperty(df_conds, :condition)
    println("First condition: ", df_conds[1, :condition])

    # 4. Get Dataset Range
    println("Testing metadata_get_dataset_range...")
    range = metadata_get_dataset_range(client, "GLBX.MDP3")
    @test !isempty(String(start(range)))
    println("Dataset range: ", range)

    # 5. Diagnostic Estimates
    symbols = ["ESH3"]
    schema = TRADES
    start_ts = "2022-10-28T20:30:00"
    stop_ts = "2022-10-28T20:31:00"

    println("Testing record count estimate...")
    count = metadata_get_record_count(client, "GLBX.MDP3", symbols, schema, start_ts, stop_ts)
    @test count >= 0
    println("Record count estimate: $count")

    println("Testing billable size estimate...")
    size = metadata_get_billable_size(client, "GLBX.MDP3", symbols, schema, start_ts, stop_ts)
    @test size >= 0
    println("Billable size estimate: $size bytes")

    println("Testing cost estimate...")
    cost = metadata_get_cost(client, "GLBX.MDP3", symbols, schema, start_ts, stop_ts)
    @test cost >= 0.0
    println("Cost estimate: \$$cost")
end
