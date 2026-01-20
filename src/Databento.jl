module Databento

using CxxWrap

# Load the C++ extension library
const depsfile = joinpath(@__DIR__, "..", "deps", "deps.jl")
if !isfile(depsfile)
    error("Databento C++ library not properly installed. Please run Pkg.build(\"Databento\").")
end
include(depsfile)

# Wrap the C++ module
# Specify the function name explicitly to match the C++ JLCXX_MODULE definition
@wrapmodule(() -> libdatabento_jl, :define_databento_module)

function __init__()
    @initcxx
end

# ============================================================================
# PHASE 1: Core Enums
# ============================================================================

# Export enum types
export Schema, Encoding, SType, RType, Action, Side, Dataset

# Export Schema constants
export MBO, MBP1, MBP10, TBBO, TRADES
export OHLCV_1S, OHLCV_1M, OHLCV_1H, OHLCV_1D, OHLCV_EOD
export DEFINITION, STATISTICS, STATUS, IMBALANCE
export CMBP1, CBBO_1S, CBBO_1M, TCBBO, BBO_1S, BBO_1M

# Export Encoding constants
export DBN, CSV, JSON

# Export SType constants
export INSTRUMENT_ID, RAW_SYMBOL, SMART, CONTINUOUS, PARENT
export NASDAQ_SYMBOL, CMS_SYMBOL, ISIN, US_CODE
export BBG_COMP_ID, BBG_COMP_TICKER, FIGI, FIGI_TICKER

# Export RType constants
export RTYPE_MBP0, RTYPE_MBP1, RTYPE_MBP10
export RTYPE_OHLCV_1S, RTYPE_OHLCV_1M, RTYPE_OHLCV_1H, RTYPE_OHLCV_1D
export RTYPE_STATUS, RTYPE_INSTRUMENT_DEF, RTYPE_IMBALANCE, RTYPE_ERROR
export RTYPE_SYMBOL_MAPPING, RTYPE_SYSTEM, RTYPE_STATISTICS
export RTYPE_MBO, RTYPE_CMBP1, RTYPE_CBBO_1S, RTYPE_CBBO_1M
export RTYPE_TCBBO, RTYPE_BBO_1S, RTYPE_BBO_1M

# Export Action constants
export ACTION_MODIFY, ACTION_TRADE, ACTION_FILL, ACTION_CANCEL
export ACTION_ADD, ACTION_CLEAR, ACTION_NONE

# Export Side constants
export SIDE_ASK, SIDE_BID, SIDE_NONE

# Export Dataset constants
export GLBX_MDP3, XNAS_ITCH, XBOS_ITCH, XPSX_ITCH
export BATS_PITCH, BATY_PITCH, EDGA_PITCH, EDGX_PITCH
export XNYS_PILLAR, XCIS_PILLAR, XASE_PILLAR, XCHI_PILLAR
export XCIS_BBO, XCIS_TRADES, MEMX_MEMOIR, EPRL_DOM
export OPRA_PILLAR, DBEQ_BASIC, ARCX_PILLAR, IEXG_TOPS
export EQUS_PLUS, XNYS_BBO, XNYS_TRADES, XNAS_QBBO, XNAS_NLS
export IFEU_IMPACT, NDEX_IMPACT, EQUS_ALL, XNAS_BASIC
export EQUS_SUMMARY, XCIS_TRADESBBO, XNYS_TRADESBBO, EQUS_MINI
export IFUS_IMPACT, IFLL_IMPACT, XEUR_EOBI, XEEE_EOBI, XCBF_PITCH

# ============================================================================
# PHASE 2: Message Types
# ============================================================================

# Export message types
export MboMsg, TradeMsg

end # module
