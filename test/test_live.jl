using Test
using Databento
using CxxWrap

@testset "Live Client" begin
    # Test Builder
    builder = LiveBuilder()
    set_key_from_env!(builder)
    set_dataset!(builder, "GLBX.MDP3")
    
    # We can't easily test a live connection without potentially blocking forever 
    # or failing due to API key permissions, but we can verify the types are mapped.
    
    @testset "LiveBlocking Construction" begin
        # build_blocking might fail if no internet or invalid key, 
        # but let's try to see if it at least resolves the symbol.
        try
            client = build_blocking(builder)
            @test client isa LiveBlocking
            
            subscribe(client, ["ESH3"], TRADES)
            println("Successfully subscribed via LiveBlocking")
            
            # Start and get one record with timeout
            # metadata = start(client)
            # println("Live session started: ", metadata)
            # rec = next_record(client, 1000) # 1s timeout
        catch e
            if e isa InterruptException rethrow(e) end
            @warn "Could not test live connection (expected if no key/internet): $e"
        end
    end

    @testset "LiveThreaded Construction" begin
        try
            client = build_threaded(builder)
            @test client isa LiveThreaded
            
            subscribe(client, ["ESH3"], TRADES)
            println("Successfully subscribed via LiveThreaded")
        catch e
            if e isa InterruptException rethrow(e) end
            @warn "Could not test live connection: $e"
        end
    end
end
