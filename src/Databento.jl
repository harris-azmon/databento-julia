module Databento

using CxxWrap
using Tables

# Load the C++ extension library
const depsfile = joinpath(@__DIR__, "..", "deps", "deps.jl")
if !isfile(depsfile)
    error("Databento C++ library not properly installed. Please run Pkg.build(\"Databento\").")
end
include(depsfile)

# Wrap the C++ module
@wrapmodule(() -> libdatabento_jl)

function __init__()
    @initcxx
end

# ============================================================================ 
# PHASE 1: Core Enums
# ============================================================================ 

# Export enum types
export Schema, Encoding, FeedMode, SType, Dataset, Publisher
export StatusAction, StatusReason, TradingEvent, TriState, StatType, StatUpdateAction
export Compression, SplitDuration, Delivery, JobState, DatasetCondition

# Export Schema constants
export MBO, MBP1, MBP10, TBBO, TRADES
export OHLCV_1S, OHLCV_1M, OHLCV_1H, OHLCV_1D
export DEFINITION, STATISTICS, STATUS, IMBALANCE
export CMBP1, CBBO_1S, CBBO_1M, TCBBO, BBO_1S, BBO_1M

# Export Encoding constants
export DBN, CSV, JSON

# Export FeedMode constants
export HISTORICAL, HISTORICAL_STREAMING, LIVE

# Export SType constants
export INSTRUMENT_ID, RAW_SYMBOL, CONTINUOUS, PARENT
export NASDAQ_SYMBOL, CMS_SYMBOL

# Export Dataset constants
export GLBX_MDP3, XNAS_ITCH, XBOS_ITCH, XPSX_ITCH
export BATS_PITCH, BATY_PITCH, EDGA_PITCH, EDGX_PITCH
export XNYS_PILLAR, XCIS_PILLAR, XASE_PILLAR, XCHI_PILLAR
export XCIS_BBO, XCIS_TRADES, MEMX_MEMOIR, EPRL_DOM
export OPRA_PILLAR, DBEQ_BASIC, ARCX_PILLAR, IEXG_TOPS
export EQUS_PLUS, XNYS_BBO, XNYS_TRADES, XNAS_QBBO, XNAS_NLS
export IFEU_IMPACT, NDEX_IMPACT, EQUS_ALL, XNAS_BASIC
export EQUS_SUMMARY, XCIS_TRADESBBO, XNYS_TRADESBBO, EQUS_MINI

# Export RType constants
export RTYPE_MBP0, RTYPE_MBP1, RTYPE_MBP10, RTYPE_OHLCV_DEPRECATED
export RTYPE_OHLCV_1S, RTYPE_OHLCV_1M, RTYPE_OHLCV_1H, RTYPE_OHLCV_1D
export RTYPE_STATUS, RTYPE_INSTRUMENT_DEF, RTYPE_IMBALANCE, RTYPE_ERROR
export RTYPE_SYMBOL_MAPPING, RTYPE_SYSTEM, RTYPE_STATISTICS, RTYPE_MBO
export RTYPE_CMBP1, RTYPE_CBBO_1S, RTYPE_CBBO_1M, RTYPE_TCBBO, RTYPE_BBO_1S, RTYPE_BBO_1M

# Export Action constants
export ACTION_MODIFY, ACTION_TRADE, ACTION_FILL, ACTION_CANCEL
export ACTION_ADD, ACTION_CLEAR, ACTION_NONE

# Export Side constants
export SIDE_ASK, SIDE_BID, SIDE_NONE

# Export InstrumentClass constants
export INSTRUMENT_CLASS_BOND, INSTRUMENT_CLASS_CALL, INSTRUMENT_CLASS_FUTURE
export INSTRUMENT_CLASS_STOCK, INSTRUMENT_CLASS_MIXED_SPREAD, INSTRUMENT_CLASS_PUT
export INSTRUMENT_CLASS_FUTURE_SPREAD, INSTRUMENT_CLASS_OPTION_SPREAD
export INSTRUMENT_CLASS_FX_SPOT, INSTRUMENT_CLASS_COMMODITY_SPOT

