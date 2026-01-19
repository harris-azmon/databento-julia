#include <jlcxx/jlcxx.hpp>
#include <databento/enums.hpp>
#include <databento/publishers.hpp>
#include <databento/record.hpp>
#include <databento/datetime.hpp>
#include <databento/flag_set.hpp>
#include <databento/historical.hpp>
#include <databento/dbn_file_store.hpp>
#include <databento/dbn.hpp>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

namespace jlcxx
{
  // Enable automatic Julia type conversion for databento enums
  template<> struct IsBits<databento::Schema> : std::true_type {};
  template<> struct IsBits<databento::Encoding> : std::true_type {};
  template<> struct IsBits<databento::SType> : std::true_type {};
  template<> struct IsBits<databento::Dataset> : std::true_type {};

  // Phase 2: Additional enums
  template<> struct IsBits<databento::RType> : std::true_type {};
  template<> struct IsBits<databento::Action> : std::true_type {};
  template<> struct IsBits<databento::Side> : std::true_type {};

  // Phase 2: Supporting types
  template<> struct IsBits<databento::FlagSet> : std::true_type {};
  template<> struct IsBits<databento::UnixNanos> : std::true_type {};
  template<> struct IsBits<databento::TimeDeltaNanos> : std::true_type {};
  template<> struct IsBits<databento::BidAskPair> : std::true_type {};

  // Phase 2: Record structures
  template<> struct IsBits<databento::RecordHeader> : std::true_type {};
  template<> struct IsBits<databento::MboMsg> : std::true_type {};
  template<> struct IsBits<databento::TradeMsg> : std::true_type {};
  template<> struct IsBits<databento::Mbp1Msg> : std::true_type {};
  template<> struct IsBits<databento::Mbp10Msg> : std::true_type {};
  template<> struct IsBits<databento::InstrumentDefMsg> : std::true_type {};
  template<> struct IsBits<databento::ImbalanceMsg> : std::true_type {};
  template<> struct IsBits<databento::StatusMsg> : std::true_type {};
  template<> struct IsBits<databento::OhlcvMsg> : std::true_type {};
  template<> struct IsBits<databento::StatMsg> : std::true_type {};
  template<> struct IsBits<databento::ErrorMsg> : std::true_type {};
  template<> struct IsBits<databento::SymbolMappingMsg> : std::true_type {};
  template<> struct IsBits<databento::SystemMsg> : std::true_type {};
  template<> struct IsBits<databento::BboMsg> : std::true_type {};
  template<> struct IsBits<databento::Cmbp1Msg> : std::true_type {};
  template<> struct IsBits<databento::CbboMsg> : std::true_type {};
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

  // ============================================================================
  // PHASE 2: Data Record Protocol
  // ============================================================================

  // RType Enum - Record type identifiers
  mod.add_bits<databento::RType>("RType", jlcxx::julia_type("CppEnum"));
  mod.set_const("RTYPE_MBP0", databento::RType::Mbp0);
  mod.set_const("RTYPE_MBP1", databento::RType::Mbp1);
  mod.set_const("RTYPE_MBP10", databento::RType::Mbp10);
  mod.set_const("RTYPE_OHLCV_DEPRECATED", databento::RType::OhlcvDeprecated);
  mod.set_const("RTYPE_OHLCV_1S", databento::RType::Ohlcv1S);
  mod.set_const("RTYPE_OHLCV_1M", databento::RType::Ohlcv1M);
  mod.set_const("RTYPE_OHLCV_1H", databento::RType::Ohlcv1H);
  mod.set_const("RTYPE_OHLCV_1D", databento::RType::Ohlcv1D);
  mod.set_const("RTYPE_OHLCV_EOD", databento::RType::OhlcvEod);
  mod.set_const("RTYPE_STATUS", databento::RType::Status);
  mod.set_const("RTYPE_INSTRUMENT_DEF", databento::RType::InstrumentDef);
  mod.set_const("RTYPE_IMBALANCE", databento::RType::Imbalance);
  mod.set_const("RTYPE_ERROR", databento::RType::Error);
  mod.set_const("RTYPE_SYMBOL_MAPPING", databento::RType::SymbolMapping);
  mod.set_const("RTYPE_SYSTEM", databento::RType::System);
  mod.set_const("RTYPE_STATISTICS", databento::RType::Statistics);
  mod.set_const("RTYPE_MBO", databento::RType::Mbo);
  mod.set_const("RTYPE_CMBP1", databento::RType::Cmbp1);
  mod.set_const("RTYPE_CBBO_1S", databento::RType::Cbbo1S);
  mod.set_const("RTYPE_CBBO_1M", databento::RType::Cbbo1M);
  mod.set_const("RTYPE_TCBBO", databento::RType::Tcbbo);
  mod.set_const("RTYPE_BBO_1S", databento::RType::Bbo1S);
  mod.set_const("RTYPE_BBO_1M", databento::RType::Bbo1M);

