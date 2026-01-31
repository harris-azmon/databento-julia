using Test
using Databento
using Tables
using DataFrames
using CxxWrap

const TRADES_DBN = "deps/build/_deps/databento-src/tests/data/test_data.trades.dbn"
const DEF_DBN = "deps/build/_deps/databento-src/tests/data/test_data.definition.dbn"

@testset "Tables.jl Integration" begin
    @testset "Trades DBN" begin
        @test isfile(TRADES_DBN)
        store = DbnFileStore(TRADES_DBN)
        count = 0
        for row in store
            count += 1
            @test row isa Record
            if count >= 10 break end
        end
        @test count > 0
    end

    @testset "Definition DBN" begin
        @test isfile(DEF_DBN)
        store = DbnFileStore(DEF_DBN)
        count = 0
        for row in store
            count += 1
            @test Tables.getcolumn(row, :raw_symbol) isa String
            if count >= 5 break end
        end
        @test count > 0
    end

    @testset "DataFrames Integration" begin
        @test isfile(TRADES_DBN)
        store = DbnFileStore(TRADES_DBN)
        df = DataFrame(store)
        @test df isa DataFrame
        @test nrow(df) > 0
    end

        @testset "In-memory Callbacks" begin

            builder = HistoricalBuilder()

            set_key_from_env!(builder)

            client = build(builder)

            

            symbols = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(["ESH3"]))

            

            metadata_received = false

            record_count = 0

            

            timeseries_get_range(

                client,

                "GLBX.MDP3",

                symbols,

                TBBO,

                "2022-10-28T20:30:00",

                "2022-10-28T20:31:00",

                RAW_SYMBOL,

                INSTRUMENT_ID,

                10, # limit

                (metadata) -> begin

                    metadata_received = true

                    # We can create a symbol map from the received metadata

                    sm = create_symbol_map(metadata)

                    @test size(sm, 1) > 0

                end,

                (record) -> begin

                    record_count += 1

                    return KEEP_GOING_CONTINUE

                end

            )

            @test metadata_received

            @test record_count > 0

            println("Successfully verified Metadata and Record callbacks")

        end

    end

    