# Export Types
export DbnFileStore, Record, Metadata, RecordHeader, InstrumentClass, Action, Side, RType
export MboMsg, TradeMsg, Mbp1Msg, Mbp10Msg, InstrumentDefMsg, ImbalanceMsg, TbboMsg
export StatusMsg, StatMsg, ErrorMsg, SymbolMappingMsg, SystemMsg
export BboMsg, Cmbp1Msg, CbboMsg, ConsolidatedBidAskPair
export TsSymbolMap
export BatchJob, BatchFileDesc
export PublisherDetail, UnitPricesForMode, DatasetConditionDetail, DatasetRange
export Historical, HistoricalBuilder

# Export Methods
export next_record, get_metadata, header, rtype, start, stop
export get_mbo_if, get_trade_if, get_mbp1_if, get_mbp10_if, get_imbalance_if, get_instrument_def_if, get_ohlcv_if
export get_status_if, get_stat_if, get_error_if, get_symbol_mapping_if, get_system_if, get_bbo_if, get_cmbp1_if, get_cbbo_if
export set_key!, set_key_from_env!, build
export metadata_list_datasets, metadata_list_schemas, metadata_list_fields
export metadata_list_publishers, metadata_list_unit_prices, metadata_get_dataset_condition, metadata_get_dataset_range
export metadata_get_record_count, metadata_get_billable_size, metadata_get_cost
export symbology_resolve, timeseries_get_range_to_file, timeseries_get_range
export batch_submit_job, batch_list_jobs, batch_list_files, batch_download
export level, bid_px, ask_px, bid_sz, ask_sz, bid_ct, ask_ct
export KEEP_GOING_CONTINUE, KEEP_GOING_STOP
export create_symbol_map, at, is_empty

Base.convert(::Type{Action}, x::Action) = x
Base.convert(::Type{Side}, x::Side) = x
Base.convert(::Type{InstrumentClass}, x::InstrumentClass) = x
Base.convert(::Type{StatusAction}, x::StatusAction) = x
Base.convert(::Type{StatusReason}, x::StatusReason) = x
Base.convert(::Type{TradingEvent}, x::TradingEvent) = x
Base.convert(::Type{TriState}, x::TriState) = x
Base.convert(::Type{StatType}, x::StatType) = x
Base.convert(::Type{StatUpdateAction}, x::StatUpdateAction) = x

# Export to_string for enums
export to_string
to_string(s::Schema) = to_string_schema(s)
to_string(e::Encoding) = to_string_encoding(e)
to_string(m::FeedMode) = to_string_feed_mode(m)
to_string(s::SType) = to_string_stype(s)
to_string(d::Dataset) = to_string_dataset(d)
to_string(p::Publisher) = to_string_publisher(p)
to_string(i::InstrumentClass) = to_string_instrument_class(i)
to_string(m::MatchAlgorithm) = to_string_match_algorithm(m)
to_string(s::StatusAction) = to_string_status_action(s)
to_string(s::StatusReason) = to_string_status_reason(s)
to_string(t::TradingEvent) = to_string_trading_event(t)
to_string(t::TriState) = to_string_tri_state(t)
to_string(s::StatType) = to_string_stat_type(s)
to_string(s::StatUpdateAction) = to_string_stat_update_action(s)
to_string(c::Compression) = to_string_compression(c)
to_string(d::SplitDuration) = to_string_split_duration(d)
to_string(d::Delivery) = to_string_delivery(d)
to_string(s::JobState) = to_string_job_state(s)
to_string(c::DatasetCondition) = to_string_dataset_condition(c)