  // RType ToString function
  mod.method("string", [](databento::RType r) -> std::string {
    return std::string(databento::ToString(r));
  });

  // Action Enum - Market event actions
  mod.add_bits<databento::Action>("Action", jlcxx::julia_type("CppEnum"));
  mod.set_const("ACTION_MODIFY", databento::Action::Modify);
  mod.set_const("ACTION_TRADE", databento::Action::Trade);
  mod.set_const("ACTION_FILL", databento::Action::Fill);
  mod.set_const("ACTION_CANCEL", databento::Action::Cancel);
  mod.set_const("ACTION_ADD", databento::Action::Add);
  mod.set_const("ACTION_CLEAR", databento::Action::Clear);
  mod.set_const("ACTION_NONE", databento::Action::None);

  // Action ToString function
  mod.method("string", [](databento::Action a) -> std::string {
    return std::string(databento::ToString(a));
  });

  // Side Enum - Order side (bid/ask)
  mod.add_bits<databento::Side>("Side", jlcxx::julia_type("CppEnum"));
  mod.set_const("SIDE_ASK", databento::Side::Ask);
  mod.set_const("SIDE_BID", databento::Side::Bid);
  mod.set_const("SIDE_NONE", databento::Side::None);

  // Side ToString function
  mod.method("string", [](databento::Side s) -> std::string {
    return std::string(databento::ToString(s));
  });

  // FlagSet - Bit flags for records
  mod.add_bits<databento::FlagSet>("FlagSet", jlcxx::julia_type("IsBits"));
  mod.method("is_last", &databento::FlagSet::IsLast);
  mod.method("is_tob", &databento::FlagSet::IsTob);
  mod.method("is_snapshot", &databento::FlagSet::IsSnapshot);
  mod.method("is_mbp", &databento::FlagSet::IsMbp);
  mod.method("is_bad_ts_recv", &databento::FlagSet::IsBadTsRecv);
  mod.method("is_maybe_bad_book", &databento::FlagSet::IsMaybeBadBook);
  mod.method("is_publisher_specific", &databento::FlagSet::IsPublisherSpecific);
  mod.method("raw", &databento::FlagSet::Raw);
  mod.method("any", &databento::FlagSet::Any);
  mod.method("is_empty", &databento::FlagSet::IsEmpty);

  // UnixNanos - Timestamp type (nanoseconds since Unix epoch)
  mod.add_bits<databento::UnixNanos>("UnixNanos", jlcxx::julia_type("IsBits"));
  mod.method("time_since_epoch", [](const databento::UnixNanos& ts) -> std::int64_t {
    return ts.time_since_epoch().count();
  });

  // TimeDeltaNanos - Time delta in nanoseconds
  mod.add_bits<databento::TimeDeltaNanos>("TimeDeltaNanos", jlcxx::julia_type("IsBits"));
  mod.method("count", [](const databento::TimeDeltaNanos& td) -> std::int32_t {
    return td.count();
  });

  // BidAskPair - Price level data
  mod.add_bits<databento::BidAskPair>("BidAskPair", jlcxx::julia_type("IsBits"));
  mod.method("bid_px", [](const databento::BidAskPair& p) { return p.bid_px; });
  mod.method("ask_px", [](const databento::BidAskPair& p) { return p.ask_px; });
  mod.method("bid_sz", [](const databento::BidAskPair& p) { return p.bid_sz; });
  mod.method("ask_sz", [](const databento::BidAskPair& p) { return p.ask_sz; });
  mod.method("bid_ct", [](const databento::BidAskPair& p) { return p.bid_ct; });
  mod.method("ask_ct", [](const databento::BidAskPair& p) { return p.ask_ct; });

