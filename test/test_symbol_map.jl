using Test
using Databento
using CxxWrap

const TRADES_DBN = "deps/build/_deps/databento-src/tests/data/test_data.trades.dbn"

@testset "TsSymbolMap" begin
    @test isfile(TRADES_DBN)
    
    store = DbnFileStore(TRADES_DBN)
    metadata = get_metadata(store)
    
    # Create symbol map from metadata
    sm = create_symbol_map(metadata)
    
    @test !is_empty(sm)
    @test only(size(sm)) > 0
    
    # Test lookup
    count = 0
    for record in store
        count += 1
        # Resolve symbol for this record
        symbol = at(sm, record)
        @test symbol isa String
        @test !isempty(symbol)
        
        if count == 1
            println("First record resolved to symbol: ", symbol)
        end
        
        if count >= 10 break end
    end
    @test count > 0
    println("Successfully resolved $count records using TsSymbolMap")
end