# Show methods for better REPL display
Base.show(io::IO, s::Schema) = print(io, "Schema::", to_string(s))
Base.show(io::IO, e::Encoding) = print(io, "Encoding::", to_string(e))
Base.show(io::IO, m::FeedMode) = print(io, "FeedMode::", to_string(m))
Base.show(io::IO, s::SType) = print(io, "SType::", to_string(s))
Base.show(io::IO, d::Dataset) = print(io, "Dataset::", to_string(d))
Base.show(io::IO, p::Publisher) = print(io, "Publisher::", to_string(p))
Base.show(io::IO, i::InstrumentClass) = print(io, "InstrumentClass::", to_string(i))
Base.show(io::IO, s::StatusAction) = print(io, "StatusAction::", to_string(s))
Base.show(io::IO, s::StatusReason) = print(io, "StatusReason::", to_string(s))
Base.show(io::IO, t::TradingEvent) = print(io, "TradingEvent::", to_string(t))
Base.show(io::IO, t::TriState) = print(io, "TriState::", to_string(t))
Base.show(io::IO, s::StatType) = print(io, "StatType::", to_string(s))
Base.show(io::IO, s::StatUpdateAction) = print(io, "StatUpdateAction::", to_string(s))
Base.show(io::IO, c::Compression) = print(io, "Compression::", to_string(c))
Base.show(io::IO, d::SplitDuration) = print(io, "SplitDuration::", to_string(d))
Base.show(io::IO, d::Delivery) = print(io, "Delivery::", to_string(d))
Base.show(io::IO, s::JobState) = print(io, "JobState::", to_string(s))
Base.show(io::IO, c::DatasetCondition) = print(io, "DatasetCondition::", to_string(c))

# TsSymbolMap dispatch
Base.size(m::TsSymbolMap) = (Int(map_size(m)),)
at(m::TsSymbolMap, r::TradeMsg) = String(at_trade(m, r))
at(m::TsSymbolMap, r::MboMsg) = String(at_mbo(m, r))
at(m::TsSymbolMap, r::Mbp1Msg) = String(at_mbp1(m, r))
at(m::TsSymbolMap, r::Mbp10Msg) = String(at_mbp10(m, r))
at(m::TsSymbolMap, r::OhlcvMsg) = String(at_ohlcv(m, r))
at(m::TsSymbolMap, r::InstrumentDefMsg) = String(at_instr_def(m, r))
at(m::TsSymbolMap, r::StatusMsg) = String(at_status(m, r))
at(m::TsSymbolMap, r::StatMsg) = String(at_stat(m, r))

function at(m::TsSymbolMap, r::Record)
    rt = rtype(r)
    if rt == RTYPE_MBP0
        msg_ptr = get_trade_if(r)
        return msg_ptr != C_NULL ? at(m, msg_ptr[]) : ""
    elseif rt == RTYPE_MBO
        msg_ptr = get_mbo_if(r)
        return msg_ptr != C_NULL ? at(m, msg_ptr[]) : ""
    elseif rt == RTYPE_MBP1
        msg_ptr = get_mbp1_if(r)
        return msg_ptr != C_NULL ? at(m, msg_ptr[]) : ""
    elseif rt == RTYPE_MBP10
        msg_ptr = get_mbp10_if(r)
        return msg_ptr != C_NULL ? at(m, msg_ptr[]) : ""
    elseif rt == RTYPE_INSTRUMENT_DEF
        msg_ptr = get_instrument_def_if(r)
        return msg_ptr != C_NULL ? at(m, msg_ptr[]) : ""
    elseif rt == RTYPE_STATUS
        msg_ptr = get_status_if(r)
        return msg_ptr != C_NULL ? at(m, msg_ptr[]) : ""
    elseif rt == RTYPE_STATISTICS
        msg_ptr = get_stat_if(r)
        return msg_ptr != C_NULL ? at(m, msg_ptr[]) : ""
    end
    return ""
end

# ============================================================================
# Batch API wrappers
# ============================================================================

function batch_submit_job(client::Historical, dataset::String, symbols::Vector{String}, schema::Schema, start::String, stop::String;
    stype_in::SType=RAW_SYMBOL,
    stype_out::SType=INSTRUMENT_ID,
    limit::Integer=0,
    encoding::Encoding=DBN,
    compression::Compression=Compression.None,
    pretty_px::Bool=false,
    pretty_ts::Bool=false,
    map_symbols::Bool=false,
    split_symbols::Bool=false,
    split_duration::SplitDuration=SplitDuration.None,
    split_size::Integer=0,
    delivery::Delivery=Delivery.Download)
    
    symbols_vec = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(symbols))
    
    return batch_submit_job(client, dataset, symbols_vec, schema, start, stop, encoding, compression, pretty_px, pretty_ts, map_symbols, split_symbols, split_duration, UInt64(split_size), delivery, stype_in, stype_out, UInt64(limit))
end

