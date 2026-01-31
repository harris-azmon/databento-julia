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
export Schema, Encoding, SType, Dataset, Publisher

# Export Schema constants
export MBO, MBP1, MBP10, TBBO, TRADES
export OHLCV_1S, OHLCV_1M, OHLCV_1H, OHLCV_1D
export DEFINITION, STATISTICS, STATUS, IMBALANCE
export CMBP1, CBBO_1S, CBBO_1M, TCBBO, BBO_1S, BBO_1M

# Export Encoding constants
export DBN, CSV, JSON

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
export MboMsg, TradeMsg, Mbp1Msg, Mbp10Msg, InstrumentDefMsg, ImbalanceMsg
export Historical, HistoricalBuilder

# Export Methods
export next_record, get_metadata, header, rtype
export get_mbo_if, get_trade_if, get_mbp1_if, get_mbp10_if, get_imbalance_if, get_instrument_def_if, get_ohlcv_if
export set_key!, set_key_from_env!, build
export metadata_list_datasets, metadata_list_schemas, metadata_list_fields
export symbology_resolve, timeseries_get_range_to_file

# Fix ambiguity for enum conversions
Base.convert(::Type{Schema}, x::Schema) = x
Base.convert(::Type{Encoding}, x::Encoding) = x
Base.convert(::Type{SType}, x::SType) = x
Base.convert(::Type{Dataset}, x::Dataset) = x
Base.convert(::Type{Publisher}, x::Publisher) = x
Base.convert(::Type{RType}, x::RType) = x
Base.convert(::Type{Action}, x::Action) = x
Base.convert(::Type{Side}, x::Side) = x
Base.convert(::Type{InstrumentClass}, x::InstrumentClass) = x

# Export to_string for enums
export to_string
to_string(d::Dataset) = to_string_dataset(d)
to_string(p::Publisher) = to_string_publisher(p)
to_string(i::InstrumentClass) = to_string_instrument_class(i)

# Show methods for better REPL display
Base.show(io::IO, s::Schema) = print(io, "Schema::", to_string(s))
Base.show(io::IO, e::Encoding) = print(io, "Encoding::", to_string(e))
Base.show(io::IO, s::SType) = print(io, "SType::", to_string(s))
Base.show(io::IO, d::Dataset) = print(io, "Dataset::", to_string(d))
Base.show(io::IO, p::Publisher) = print(io, "Publisher::", to_string(p))
Base.show(io::IO, i::InstrumentClass) = print(io, "InstrumentClass::", to_string(i))

# ============================================================================ 
# Tables.jl Integration
# ============================================================================ 

Tables.istable(::Type{<:DbnFileStore}) = true
Tables.rowaccess(::Type{<:DbnFileStore}) = true
Tables.rows(store::DbnFileStore) = store

# Record as an AbstractRow
Tables.istable(::Type{<:Record}) = true # A record can be seen as a single row table

const TRADE_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :price, :size, :action, :side, :flags, :depth, :sequence)
const MBO_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :order_id, :price, :size, :action, :side, :flags, :channel_id, :sequence)
const OHLCV_COLUMNS = (:ts_event, :instrument_id, :publisher_id, :open, :high, :low, :close, :volume)
const DEF_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :raw_symbol, :exchange, :asset, :security_type, :instrument_class, :expiration, :min_price_increment)
const IMBALANCE_COLUMNS = (:ts_event, :ts_recv, :instrument_id, :publisher_id, :ref_price, :auction_time, :paired_qty, :total_imbalance_qty, :market_imbalance_qty, :unpaired_qty, :side)

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
        msg_ptr = get_trade_if(r)
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