  // RecordHeader - Common header for all records
  mod.add_bits<databento::RecordHeader>("RecordHeader", jlcxx::julia_type("IsBits"));
  mod.method("length", [](const databento::RecordHeader& h) { return h.length; });
  mod.method("rtype", [](const databento::RecordHeader& h) { return h.rtype; });
  mod.method("publisher_id", [](const databento::RecordHeader& h) { return h.publisher_id; });
  mod.method("instrument_id", [](const databento::RecordHeader& h) { return h.instrument_id; });
  mod.method("ts_event", [](const databento::RecordHeader& h) { return h.ts_event; });
  mod.method("size", &databento::RecordHeader::Size);
  mod.method("publisher", &databento::RecordHeader::Publisher);

  // MboMsg - Market By Order message
  mod.add_bits<databento::MboMsg>("MboMsg", jlcxx::julia_type("IsBits"));
  mod.method("hd", [](const databento::MboMsg& m) { return m.hd; });
  mod.method("order_id", [](const databento::MboMsg& m) { return m.order_id; });
  mod.method("price", [](const databento::MboMsg& m) { return m.price; });
  mod.method("size", [](const databento::MboMsg& m) { return m.size; });
  mod.method("flags", [](const databento::MboMsg& m) { return m.flags; });
  mod.method("channel_id", [](const databento::MboMsg& m) { return m.channel_id; });
  mod.method("action", [](const databento::MboMsg& m) { return m.action; });
  mod.method("side", [](const databento::MboMsg& m) { return m.side; });
  mod.method("ts_recv", [](const databento::MboMsg& m) { return m.ts_recv; });
  mod.method("ts_in_delta", [](const databento::MboMsg& m) { return m.ts_in_delta; });
  mod.method("sequence", [](const databento::MboMsg& m) { return m.sequence; });
  mod.method("index_ts", &databento::MboMsg::IndexTs);
  mod.method("to_string", [](const databento::MboMsg& m) -> std::string {
    return databento::ToString(m);
  });

  // TradeMsg - Trade message
  mod.add_bits<databento::TradeMsg>("TradeMsg", jlcxx::julia_type("IsBits"));
  mod.method("hd", [](const databento::TradeMsg& m) { return m.hd; });
  mod.method("price", [](const databento::TradeMsg& m) { return m.price; });
  mod.method("size", [](const databento::TradeMsg& m) { return m.size; });
  mod.method("action", [](const databento::TradeMsg& m) { return m.action; });
  mod.method("side", [](const databento::TradeMsg& m) { return m.side; });
  mod.method("flags", [](const databento::TradeMsg& m) { return m.flags; });
  mod.method("depth", [](const databento::TradeMsg& m) { return m.depth; });
  mod.method("ts_recv", [](const databento::TradeMsg& m) { return m.ts_recv; });
  mod.method("ts_in_delta", [](const databento::TradeMsg& m) { return m.ts_in_delta; });
  mod.method("sequence", [](const databento::TradeMsg& m) { return m.sequence; });
  mod.method("index_ts", &databento::TradeMsg::IndexTs);
  mod.method("to_string", [](const databento::TradeMsg& m) -> std::string {
    return databento::ToString(m);
  });

  // Mbp1Msg - Market By Price Level 1 message
  mod.add_bits<databento::Mbp1Msg>("Mbp1Msg", jlcxx::julia_type("IsBits"));
  mod.method("hd", [](const databento::Mbp1Msg& m) { return m.hd; });
  mod.method("price", [](const databento::Mbp1Msg& m) { return m.price; });
  mod.method("size", [](const databento::Mbp1Msg& m) { return m.size; });
  mod.method("action", [](const databento::Mbp1Msg& m) { return m.action; });
  mod.method("side", [](const databento::Mbp1Msg& m) { return m.side; });
  mod.method("flags", [](const databento::Mbp1Msg& m) { return m.flags; });
  mod.method("depth", [](const databento::Mbp1Msg& m) { return m.depth; });
  mod.method("ts_recv", [](const databento::Mbp1Msg& m) { return m.ts_recv; });
  mod.method("ts_in_delta", [](const databento::Mbp1Msg& m) { return m.ts_in_delta; });
  mod.method("sequence", [](const databento::Mbp1Msg& m) { return m.sequence; });
  mod.method("levels", [](const databento::Mbp1Msg& m) { return m.levels; });
  mod.method("index_ts", &databento::Mbp1Msg::IndexTs);
  mod.method("to_string", [](const databento::Mbp1Msg& m) -> std::string {
    return databento::ToString(m);
  });