function metadata_get_record_count(client::Historical, dataset::String, symbols::Vector{String}, schema::Schema, start::String, stop::String;
    stype_in::SType=RAW_SYMBOL,
    limit::Integer=0)
    symbols_vec = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(symbols))
    return metadata_get_record_count(client, dataset, start, stop, symbols_vec, schema, stype_in, UInt64(limit))
end

function metadata_get_billable_size(client::Historical, dataset::String, symbols::Vector{String}, schema::Schema, start::String, stop::String;
    stype_in::SType=RAW_SYMBOL,
    limit::Integer=0)
    symbols_vec = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(symbols))
    return metadata_get_billable_size(client, dataset, start, stop, symbols_vec, schema, stype_in, UInt64(limit))
end

function metadata_get_cost(client::Historical, dataset::String, symbols::Vector{String}, schema::Schema, start::String, stop::String;
    mode::FeedMode=HISTORICAL_STREAMING,
    stype_in::SType=RAW_SYMBOL,
    limit::Integer=0)
    symbols_vec = CxxWrap.StdLib.StdVector(CxxWrap.StdLib.StdString.(symbols))
    return metadata_get_cost(client, dataset, start, stop, symbols_vec, schema, mode, stype_in, UInt64(limit))
end

# ============================================================================
# Tables.jl Integration
# ============================================================================
Tables.istable(::Type{<:DbnFileStore}) = true
Tables.rowaccess(::Type{<:DbnFileStore}) = true
Tables.rows(store::DbnFileStore) = store

# Record as an AbstractRow
Tables.istable(::Type{<:Record}) = true # A record can be seen as a single row table

# BatchJob and BatchFileDesc as tables
Tables.istable(::Type{BatchJobVector}) = true
Tables.rowaccess(::Type{BatchJobVector}) = true
Tables.rows(v::BatchJobVector) = v
Tables.istable(::Type{BatchJobVectorAllocated}) = true
Tables.rowaccess(::Type{BatchJobVectorAllocated}) = true
Tables.rows(v::BatchJobVectorAllocated) = v

Tables.istable(::Type{BatchFileDescVector}) = true
Tables.rowaccess(::Type{BatchFileDescVector}) = true
Tables.rows(v::BatchFileDescVector) = v
Tables.istable(::Type{BatchFileDescVectorAllocated}) = true
Tables.rowaccess(::Type{BatchFileDescVectorAllocated}) = true
Tables.rows(v::BatchFileDescVectorAllocated) = v

Tables.istable(::Type{PublisherDetailVector}) = true
Tables.rowaccess(::Type{PublisherDetailVector}) = true
Tables.rows(v::PublisherDetailVector) = v
Tables.istable(::Type{PublisherDetailVectorAllocated}) = true
Tables.rowaccess(::Type{PublisherDetailVectorAllocated}) = true
Tables.rows(v::PublisherDetailVectorAllocated) = v

Tables.istable(::Type{DatasetConditionDetailVector}) = true
Tables.rowaccess(::Type{DatasetConditionDetailVector}) = true
Tables.rows(v::DatasetConditionDetailVector) = v
Tables.istable(::Type{DatasetConditionDetailVectorAllocated}) = true
Tables.rowaccess(::Type{DatasetConditionDetailVectorAllocated}) = true
Tables.rows(v::DatasetConditionDetailVectorAllocated) = v

# show methods for metadata structs
Base.show(io::IO, p::PublisherDetail) = print(io, "PublisherDetail(", publisher_id(p), ", \"", String(dataset(p)), "\", \"", String(venue(p)), "\")")
function Base.show(io::IO, d::DatasetConditionDetail)
    print(io, "DatasetConditionDetail(\"", String(date(d)), "\", ", condition(d), ")")
end
function Base.show(io::IO, d::DatasetRange)
    print(io, "DatasetRange(\"", String(start(d)), "\", \"", String(stop(d)), "\")")
end
Base.show(io::IO, u::UnitPricesForMode) = print(io, "UnitPricesForMode(", mode(u), ", ", length(unit_prices(u)), " prices)")

