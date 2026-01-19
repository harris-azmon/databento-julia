#include <jlcxx/jlcxx.hpp>
#include <databento/enums.hpp>
#include <databento/publishers.hpp>
#include <sstream>
#include <string>

namespace jlcxx
{
  // Enable automatic Julia type conversion for databento enums
  template<> struct IsBits<databento::Schema> : std::true_type {};
  template<> struct IsBits<databento::Encoding> : std::true_type {};
  template<> struct IsBits<databento::SType> : std::true_type {};
  template<> struct IsBits<databento::Dataset> : std::true_type {};
}

JLCXX_MODULE define_databento_module(jlcxx::Module& mod)
{
  // ============================================================================
  // PHASE 1: Core Enums
  // ============================================================================

  // Schema Enum - Market data schemas
  mod.add_bits<databento::Schema>("Schema", jlcxx::julia_type("CppEnum"));
  mod.set_const("MBO", databento::Schema::Mbo);
  mod.set_const("MBP1", databento::Schema::Mbp1);
  mod.set_const("MBP10", databento::Schema::Mbp10);
  mod.set_const("TBBO", databento::Schema::Tbbo);
  mod.set_const("TRADES", databento::Schema::Trades);
  mod.set_const("OHLCV_1S", databento::Schema::Ohlcv1S);
  mod.set_const("OHLCV_1M", databento::Schema::Ohlcv1M);
  mod.set_const("OHLCV_1H", databento::Schema::Ohlcv1H);
  mod.set_const("OHLCV_1D", databento::Schema::Ohlcv1D);
  mod.set_const("DEFINITION", databento::Schema::Definition);
  mod.set_const("STATISTICS", databento::Schema::Statistics);
  mod.set_const("STATUS", databento::Schema::Status);
  mod.set_const("IMBALANCE", databento::Schema::Imbalance);
  mod.set_const("OHLCV_EOD", databento::Schema::OhlcvEod);
  mod.set_const("CMBP1", databento::Schema::Cmbp1);
  mod.set_const("CBBO_1S", databento::Schema::Cbbo1S);
  mod.set_const("CBBO_1M", databento::Schema::Cbbo1M);
  mod.set_const("TCBBO", databento::Schema::Tcbbo);
  mod.set_const("BBO_1S", databento::Schema::Bbo1S);
  mod.set_const("BBO_1M", databento::Schema::Bbo1M);

  // Schema ToString function
  mod.method("string", [](databento::Schema s) -> std::string {
    return std::string(databento::ToString(s));
  });

  // Encoding Enum - Data encoding formats
  mod.add_bits<databento::Encoding>("Encoding", jlcxx::julia_type("CppEnum"));
  mod.set_const("DBN", databento::Encoding::Dbn);
  mod.set_const("CSV", databento::Encoding::Csv);
  mod.set_const("JSON", databento::Encoding::Json);

  // Encoding ToString function
  mod.method("string", [](databento::Encoding e) -> std::string {
    return std::string(databento::ToString(e));
  });

  // SType Enum - Symbology types
  mod.add_bits<databento::SType>("SType", jlcxx::julia_type("CppEnum"));
  mod.set_const("INSTRUMENT_ID", databento::SType::InstrumentId);
  mod.set_const("RAW_SYMBOL", databento::SType::RawSymbol);
  mod.set_const("SMART", databento::SType::Smart);
  mod.set_const("CONTINUOUS", databento::SType::Continuous);
  mod.set_const("PARENT", databento::SType::Parent);
  mod.set_const("NASDAQ_SYMBOL", databento::SType::NasdaqSymbol);
  mod.set_const("CMS_SYMBOL", databento::SType::CmsSymbol);
  mod.set_const("ISIN", databento::SType::Isin);
  mod.set_const("US_CODE", databento::SType::UsCode);
  mod.set_const("BBG_COMP_ID", databento::SType::BbgCompId);
  mod.set_const("BBG_COMP_TICKER", databento::SType::BbgCompTicker);
  mod.set_const("FIGI", databento::SType::Figi);
  mod.set_const("FIGI_TICKER", databento::SType::FigiTicker);

  // SType ToString function
  mod.method("string", [](databento::SType s) -> std::string {
    return std::string(databento::ToString(s));
  });

  // Dataset Enum - Data vendors and feeds
  mod.add_bits<databento::Dataset>("Dataset", jlcxx::julia_type("CppEnum"));
  mod.set_const("GLBX_MDP3", databento::Dataset::GlbxMdp3);
  mod.set_const("XNAS_ITCH", databento::Dataset::XnasItch);
  mod.set_const("XBOS_ITCH", databento::Dataset::XbosItch);
  mod.set_const("XPSX_ITCH", databento::Dataset::XpsxItch);
  mod.set_const("BATS_PITCH", databento::Dataset::BatsPitch);
  mod.set_const("BATY_PITCH", databento::Dataset::BatyPitch);
  mod.set_const("EDGA_PITCH", databento::Dataset::EdgaPitch);
  mod.set_const("EDGX_PITCH", databento::Dataset::EdgxPitch);
  mod.set_const("XNYS_PILLAR", databento::Dataset::XnysPillar);
  mod.set_const("XCIS_PILLAR", databento::Dataset::XcisPillar);
  mod.set_const("XASE_PILLAR", databento::Dataset::XasePillar);
  mod.set_const("XCHI_PILLAR", databento::Dataset::XchiPillar);
  mod.set_const("XCIS_BBO", databento::Dataset::XcisBbo);
  mod.set_const("XCIS_TRADES", databento::Dataset::XcisTrades);
  mod.set_const("MEMX_MEMOIR", databento::Dataset::MemxMemoir);
  mod.set_const("EPRL_DOM", databento::Dataset::EprlDom);
  mod.set_const("OPRA_PILLAR", databento::Dataset::OpraPillar);
  mod.set_const("DBEQ_BASIC", databento::Dataset::DbeqBasic);
  mod.set_const("ARCX_PILLAR", databento::Dataset::ArcxPillar);
  mod.set_const("IEXG_TOPS", databento::Dataset::IexgTops);
  mod.set_const("EQUS_PLUS", databento::Dataset::EqusPlus);
  mod.set_const("XNYS_BBO", databento::Dataset::XnysBbo);
  mod.set_const("XNYS_TRADES", databento::Dataset::XnysTrades);
  mod.set_const("XNAS_QBBO", databento::Dataset::XnasQbbo);
  mod.set_const("XNAS_NLS", databento::Dataset::XnasNls);
  mod.set_const("IFEU_IMPACT", databento::Dataset::IfeuImpact);
  mod.set_const("NDEX_IMPACT", databento::Dataset::NdexImpact);
  mod.set_const("EQUS_ALL", databento::Dataset::EqusAll);
  mod.set_const("XNAS_BASIC", databento::Dataset::XnasBasic);
  mod.set_const("EQUS_SUMMARY", databento::Dataset::EqusSummary);
  mod.set_const("XCIS_TRADESBBO", databento::Dataset::XcisTradesbbo);
  mod.set_const("XNYS_TRADESBBO", databento::Dataset::XnysTradesbbo);
  mod.set_const("EQUS_MINI", databento::Dataset::EqusMini);
  mod.set_const("IFUS_IMPACT", databento::Dataset::IfusImpact);
  mod.set_const("IFLL_IMPACT", databento::Dataset::IfllImpact);
  mod.set_const("XEUR_EOBI", databento::Dataset::XeurEobi);
  mod.set_const("XEEE_EOBI", databento::Dataset::XeeeEobi);
  mod.set_const("XCBF_PITCH", databento::Dataset::XcbfPitch);

  // Dataset ToString function
  mod.method("string", [](databento::Dataset d) -> std::string {
    return std::string(databento::ToString(d));
  });
}
