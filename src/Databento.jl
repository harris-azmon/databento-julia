module Databento

using CxxWrap

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
export Schema, Encoding, SType, Dataset

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


end # module