# Iteration for vectors
for T in (BatchJobVector, BatchJobVectorAllocated, BatchFileDescVector, BatchFileDescVectorAllocated,
          PublisherDetailVector, PublisherDetailVectorAllocated,
          UnitPricesForModeVector, UnitPricesForModeVectorAllocated,
          DatasetConditionDetailVector, DatasetConditionDetailVectorAllocated,
          SchemaDoublePairVector, SchemaDoublePairVectorAllocated)
    @eval Base.iterate(v::$T, state=1) = state > Base.length(v) ? nothing : (v[state], state + 1)
    @eval Base.getindex(v::$T, i::Integer) = get_item(v, i - 1)
    @eval Base.length(v::$T) = Int(map_size(v))
    
    if T <: Union{PublisherDetailVector, PublisherDetailVectorAllocated}
        @eval Base.eltype(::Type{$T}) = PublisherDetail
    elseif T <: Union{UnitPricesForModeVector, UnitPricesForModeVectorAllocated}
        @eval Base.eltype(::Type{$T}) = UnitPricesForMode
    elseif T <: Union{DatasetConditionDetailVector, DatasetConditionDetailVectorAllocated}
        @eval Base.eltype(::Type{$T}) = DatasetConditionDetail
    elseif T <: Union{SchemaDoublePairVector, SchemaDoublePairVectorAllocated}
        @eval Base.eltype(::Type{$T}) = SchemaDoublePair
    elseif T <: Union{BatchJobVector, BatchJobVectorAllocated}
        @eval Base.eltype(::Type{$T}) = BatchJob
    else
        @eval Base.eltype(::Type{$T}) = BatchFileDesc
    end
end

const BATCH_JOB_COLUMNS = (:id, :state, :dataset, :schema, :cost_usd, :record_count, :billed_size, :ts_received, :ts_process_done)
const BATCH_FILE_COLUMNS = (:filename, :size, :hash, :https_url)
const PUBLISHER_COLUMNS = (:publisher_id, :dataset, :venue, :description)
const CONDITION_COLUMNS = (:date, :condition, :last_modified_date)

Tables.columnnames(::PublisherDetail) = PUBLISHER_COLUMNS
function Tables.getcolumn(p::PublisherDetail, nm::Symbol)
    if nm == :publisher_id; return publisher_id(p)
    elseif nm == :dataset; return String(dataset(p))
    elseif nm == :venue; return String(venue(p))
    elseif nm == :description; return String(description(p))
    end
    return nothing
end

Tables.columnnames(::DatasetConditionDetail) = CONDITION_COLUMNS
function Tables.getcolumn(d::DatasetConditionDetail, nm::Symbol)
    if nm == :date; return String(date(d))
    elseif nm == :condition; return condition(d)
    elseif nm == :last_modified_date; return String(last_modified_date(d))
    end
    return nothing
end

Tables.columnnames(::BatchJob) = BATCH_JOB_COLUMNS
function Tables.getcolumn(j::BatchJob, nm::Symbol)
    if nm == :id; return String(id(j))
    elseif nm == :state; return state(j)
    elseif nm == :dataset; return String(dataset(j))
    elseif nm == :schema; return schema(j)
    elseif nm == :cost_usd; return cost_usd(j)
    elseif nm == :record_count; return record_count(j)
    elseif nm == :billed_size; return billed_size(j)
    elseif nm == :ts_received; return String(ts_received(j))
    elseif nm == :ts_process_done; return String(ts_process_done(j))
    end
    return nothing
end

Tables.columnnames(::BatchFileDesc) = BATCH_FILE_COLUMNS
function Tables.getcolumn(f::BatchFileDesc, nm::Symbol)
    if nm == :filename; return String(filename(f))
    elseif nm == :size; return size(f)
    elseif nm == :hash; return String(hash(f))
    elseif nm == :https_url; return String(https_url(f))
    end
    return nothing
end

const TRADE_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :price, :size, :action, :side, :flags, :depth, :sequence, :bid_px, :ask_px)
const MBO_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :order_id, :price, :size, :action, :side, :flags, :channel_id, :sequence)
const OHLCV_COLUMNS = (:ts_event, :instrument_id, :publisher_id, :open, :high, :low, :close, :volume)
const DEF_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :raw_symbol, :exchange, :asset, :security_type, :instrument_class, :expiration, :min_price_increment)
const IMBALANCE_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :ref_price, :auction_time, :paired_qty, :total_imbalance_qty, :market_imbalance_qty, :unpaired_qty, :side)
const STATUS_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :action, :reason, :trading_event, :is_trading, :is_quoting, :is_short_sell_restricted)
const STAT_COLUMNS = (:ts_event, :ts_recv, :ts_ref, :instrument_id, :publisher_id, :price, :quantity, :stat_type, :update_action, :sequence)
const BBO_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :price, :size, :side, :flags, :sequence, :bid_px, :ask_px)
const CBBO_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :price, :size, :side, :flags, :bid_px, :ask_px, :bid_pb, :ask_pb)