  // Mbp10Msg - Market By Price Level 10 message
  mod.add_bits<databento::Mbp10Msg>("Mbp10Msg", jlcxx::julia_type("IsBits"));
  mod.method("hd", [](const databento::Mbp10Msg& m) { return m.hd; });
  mod.method("price", [](const databento::Mbp10Msg& m) { return m.price; });
  mod.method("size", [](const databento::Mbp10Msg& m) { return m.size; });
  mod.method("action", [](const databento::Mbp10Msg& m) { return m.action; });
  mod.method("side", [](const databento::Mbp10Msg& m) { return m.side; });
  mod.method("flags", [](const databento::Mbp10Msg& m) { return m.flags; });
  mod.method("depth", [](const databento::Mbp10Msg& m) { return m.depth; });
  mod.method("ts_recv", [](const databento::Mbp10Msg& m) { return m.ts_recv; });
  mod.method("ts_in_delta", [](const databento::Mbp10Msg& m) { return m.ts_in_delta; });
  mod.method("sequence", [](const databento::Mbp10Msg& m) { return m.sequence; });
  mod.method("levels", [](const databento::Mbp10Msg& m) { return m.levels; });
  mod.method("index_ts", &databento::Mbp10Msg::IndexTs);
  mod.method("to_string", [](const databento::Mbp10Msg& m) -> std::string {
    return databento::ToString(m);
  });

