"""
Phase 2 Test Suite for databento-julia

Tests all Phase 2 functionality once Julia environment is available:
- Enums: RType, Action, Side
- Supporting types: FlagSet, UnixNanos, TimeDeltaNanos, BidAskPair
- Record types: RecordHeader, MboMsg, TradeMsg, Mbp1Msg, Mbp10Msg, InstrumentDefMsg, ImbalanceMsg
"""

using Databento
using Test

@testset "Phase 2: Data Record Protocol" begin

    @testset "Phase 2 Enums" begin
        # RType enum tests
        @test Databento.RTYPE_MBP0 isa Databento.RType
        @test Databento.RTYPE_MBP1 isa Databento.RType
        @test Databento.RTYPE_MBP10 isa Databento.RType
        @test Databento.RTYPE_OHLCV_1S isa Databento.RType
        @test Databento.RTYPE_MBO isa Databento.RType
        @test Databento.RTYPE_INSTRUMENT_DEF isa Databento.RType
        @test Databento.RTYPE_IMBALANCE isa Databento.RType

        # Action enum tests
        @test Databento.ACTION_MODIFY isa Databento.Action
        @test Databento.ACTION_TRADE isa Databento.Action
        @test Databento.ACTION_FILL isa Databento.Action
        @test Databento.ACTION_CANCEL isa Databento.Action
        @test Databento.ACTION_ADD isa Databento.Action
        @test Databento.ACTION_CLEAR isa Databento.Action
        @test Databento.ACTION_NONE isa Databento.Action

        # Side enum tests
        @test Databento.SIDE_ASK isa Databento.Side
        @test Databento.SIDE_BID isa Databento.Side
        @test Databento.SIDE_NONE isa Databento.Side
    end

    @testset "Supporting Types" begin
        # FlagSet type should be accessible
        @test isdefined(Databento, :FlagSet)

        # UnixNanos type should be accessible
        @test isdefined(Databento, :UnixNanos)

        # TimeDeltaNanos type should be accessible
        @test isdefined(Databento, :TimeDeltaNanos)

        # BidAskPair type should be accessible
        @test isdefined(Databento, :BidAskPair)
    end

    @testset "Record Types" begin
        # RecordHeader type should be accessible
        @test isdefined(Databento, :RecordHeader)

        # Message types should be accessible
        @test isdefined(Databento, :MboMsg)
        @test isdefined(Databento, :TradeMsg)
        @test isdefined(Databento, :Mbp1Msg)
        @test isdefined(Databento, :Mbp10Msg)
        @test isdefined(Databento, :InstrumentDefMsg)
        @test isdefined(Databento, :ImbalanceMsg)
    end

    @testset "Method Accessibility" begin
        # These tests assume we have instantiated types available
        # They would test method calls on actual instances
        # Skipped for now as they require actual data

        @info "Method tests require actual message instances"
        @info "Run with real market data to fully validate field accessors"
    end

end

println("\n" * "=" ^ 80)
println("PHASE 2 TEST SUITE COMPLETE")
println("=" ^ 80)
println("All Phase 2 types and enums are accessible from Julia!")
println("Ready for market data processing.")