function Tables.columnnames(r::Record)
    rt = rtype(r)
    if rt == RTYPE_MBP0 || rt == RTYPE_MBP1 || rt == RTYPE_MBP10
        return TRADE_COLUMNS
    elseif rt == RTYPE_MBO
        return MBO_COLUMNS
    elseif rt == RTYPE_INSTRUMENT_DEF
        return DEF_COLUMNS
    elseif rt == RTYPE_IMBALANCE
        return IMBALANCE_COLUMNS
    elseif rt == RTYPE_STATUS
        return STATUS_COLUMNS
    elseif rt == RTYPE_STATISTICS
        return STAT_COLUMNS
    elseif rt == RTYPE_BBO_1S || rt == RTYPE_BBO_1M
        return BBO_COLUMNS
    elseif rt == RTYPE_CBBO_1S || rt == RTYPE_CBBO_1M
        return CBBO_COLUMNS
    elseif rt == RTYPE_CMBP1 || rt == RTYPE_TCBBO
        return CBBO_COLUMNS # Consolidated MBP uses same columns as CBBO
    elseif rt == RTYPE_OHLCV_1S || rt == RTYPE_OHLCV_1M || rt == RTYPE_OHLCV_1H || rt == RTYPE_OHLCV_1D
        return OHLCV_COLUMNS
    end
    # Fallback to header fields
    return (:ts_event, :instrument_id, :publisher_id, :rtype)
end

