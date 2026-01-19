using Test
using Databento

@testset "Databento.jl - Phase 1: Core Enums" begin

    @testset "Schema Enum" begin
        # Test that Schema constants are defined
        @test isdefined(Databento, :Schema)
        @test isdefined(Databento, :MBO)
        @test isdefined(Databento, :MBP1)
        @test isdefined(Databento, :TRADES)
        @test isdefined(Databento, :DEFINITION)

        # Test string conversion
        @test string(MBO) == "mbo"
        @test string(MBP1) == "mbp-1"
        @test string(TRADES) == "trades"
        @test string(DEFINITION) == "definition"
        @test string(IMBALANCE) == "imbalance"

        # Test REPL display
        io = IOBuffer()
        show(io, MBO)
        @test String(take!(io)) == "Schema::mbo"
    end

    @testset "Encoding Enum" begin
        # Test that Encoding constants are defined
        @test isdefined(Databento, :Encoding)
        @test isdefined(Databento, :DBN)
        @test isdefined(Databento, :CSV)
        @test isdefined(Databento, :JSON)

        # Test string conversion
        @test string(DBN) == "dbn"
        @test string(CSV) == "csv"
        @test string(JSON) == "json"

        # Test REPL display
        io = IOBuffer()
        show(io, DBN)
        @test String(take!(io)) == "Encoding::dbn"
    end

    @testset "SType Enum" begin
        # Test that SType constants are defined
        @test isdefined(Databento, :SType)
        @test isdefined(Databento, :RAW_SYMBOL)
        @test isdefined(Databento, :INSTRUMENT_ID)
        @test isdefined(Databento, :PARENT)
        @test isdefined(Databento, :CONTINUOUS)

        # Test string conversion
        @test string(RAW_SYMBOL) == "raw_symbol"
        @test string(INSTRUMENT_ID) == "instrument_id"
        @test string(PARENT) == "parent"
        @test string(CONTINUOUS) == "continuous"

        # Test REPL display
        io = IOBuffer()
        show(io, RAW_SYMBOL)
        @test String(take!(io)) == "SType::raw_symbol"
    end

    @testset "Dataset Enum" begin
        # Test that Dataset constants are defined
        @test isdefined(Databento, :Dataset)
        @test isdefined(Databento, :GLBX_MDP3)
        @test isdefined(Databento, :XNAS_ITCH)
        @test isdefined(Databento, :OPRA_PILLAR)

        # Test string conversion
        @test string(GLBX_MDP3) == "GLBX.MDP3"
        @test string(XNAS_ITCH) == "XNAS.ITCH"
        @test string(OPRA_PILLAR) == "OPRA.PILLAR"
        @test string(DBEQ_BASIC) == "DBEQ.BASIC"

        # Test REPL display
        io = IOBuffer()
        show(io, GLBX_MDP3)
        @test String(take!(io)) == "Dataset::GLBX.MDP3"
    end

    @testset "Enum Type Safety" begin
        # Ensure enums are different types
        @test typeof(MBO) != typeof(DBN)
        @test typeof(RAW_SYMBOL) != typeof(GLBX_MDP3)
        @test typeof(TRADES) != typeof(JSON)
    end
end