  // InstrumentDefMsg - Instrument definition message
  mod.add_bits<databento::InstrumentDefMsg>("InstrumentDefMsg", jlcxx::julia_type("IsBits"));
  mod.method("hd", [](const databento::InstrumentDefMsg& m) { return m.hd; });
  mod.method("ts_recv", [](const databento::InstrumentDefMsg& m) { return m.ts_recv; });
  mod.method("min_price_increment", [](const databento::InstrumentDefMsg& m) { return m.min_price_increment; });
  mod.method("display_factor", [](const databento::InstrumentDefMsg& m) { return m.display_factor; });
  mod.method("expiration", [](const databento::InstrumentDefMsg& m) { return m.expiration; });
  mod.method("activation", [](const databento::InstrumentDefMsg& m) { return m.activation; });
  mod.method("high_limit_price", [](const databento::InstrumentDefMsg& m) { return m.high_limit_price; });
  mod.method("low_limit_price", [](const databento::InstrumentDefMsg& m) { return m.low_limit_price; });
  mod.method("max_price_variation", [](const databento::InstrumentDefMsg& m) { return m.max_price_variation; });
  mod.method("trading_reference_price", [](const databento::InstrumentDefMsg& m) { return m.trading_reference_price; });
  mod.method("unit_of_measure_qty", [](const databento::InstrumentDefMsg& m) { return m.unit_of_measure_qty; });
  mod.method("min_price_increment_amount", [](const databento::InstrumentDefMsg& m) { return m.min_price_increment_amount; });
  mod.method("price_ratio", [](const databento::InstrumentDefMsg& m) { return m.price_ratio; });
  mod.method("inst_attrib_value", [](const databento::InstrumentDefMsg& m) { return m.inst_attrib_value; });
  mod.method("underlying_id", [](const databento::InstrumentDefMsg& m) { return m.underlying_id; });
  mod.method("raw_instrument_id", [](const databento::InstrumentDefMsg& m) { return m.raw_instrument_id; });
  mod.method("market_depth_implied", [](const databento::InstrumentDefMsg& m) { return m.market_depth_implied; });
  mod.method("market_depth", [](const databento::InstrumentDefMsg& m) { return m.market_depth; });
  mod.method("market_segment_id", [](const databento::InstrumentDefMsg& m) { return m.market_segment_id; });
  mod.method("max_trade_vol", [](const databento::InstrumentDefMsg& m) { return m.max_trade_vol; });
  mod.method("min_lot_size", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size; });
  mod.method("min_lot_size_block", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size_block; });
  mod.method("min_lot_size_round_lot", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size_round_lot; });
  mod.method("min_trade_vol", [](const databento::InstrumentDefMsg& m) { return m.min_trade_vol; });
  mod.method("contract_multiplier", [](const databento::InstrumentDefMsg& m) { return m.contract_multiplier; });
  mod.method("decay_quantity", [](const databento::InstrumentDefMsg& m) { return m.decay_quantity; });
  mod.method("original_contract_size", [](const databento::InstrumentDefMsg& m) { return m.original_contract_size; });
  mod.method("trading_reference_date", [](const databento::InstrumentDefMsg& m) { return m.trading_reference_date; });
  mod.method("appl_id", [](const databento::InstrumentDefMsg& m) { return m.appl_id; });
  mod.method("maturity_year", [](const databento::InstrumentDefMsg& m) { return m.maturity_year; });
  mod.method("decay_start_date", [](const databento::InstrumentDefMsg& m) { return m.decay_start_date; });
  mod.method("channel_id", [](const databento::InstrumentDefMsg& m) { return m.channel_id; });
  mod.method("currency", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.Currency();
  });
  mod.method("settl_currency", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.SettlCurrency();
  });
  mod.method("secsubtype", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.SecSubType();
  });
  mod.method("raw_symbol", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.RawSymbol();
  });
  mod.method("group", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.Group();
  });
  mod.method("exchange", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.Exchange();
  });
  mod.method("asset", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.Asset();
  });
  mod.method("cfi", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.Cfi();
  });
  mod.method("security_type", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.SecurityType();
  });
  mod.method("unit_of_measure", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.UnitOfMeasure();
  });
  mod.method("underlying", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.Underlying();
  });
  mod.method("strike_price_currency", [](const databento::InstrumentDefMsg& m) -> std::string {
    return m.StrikePriceCurrency();
  });
  mod.method("instrument_class", [](const databento::InstrumentDefMsg& m) { return m.instrument_class; });
  mod.method("strike_price", [](const databento::InstrumentDefMsg& m) { return m.strike_price; });
  mod.method("match_algorithm", [](const databento::InstrumentDefMsg& m) { return m.match_algorithm; });
  mod.method("md_security_trading_status", [](const databento::InstrumentDefMsg& m) { return m.md_security_trading_status; });
  mod.method("main_fraction", [](const databento::InstrumentDefMsg& m) { return m.main_fraction; });
  mod.method("price_display_format", [](const databento::InstrumentDefMsg& m) { return m.price_display_format; });
  mod.method("settl_price_type", [](const databento::InstrumentDefMsg& m) { return m.settl_price_type; });
  mod.method("sub_fraction", [](const databento::InstrumentDefMsg& m) { return m.sub_fraction; });
  mod.method("underlying_product", [](const databento::InstrumentDefMsg& m) { return m.underlying_product; });
  mod.method("security_update_action", [](const databento::InstrumentDefMsg& m) { return m.security_update_action; });
  mod.method("maturity_month", [](const databento::InstrumentDefMsg& m) { return m.maturity_month; });
  mod.method("maturity_day", [](const databento::InstrumentDefMsg& m) { return m.maturity_day; });
  mod.method("maturity_week", [](const databento::InstrumentDefMsg& m) { return m.maturity_week; });
  mod.method("user_defined_instrument", [](const databento::InstrumentDefMsg& m) { return m.user_defined_instrument; });
  mod.method("contract_multiplier_unit", [](const databento::InstrumentDefMsg& m) { return m.contract_multiplier_unit; });
  mod.method("flow_schedule_type", [](const databento::InstrumentDefMsg& m) { return m.flow_schedule_type; });
  mod.method("tick_rule", [](const databento::InstrumentDefMsg& m) { return m.tick_rule; });
  mod.method("index_ts", &databento::InstrumentDefMsg::IndexTs);
  mod.method("to_string", [](const databento::InstrumentDefMsg& m) -> std::string {
    return databento::ToString(m);
  });

  // ImbalanceMsg - Imbalance message
  mod.add_bits<databento::ImbalanceMsg>("ImbalanceMsg", jlcxx::julia_type("IsBits"));
  mod.method("hd", [](const databento::ImbalanceMsg& m) { return m.hd; });
  mod.method("ts_recv", [](const databento::ImbalanceMsg& m) { return m.ts_recv; });
  mod.method("ref_price", [](const databento::ImbalanceMsg& m) { return m.ref_price; });
  mod.method("auction_time", [](const databento::ImbalanceMsg& m) { return m.auction_time; });
  mod.method("cont_book_clr_price", [](const databento::ImbalanceMsg& m) { return m.cont_book_clr_price; });
  mod.method("auct_interest_clr_price", [](const databento::ImbalanceMsg& m) { return m.auct_interest_clr_price; });
  mod.method("ssr_filling_price", [](const databento::ImbalanceMsg& m) { return m.ssr_filling_price; });
  mod.method("ind_match_price", [](const databento::ImbalanceMsg& m) { return m.ind_match_price; });
  mod.method("upper_collar", [](const databento::ImbalanceMsg& m) { return m.upper_collar; });
  mod.method("lower_collar", [](const databento::ImbalanceMsg& m) { return m.lower_collar; });
  mod.method("paired_qty", [](const databento::ImbalanceMsg& m) { return m.paired_qty; });
  mod.method("total_imbalance_qty", [](const databento::ImbalanceMsg& m) { return m.total_imbalance_qty; });
  mod.method("market_imbalance_qty", [](const databento::ImbalanceMsg& m) { return m.market_imbalance_qty; });
  mod.method("unpaired_qty", [](const databento::ImbalanceMsg& m) { return m.unpaired_qty; });
  mod.method("auction_type", [](const databento::ImbalanceMsg& m) { return m.auction_type; });
  mod.method("side", [](const databento::ImbalanceMsg& m) { return m.side; });
  mod.method("auction_status", [](const databento::ImbalanceMsg& m) { return m.auction_status; });
  mod.method("index_ts", &databento::ImbalanceMsg::IndexTs);
  mod.method("to_string", [](const databento::ImbalanceMsg& m) -> std::string {
    return databento::ToString(m);
  });

  // ============================================================================
  // PHASE 3: Historical Client
  // ============================================================================

  // Historical::Builder - Builder pattern for constructing Historical client
  mod.add_type<databento::HistoricalBuilder>("HistoricalBuilder")
    .constructor<>()
    .method("set_key!", [](databento::HistoricalBuilder& builder, const std::string& key) -> databento::HistoricalBuilder& {
      return builder.SetKey(key);
    })
    .method("set_key_from_env!", [](databento::HistoricalBuilder& builder) -> databento::HistoricalBuilder& {
      return builder.SetKeyFromEnv();
    })
    .method("build", [](databento::HistoricalBuilder& builder) -> databento::Historical {
      return builder.Build();
    });

  // Historical - Main client for historical data access
  mod.add_type<databento::Historical>("Historical")
    // Metadata methods
    .method("metadata_list_datasets", [](databento::Historical& client) -> std::vector<std::string> {
      return client.MetadataListDatasets();
    })
    .method("metadata_list_schemas", [](databento::Historical& client, const std::string& dataset) -> std::vector<databento::Schema> {
      return client.MetadataListSchemas(dataset);
    })
    .method("metadata_list_fields", [](databento::Historical& client, databento::Encoding encoding, databento::Schema schema) -> std::vector<std::string> {
      return client.MetadataListFields(encoding, schema);
    })
    // Symbology methods
    .method("symbology_resolve", [](databento::Historical& client,
                                     const std::string& dataset,
                                     const std::vector<std::string>& symbols,
                                     databento::SType stype_in,
                                     const std::string& date) -> std::string {
      return client.SymbologyResolve(dataset, symbols, stype_in, date);
    })
    // Data retrieval methods
    .method("timeseries_get_range_to_file", [](databento::Historical& client,
                                                const std::string& dataset,
                                                const std::vector<std::string>& symbols,
                                                databento::Schema schema,
                                                const std::string& start,
                                                const std::string& end,
                                                databento::SType stype_in,
                                                databento::SType stype_out,
                                                const std::string& output_file) -> std::string {
      return client.TimeseriesGetRangeToFile(dataset, symbols, schema, start, end, stype_in, stype_out, output_file);
    });

  // ============================================================================
  // PHASE 4: DBN Reader and Record Iteration
  // ============================================================================

  // Record - Universal record wrapper for type-safe access
  mod.add_type<databento::Record>("Record")
    .method("header", [](const databento::Record& r) -> const databento::RecordHeader& {
      return r.Header();
    })
    .method("rtype", [](const databento::Record& r) -> databento::RType {
      return r.RType();
    })
    .method("size", [](const databento::Record& r) -> std::size_t {
      return r.Size();
    })
    // Type checking methods
    .method("holds_mbo", [](const databento::Record& r) -> bool {
      return r.Holds<databento::MboMsg>();
    })
    .method("holds_trade", [](const databento::Record& r) -> bool {
      return r.Holds<databento::TradeMsg>();
    })
    .method("holds_mbp1", [](const databento::Record& r) -> bool {
      return r.Holds<databento::Mbp1Msg>();
    })
    .method("holds_mbp10", [](const databento::Record& r) -> bool {
      return r.Holds<databento::Mbp10Msg>();
    })
    .method("holds_ohlcv", [](const databento::Record& r) -> bool {
      return r.Holds<databento::OhlcvMsg>();
    })
    .method("holds_status", [](const databento::Record& r) -> bool {
      return r.Holds<databento::StatusMsg>();
    })
    .method("holds_instrument_def", [](const databento::Record& r) -> bool {
      return r.Holds<databento::InstrumentDefMsg>();
    })
    .method("holds_imbalance", [](const databento::Record& r) -> bool {
      return r.Holds<databento::ImbalanceMsg>();
    })
    .method("holds_stat", [](const databento::Record& r) -> bool {
      return r.Holds<databento::StatMsg>();
    })
    // Safe casting methods (returns pointer, nullptr if wrong type)
    .method("get_mbo_if", [](const databento::Record& r) -> const databento::MboMsg* {
      return r.GetIf<databento::MboMsg>();
    })
    .method("get_trade_if", [](const databento::Record& r) -> const databento::TradeMsg* {
      return r.GetIf<databento::TradeMsg>();
    })
    .method("get_mbp1_if", [](const databento::Record& r) -> const databento::Mbp1Msg* {
      return r.GetIf<databento::Mbp1Msg>();
    })
    .method("get_mbp10_if", [](const databento::Record& r) -> const databento::Mbp10Msg* {
      return r.GetIf<databento::Mbp10Msg>();
    })
    .method("get_ohlcv_if", [](const databento::Record& r) -> const databento::OhlcvMsg* {
      return r.GetIf<databento::OhlcvMsg>();
    })
    .method("get_status_if", [](const databento::Record& r) -> const databento::StatusMsg* {
      return r.GetIf<databento::StatusMsg>();
    })
    .method("get_instrument_def_if", [](const databento::Record& r) -> const databento::InstrumentDefMsg* {
      return r.GetIf<databento::InstrumentDefMsg>();
    })
    .method("get_imbalance_if", [](const databento::Record& r) -> const databento::ImbalanceMsg* {
      return r.GetIf<databento::ImbalanceMsg>();
    })
    .method("get_stat_if", [](const databento::Record& r) -> const databento::StatMsg* {
      return r.GetIf<databento::StatMsg>();
    });

  // Metadata - DBN file metadata structure
  mod.add_type<databento::Metadata>("Metadata")
    .method("version", [](const databento::Metadata& m) -> std::uint8_t {
      return m.version;
    })
    .method("dataset", [](const databento::Metadata& m) -> std::string {
      return m.dataset;
    })
    .method("has_mixed_schema", [](const databento::Metadata& m) -> bool {
      return m.has_mixed_schema;
    })
    .method("schema", [](const databento::Metadata& m) -> databento::Schema {
      return m.schema;
    })
    .method("start", [](const databento::Metadata& m) -> std::uint64_t {
      return m.start.time_since_epoch().count();
    })
    .method("end", [](const databento::Metadata& m) -> std::uint64_t {
      return m.end.time_since_epoch().count();
    })
    .method("limit", [](const databento::Metadata& m) -> std::uint64_t {
      return m.limit;
    })
    .method("stype_in", [](const databento::Metadata& m) -> databento::SType {
      return m.stype_in;
    })
    .method("stype_out", [](const databento::Metadata& m) -> databento::SType {
      return m.stype_out;
    })
    .method("ts_out", [](const databento::Metadata& m) -> bool {
      return m.ts_out;
    })
    .method("symbols", [](const databento::Metadata& m) -> std::vector<std::string> {
      return m.symbols;
    })
    .method("partial", [](const databento::Metadata& m) -> std::vector<std::string> {
      return m.partial;
    })
    .method("not_found", [](const databento::Metadata& m) -> std::vector<std::string> {
      return m.not_found;
    });

  // DbnFileStore - Main class for reading DBN files
  mod.add_type<databento::DbnFileStore>("DbnFileStore")
    .constructor<const std::string&>()
    .method("get_metadata", [](databento::DbnFileStore& store) -> const databento::Metadata& {
      return store.GetMetadata();
    })
    .method("next_record", [](databento::DbnFileStore& store) -> const databento::Record* {
      return store.NextRecord();
    });
}