function Tables.getcolumn(r::Record, nm::Symbol)
    h = header(r)
    if nm == :ts_event
        return time_since_epoch(ts_event(h))
    elseif nm == :instrument_id
        return instrument_id(h)
    elseif nm == :publisher_id
        return publisher_id(h)
    elseif nm == :rtype
        return rtype(h)
    end

    rt = rtype(r)
    
    # MBP and Trades
    if rt == RTYPE_MBP0 || rt == RTYPE_MBP1 || rt == RTYPE_MBP10
        msg_ptr = if rt == RTYPE_MBP0
            get_trade_if(r)
        elseif rt == RTYPE_MBP1
            get_mbp1_if(r)
        else
            get_mbp10_if(r)
        end

        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :price; return price(msg)
            elseif nm == :size; return size(msg)
            elseif nm == :action; return action(msg)
            elseif nm == :side; return side(msg)
            elseif nm == :flags; return flags(msg)
            elseif nm == :depth; return depth(msg)
            elseif nm == :sequence; return sequence(msg)
            elseif nm == :bid_px || nm == :ask_px
                if rt == RTYPE_MBP1 || rt == RTYPE_MBP10
                    # Level 0 is the top of book
                    l = level(msg, 0)
                    return nm == :bid_px ? bid_px(l) : ask_px(l)
                end
            end
        end
    
    # MBO
    elseif rt == RTYPE_MBO
        msg_ptr = get_mbo_if(r)
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :order_id; return order_id(msg)
            elseif nm == :price; return price(msg)
            elseif nm == :size; return size(msg)
            elseif nm == :action; return action(msg)
            elseif nm == :side; return side(msg)
            elseif nm == :flags; return flags(msg)
            elseif nm == :channel_id; return channel_id(msg)
            elseif nm == :sequence; return sequence(msg)
            end
        end

    # Definition
    elseif rt == RTYPE_INSTRUMENT_DEF
        msg_ptr = get_instrument_def_if(r)
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :raw_symbol; return String(raw_symbol(msg))
            elseif nm == :exchange; return String(exchange(msg))
            elseif nm == :asset; return String(asset(msg))
            elseif nm == :security_type; return String(security_type(msg))
            elseif nm == :instrument_class; return instrument_class(msg)
            elseif nm == :expiration; return time_since_epoch(expiration(msg))
            elseif nm == :min_price_increment; return min_price_increment(msg)
            end
        end
    
    # Imbalance
    elseif rt == RTYPE_IMBALANCE
        msg_ptr = get_imbalance_if(r)
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :ref_price; return ref_price(msg)
            elseif nm == :auction_time; return time_since_epoch(auction_time(msg))
            elseif nm == :paired_qty; return paired_qty(msg)
            elseif nm == :total_imbalance_qty; return total_imbalance_qty(msg)
            elseif nm == :market_imbalance_qty; return market_imbalance_qty(msg)
            elseif nm == :unpaired_qty; return unpaired_qty(msg)
            elseif nm == :side; return side(msg)
            end
        end

    # Status
    elseif rt == RTYPE_STATUS
        msg_ptr = get_status_if(r)
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :action; return action(msg)
            elseif nm == :reason; return reason(msg)
            elseif nm == :trading_event; return trading_event(msg)
            elseif nm == :is_trading; return is_trading(msg)
            elseif nm == :is_quoting; return is_quoting(msg)
            elseif nm == :is_short_sell_restricted; return is_short_sell_restricted(msg)
            end
        end

    # Statistics
    elseif rt == RTYPE_STATISTICS
        msg_ptr = get_stat_if(r)
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :ts_ref; return time_since_epoch(ts_ref(msg))
            elseif nm == :price; return price(msg)
            elseif nm == :quantity; return quantity(msg)
            elseif nm == :stat_type; return stat_type(msg)
            elseif nm == :update_action; return update_action(msg)
            elseif nm == :sequence; return sequence(msg)
            end
        end

    # BBO
    elseif rt == RTYPE_BBO_1S || rt == RTYPE_BBO_1M
        msg_ptr = get_bbo_if(r)
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :price; return price(msg)
            elseif nm == :size; return size(msg)
            elseif nm == :side; return side(msg)
            elseif nm == :flags; return flags(msg)
            elseif nm == :sequence; return sequence(msg)
            elseif nm == :bid_px || nm == :ask_px
                l = level(msg, 0)
                return nm == :bid_px ? bid_px(l) : ask_px(l)
            end
        end

    # CBBO and CMBP1
    elseif rt == RTYPE_CBBO_1S || rt == RTYPE_CBBO_1M || rt == RTYPE_CMBP1 || rt == RTYPE_TCBBO
        msg_ptr = if rt == RTYPE_CMBP1 || rt == RTYPE_TCBBO
            get_cmbp1_if(r)
        else
            get_cbbo_if(r)
        end
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :ts_recv; return time_since_epoch(ts_recv(msg))
            elseif nm == :price; return price(msg)
            elseif nm == :size; return size(msg)
            elseif nm == :side; return side(msg)
            elseif nm == :flags; return flags(msg)
            elseif nm == :bid_px || nm == :ask_px || nm == :bid_pb || nm == :ask_pb
                l = level(msg, 0)
                if nm == :bid_px; return bid_px(l)
                elseif nm == :ask_px; return ask_px(l)
                elseif nm == :bid_pb; return bid_pb(l)
                elseif nm == :ask_pb; return ask_pb(l)
                end
            end
        end

    # OHLCV
    elseif rt == RTYPE_OHLCV_1S || rt == RTYPE_OHLCV_1M || rt == RTYPE_OHLCV_1H || rt == RTYPE_OHLCV_1D
        msg_ptr = get_ohlcv_if(r)
        if msg_ptr != C_NULL
            msg = msg_ptr[]
            if nm == :open; return open(msg)
            elseif nm == :high; return high(msg)
            elseif nm == :low; return low(msg)
            elseif nm == :close; return close(msg)
            elseif nm == :volume; return volume(msg)
            end
        end
    end
    
    return nothing
end

# ============================================================================
# Iteration Interface
# ============================================================================



Base.eltype(::Type{DbnFileStore}) = Record
Base.IteratorSize(::Type{<:DbnFileStore}) = Base.SizeUnknown()

function Base.iterate(store::DbnFileStore, state=nothing)
    record_ptr = next_record(store)
    if record_ptr == C_NULL
        return nothing
    end
    # Dereference the pointer to get the Record object
    return (record_ptr[], nothing)
end

end # module
