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

// Disable mirroring for types we want to wrap with add_type
namespace jlcxx {
  template<> struct IsMirroredType<databento::FlagSet> : std::false_type {};
  template<> struct IsMirroredType<databento::UnixNanos> : std::false_type {};
  template<> struct IsMirroredType<databento::TimeDeltaNanos> : std::false_type {};
  template<> struct IsMirroredType<databento::BidAskPair> : std::false_type {};
  template<> struct IsMirroredType<databento::RecordHeader> : std::false_type {};
  template<> struct IsMirroredType<databento::MboMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::TradeMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::Mbp1Msg> : std::false_type {};
  template<> struct IsMirroredType<databento::Mbp10Msg> : std::false_type {};
  template<> struct IsMirroredType<databento::InstrumentDefMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::ImbalanceMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::OhlcvMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::StatusMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::StatMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::ErrorMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::SymbolMappingMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::SystemMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::BboMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::Cmbp1Msg> : std::false_type {};
  template<> struct IsMirroredType<databento::CbboMsg> : std::false_type {};
  template<> struct IsMirroredType<databento::ConsolidatedBidAskPair> : std::false_type {};
}

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
{
  // ============================================================================
  // PRE-DECLARE TYPES
  // ============================================================================
  
  // Register enums first as they are often used in other types
  mod.add_bits<databento::Schema>("Schema", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::Encoding>("Encoding", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::SType>("SType", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::Dataset>("Dataset", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::Publisher>("Publisher", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::RType>("RType", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::Action>("Action", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::Side>("Side", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::InstrumentClass>("InstrumentClass", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::MatchAlgorithm>("MatchAlgorithm", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::StatusAction>("StatusAction", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::StatusReason>("StatusReason", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::TradingEvent>("TradingEvent", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::TriState>("TriState", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::StatType>("StatType", jlcxx::julia_type("CppEnum"));
  mod.add_bits<databento::StatUpdateAction>("StatUpdateAction", jlcxx::julia_type("CppEnum"));

  // Register structs and classes
  auto flag_set = mod.add_type<databento::FlagSet>("FlagSet");
  auto unix_nanos = mod.add_type<databento::UnixNanos>("UnixNanos");
  auto time_delta_nanos = mod.add_type<databento::TimeDeltaNanos>("TimeDeltaNanos");
  auto bid_ask_pair = mod.add_type<databento::BidAskPair>("BidAskPair");
  auto record_header = mod.add_type<databento::RecordHeader>("RecordHeader");
  auto mbo_msg = mod.add_type<databento::MboMsg>("MboMsg");
  auto trade_msg = mod.add_type<databento::TradeMsg>("TradeMsg");
  auto mbp1_msg = mod.add_type<databento::Mbp1Msg>("Mbp1Msg");
  auto mbp10_msg = mod.add_type<databento::Mbp10Msg>("Mbp10Msg");
  auto instrument_def_msg = mod.add_type<databento::InstrumentDefMsg>("InstrumentDefMsg");
  auto imbalance_msg = mod.add_type<databento::ImbalanceMsg>("ImbalanceMsg");
  auto ohlcv_msg = mod.add_type<databento::OhlcvMsg>("OhlcvMsg");
  auto status_msg = mod.add_type<databento::StatusMsg>("StatusMsg");
  auto stat_msg = mod.add_type<databento::StatMsg>("StatMsg");
  auto error_msg = mod.add_type<databento::ErrorMsg>("ErrorMsg");
  auto symbol_mapping_msg = mod.add_type<databento::SymbolMappingMsg>("SymbolMappingMsg");
  auto system_msg = mod.add_type<databento::SystemMsg>("SystemMsg");
  
  auto consolidated_bid_ask_pair = mod.add_type<databento::ConsolidatedBidAskPair>("ConsolidatedBidAskPair");
  auto bbo_msg = mod.add_type<databento::BboMsg>("BboMsg");
  auto cmbp1_msg = mod.add_type<databento::Cmbp1Msg>("Cmbp1Msg");
  auto cbbo_msg = mod.add_type<databento::CbboMsg>("CbboMsg");

  auto field_detail = mod.add_type<databento::FieldDetail>("FieldDetail");
  
  // Register vectors used in methods
  mod.add_type<std::vector<databento::Schema>>("SchemaVector");
  mod.add_type<std::vector<databento::FieldDetail>>("FieldDetailVector");

  auto symbology_resolution = mod.add_type<databento::SymbologyResolution>("SymbologyResolution");
  auto historical = mod.add_type<databento::Historical>("Historical");
  auto historical_builder = mod.add_type<databento::HistoricalBuilder>("HistoricalBuilder");
  auto record = mod.add_type<databento::Record>("Record");
  auto metadata = mod.add_type<databento::Metadata>("Metadata");
  auto dbn_file_store = mod.add_type<databento::DbnFileStore>("DbnFileStore");

  // ============================================================================
  // ENUM CONSTANTS
  // ============================================================================

  // Schema
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
  mod.set_const("CMBP1", databento::Schema::Cmbp1);
  mod.set_const("CBBO_1S", databento::Schema::Cbbo1S);
  mod.set_const("CBBO_1M", databento::Schema::Cbbo1M);
  mod.set_const("TCBBO", databento::Schema::Tcbbo);
  mod.set_const("BBO_1S", databento::Schema::Bbo1S);
  mod.set_const("BBO_1M", databento::Schema::Bbo1M);

  // Encoding
  mod.set_const("DBN", databento::Encoding::Dbn);
  mod.set_const("CSV", databento::Encoding::Csv);
  mod.set_const("JSON", databento::Encoding::Json);

  // SType
  mod.set_const("INSTRUMENT_ID", databento::SType::InstrumentId);
  mod.set_const("RAW_SYMBOL", databento::SType::RawSymbol);
  mod.set_const("CONTINUOUS", databento::SType::Continuous);
  mod.set_const("PARENT", databento::SType::Parent);
  mod.set_const("NASDAQ_SYMBOL", databento::SType::NasdaqSymbol);
  mod.set_const("CMS_SYMBOL", databento::SType::CmsSymbol);

  // Dataset
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

  // RType
  mod.set_const("RTYPE_MBP0", databento::RType::Mbp0);
  mod.set_const("RTYPE_MBP1", databento::RType::Mbp1);
  mod.set_const("RTYPE_MBP10", databento::RType::Mbp10);
  mod.set_const("RTYPE_OHLCV_DEPRECATED", databento::RType::OhlcvDeprecated);
  mod.set_const("RTYPE_OHLCV_1S", databento::RType::Ohlcv1S);
  mod.set_const("RTYPE_OHLCV_1M", databento::RType::Ohlcv1M);
  mod.set_const("RTYPE_OHLCV_1H", databento::RType::Ohlcv1H);
  mod.set_const("RTYPE_OHLCV_1D", databento::RType::Ohlcv1D);
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

  // Action
  mod.set_const("ACTION_MODIFY", databento::Action::Modify);
  mod.set_const("ACTION_TRADE", databento::Action::Trade);
  mod.set_const("ACTION_FILL", databento::Action::Fill);
  mod.set_const("ACTION_CANCEL", databento::Action::Cancel);
  mod.set_const("ACTION_ADD", databento::Action::Add);
  mod.set_const("ACTION_CLEAR", databento::Action::Clear);
  mod.set_const("ACTION_NONE", databento::Action::None);

  // Side
  mod.set_const("SIDE_ASK", databento::Side::Ask);
  mod.set_const("SIDE_BID", databento::Side::Bid);
  mod.set_const("SIDE_NONE", databento::Side::None);

  // InstrumentClass
  mod.set_const("INSTRUMENT_CLASS_BOND", databento::InstrumentClass::Bond);
  mod.set_const("INSTRUMENT_CLASS_CALL", databento::InstrumentClass::Call);
  mod.set_const("INSTRUMENT_CLASS_FUTURE", databento::InstrumentClass::Future);
  mod.set_const("INSTRUMENT_CLASS_STOCK", databento::InstrumentClass::Stock);
  mod.set_const("INSTRUMENT_CLASS_MIXED_SPREAD", databento::InstrumentClass::MixedSpread);
  mod.set_const("INSTRUMENT_CLASS_PUT", databento::InstrumentClass::Put);
  mod.set_const("INSTRUMENT_CLASS_FUTURE_SPREAD", databento::InstrumentClass::FutureSpread);
  mod.set_const("INSTRUMENT_CLASS_OPTION_SPREAD", databento::InstrumentClass::OptionSpread);
  mod.set_const("INSTRUMENT_CLASS_FX_SPOT", databento::InstrumentClass::FxSpot);
  mod.set_const("INSTRUMENT_CLASS_COMMODITY_SPOT", databento::InstrumentClass::CommoditySpot);

  // ============================================================================
  // METHODS
  // ============================================================================

  // ToString methods
  mod.method("to_string_schema", [](databento::Schema s) { return std::string(databento::ToString(s)); });
  mod.method("to_string_encoding", [](databento::Encoding e) { return std::string(databento::ToString(e)); });
  mod.method("to_string_stype", [](databento::SType s) { return std::string(databento::ToString(s)); });
  mod.method("to_string_dataset", [](databento::Dataset d) { return std::string(databento::ToString(d)); });
  mod.method("to_string_publisher", [](databento::Publisher p) { return std::string(databento::ToString(p)); });
  mod.method("to_string_rtype", [](databento::RType r) { return std::string(databento::ToString(r)); });
  mod.method("to_string_action", [](databento::Action a) { return std::string(databento::ToString(a)); });
  mod.method("to_string_side", [](databento::Side s) { return std::string(databento::ToString(s)); });
  mod.method("to_string_instrument_class", [](databento::InstrumentClass i) { return std::string(databento::ToString(i)); });
  mod.method("to_string_match_algorithm", [](databento::MatchAlgorithm m) { return std::string(databento::ToString(m)); });
  mod.method("to_string_status_action", [](databento::StatusAction s) { return std::string(databento::ToString(s)); });
  mod.method("to_string_status_reason", [](databento::StatusReason s) { return std::string(databento::ToString(s)); });
  mod.method("to_string_trading_event", [](databento::TradingEvent t) { return std::string(databento::ToString(t)); });
  mod.method("to_string_tri_state", [](databento::TriState t) { return std::string(databento::ToString(t)); });
  mod.method("to_string_stat_type", [](databento::StatType s) { return std::string(databento::ToString(s)); });
  mod.method("to_string_stat_update_action", [](databento::StatUpdateAction s) { return std::string(databento::ToString(s)); });

  // FlagSet
  flag_set.method("is_last", [](const databento::FlagSet& f) { return f.IsLast(); });
  flag_set.method("is_tob", [](const databento::FlagSet& f) { return f.IsTob(); });
  flag_set.method("is_snapshot", [](const databento::FlagSet& f) { return f.IsSnapshot(); });
  flag_set.method("is_mbp", [](const databento::FlagSet& f) { return f.IsMbp(); });
  flag_set.method("is_bad_ts_recv", [](const databento::FlagSet& f) { return f.IsBadTsRecv(); });
  flag_set.method("is_maybe_bad_book", [](const databento::FlagSet& f) { return f.IsMaybeBadBook(); });
  flag_set.method("raw", [](const databento::FlagSet& f) { return f.Raw(); });
  flag_set.method("any", [](const databento::FlagSet& f) { return f.Any(); });
  flag_set.method("is_empty", [](const databento::FlagSet& f) { return f.IsEmpty(); });

  // UnixNanos
  unix_nanos.method("time_since_epoch", [](const databento::UnixNanos& ts) -> std::uint64_t {
    return ts.time_since_epoch().count();
  });

  // TimeDeltaNanos
  time_delta_nanos.method("count", [](const databento::TimeDeltaNanos& td) -> std::int32_t {
    return td.count();
  });

  // BidAskPair
  bid_ask_pair.method("bid_px", [](const databento::BidAskPair& p) { return p.bid_px; });
  bid_ask_pair.method("ask_px", [](const databento::BidAskPair& p) { return p.ask_px; });
  bid_ask_pair.method("bid_sz", [](const databento::BidAskPair& p) { return p.bid_sz; });
  bid_ask_pair.method("ask_sz", [](const databento::BidAskPair& p) { return p.ask_sz; });
  bid_ask_pair.method("bid_ct", [](const databento::BidAskPair& p) { return p.bid_ct; });
  bid_ask_pair.method("ask_ct", [](const databento::BidAskPair& p) { return p.ask_ct; });

  // RecordHeader
  record_header.method("length", [](const databento::RecordHeader& h) { return h.length; });
  record_header.method("rtype", [](const databento::RecordHeader& h) { return h.rtype; });
  record_header.method("publisher_id", [](const databento::RecordHeader& h) { return h.publisher_id; });
  record_header.method("instrument_id", [](const databento::RecordHeader& h) { return h.instrument_id; });
  record_header.method("ts_event", [](const databento::RecordHeader& h) { return h.ts_event; });
  record_header.method("size", [](const databento::RecordHeader& h) { return h.Size(); });
  record_header.method("publisher_enum", [](const databento::RecordHeader& h) { return h.Publisher(); });

  // MboMsg
  mbo_msg.method("hd", [](const databento::MboMsg& m) { return m.hd; });
  mbo_msg.method("order_id", [](const databento::MboMsg& m) { return m.order_id; });
  mbo_msg.method("price", [](const databento::MboMsg& m) { return m.price; });
  mbo_msg.method("size", [](const databento::MboMsg& m) { return m.size; });
  mbo_msg.method("flags", [](const databento::MboMsg& m) { return m.flags; });
  mbo_msg.method("channel_id", [](const databento::MboMsg& m) { return m.channel_id; });
  mbo_msg.method("action", [](const databento::MboMsg& m) { return m.action; });
  mbo_msg.method("side", [](const databento::MboMsg& m) { return m.side; });
  mbo_msg.method("ts_recv", [](const databento::MboMsg& m) { return m.ts_recv; });
  mbo_msg.method("ts_in_delta", [](const databento::MboMsg& m) { return m.ts_in_delta; });
  mbo_msg.method("sequence", [](const databento::MboMsg& m) { return m.sequence; });
  mbo_msg.method("index_ts", [](const databento::MboMsg& m) { return m.IndexTs(); });
  mbo_msg.method("to_string", [](const databento::MboMsg& m) -> std::string { return databento::ToString(m); });

  // TradeMsg
  trade_msg.method("hd", [](const databento::TradeMsg& m) { return m.hd; });
  trade_msg.method("price", [](const databento::TradeMsg& m) { return m.price; });
  trade_msg.method("size", [](const databento::TradeMsg& m) { return m.size; });
  trade_msg.method("action", [](const databento::TradeMsg& m) { return m.action; });
  trade_msg.method("side", [](const databento::TradeMsg& m) { return m.side; });
  trade_msg.method("flags", [](const databento::TradeMsg& m) { return m.flags; });
  trade_msg.method("depth", [](const databento::TradeMsg& m) { return m.depth; });
  trade_msg.method("ts_recv", [](const databento::TradeMsg& m) { return m.ts_recv; });
  trade_msg.method("ts_in_delta", [](const databento::TradeMsg& m) { return m.ts_in_delta; });
  trade_msg.method("sequence", [](const databento::TradeMsg& m) { return m.sequence; });
  trade_msg.method("index_ts", [](const databento::TradeMsg& m) { return m.IndexTs(); });
  trade_msg.method("to_string", [](const databento::TradeMsg& m) -> std::string { return databento::ToString(m); });

  // Mbp1Msg
  mbp1_msg.method("hd", [](const databento::Mbp1Msg& m) { return m.hd; });
  mbp1_msg.method("price", [](const databento::Mbp1Msg& m) { return m.price; });
  mbp1_msg.method("size", [](const databento::Mbp1Msg& m) { return m.size; });
  mbp1_msg.method("action", [](const databento::Mbp1Msg& m) { return m.action; });
  mbp1_msg.method("side", [](const databento::Mbp1Msg& m) { return m.side; });
  mbp1_msg.method("flags", [](const databento::Mbp1Msg& m) { return m.flags; });
  mbp1_msg.method("depth", [](const databento::Mbp1Msg& m) { return m.depth; });
  mbp1_msg.method("ts_recv", [](const databento::Mbp1Msg& m) { return m.ts_recv; });
  mbp1_msg.method("ts_in_delta", [](const databento::Mbp1Msg& m) { return m.ts_in_delta; });
  mbp1_msg.method("sequence", [](const databento::Mbp1Msg& m) { return m.sequence; });
  mbp1_msg.method("index_ts", [](const databento::Mbp1Msg& m) { return m.IndexTs(); });
  mbp1_msg.method("level", [](const databento::Mbp1Msg& m, std::size_t i) { return m.levels.at(i); });
  mbp1_msg.method("to_string", [](const databento::Mbp1Msg& m) -> std::string { return databento::ToString(m); });

  // Mbp10Msg
  mbp10_msg.method("hd", [](const databento::Mbp10Msg& m) { return m.hd; });
  mbp10_msg.method("price", [](const databento::Mbp10Msg& m) { return m.price; });
  mbp10_msg.method("size", [](const databento::Mbp10Msg& m) { return m.size; });
  mbp10_msg.method("action", [](const databento::Mbp10Msg& m) { return m.action; });
  mbp10_msg.method("side", [](const databento::Mbp10Msg& m) { return m.side; });
  mbp10_msg.method("flags", [](const databento::Mbp10Msg& m) { return m.flags; });
  mbp10_msg.method("depth", [](const databento::Mbp10Msg& m) { return m.depth; });
  mbp10_msg.method("ts_recv", [](const databento::Mbp10Msg& m) { return m.ts_recv; });
  mbp10_msg.method("ts_in_delta", [](const databento::Mbp10Msg& m) { return m.ts_in_delta; });
  mbp10_msg.method("sequence", [](const databento::Mbp10Msg& m) { return m.sequence; });
  mbp10_msg.method("index_ts", [](const databento::Mbp10Msg& m) { return m.IndexTs(); });
  mbp10_msg.method("level", [](const databento::Mbp10Msg& m, std::size_t i) { return m.levels.at(i); });
  mbp10_msg.method("to_string", [](const databento::Mbp10Msg& m) -> std::string { return databento::ToString(m); });

  // InstrumentDefMsg
  instrument_def_msg.method("hd", [](const databento::InstrumentDefMsg& m) { return m.hd; });
  instrument_def_msg.method("ts_recv", [](const databento::InstrumentDefMsg& m) { return m.ts_recv; });
  instrument_def_msg.method("min_price_increment", [](const databento::InstrumentDefMsg& m) { return m.min_price_increment; });
  instrument_def_msg.method("display_factor", [](const databento::InstrumentDefMsg& m) { return m.display_factor; });
  instrument_def_msg.method("expiration", [](const databento::InstrumentDefMsg& m) { return m.expiration; });
  instrument_def_msg.method("activation", [](const databento::InstrumentDefMsg& m) { return m.activation; });
  instrument_def_msg.method("high_limit_price", [](const databento::InstrumentDefMsg& m) { return m.high_limit_price; });
  instrument_def_msg.method("low_limit_price", [](const databento::InstrumentDefMsg& m) { return m.low_limit_price; });
  instrument_def_msg.method("max_price_variation", [](const databento::InstrumentDefMsg& m) { return m.max_price_variation; });
  instrument_def_msg.method("trading_reference_price", [](const databento::InstrumentDefMsg& m) { return m.trading_reference_price; });
  instrument_def_msg.method("unit_of_measure_qty", [](const databento::InstrumentDefMsg& m) { return m.unit_of_measure_qty; });
  instrument_def_msg.method("min_price_increment_amount", [](const databento::InstrumentDefMsg& m) { return m.min_price_increment_amount; });
  instrument_def_msg.method("price_ratio", [](const databento::InstrumentDefMsg& m) { return m.price_ratio; });
  instrument_def_msg.method("strike_price", [](const databento::InstrumentDefMsg& m) { return m.strike_price; });
  instrument_def_msg.method("underlying_id", [](const databento::InstrumentDefMsg& m) { return m.underlying_id; });
  instrument_def_msg.method("raw_instrument_id", [](const databento::InstrumentDefMsg& m) { return m.raw_instrument_id; });
  instrument_def_msg.method("market_depth_implied", [](const databento::InstrumentDefMsg& m) { return m.market_depth_implied; });
  instrument_def_msg.method("market_depth", [](const databento::InstrumentDefMsg& m) { return m.market_depth; });
  instrument_def_msg.method("market_segment_id", [](const databento::InstrumentDefMsg& m) { return m.market_segment_id; });
  instrument_def_msg.method("max_trade_vol", [](const databento::InstrumentDefMsg& m) { return m.max_trade_vol; });
  instrument_def_msg.method("min_lot_size", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size; });
  instrument_def_msg.method("min_lot_size_block", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size_block; });
  instrument_def_msg.method("min_lot_size_round_lot", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size_round_lot; });
  instrument_def_msg.method("min_trade_vol", [](const databento::InstrumentDefMsg& m) { return m.min_trade_vol; });
  instrument_def_msg.method("contract_multiplier", [](const databento::InstrumentDefMsg& m) { return m.contract_multiplier; });
  instrument_def_msg.method("decay_quantity", [](const databento::InstrumentDefMsg& m) { return m.decay_quantity; });
  instrument_def_msg.method("original_contract_size", [](const databento::InstrumentDefMsg& m) { return m.original_contract_size; });
  instrument_def_msg.method("trading_reference_date", [](const databento::InstrumentDefMsg& m) { return m.trading_reference_date; });
  instrument_def_msg.method("maturity_year", [](const databento::InstrumentDefMsg& m) { return m.maturity_year; });
  instrument_def_msg.method("decay_start_date", [](const databento::InstrumentDefMsg& m) { return m.decay_start_date; });
  instrument_def_msg.method("channel_id", [](const databento::InstrumentDefMsg& m) { return m.channel_id; });
  instrument_def_msg.method("currency", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Currency(); });
  instrument_def_msg.method("settl_currency", [](const databento::InstrumentDefMsg& m) -> std::string { return m.SettlCurrency(); });
  instrument_def_msg.method("secsubtype", [](const databento::InstrumentDefMsg& m) -> std::string { return m.SecSubType(); });
  instrument_def_msg.method("raw_symbol", [](const databento::InstrumentDefMsg& m) -> std::string { return m.RawSymbol(); });
  instrument_def_msg.method("group", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Group(); });
  instrument_def_msg.method("exchange", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Exchange(); });
  instrument_def_msg.method("asset", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Asset(); });
  instrument_def_msg.method("cfi", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Cfi(); });
  instrument_def_msg.method("security_type", [](const databento::InstrumentDefMsg& m) -> std::string { return m.SecurityType(); });
  instrument_def_msg.method("instrument_class", [](const databento::InstrumentDefMsg& m) { return m.instrument_class; });
  instrument_def_msg.method("unit_of_measure", [](const databento::InstrumentDefMsg& m) -> std::string { return m.UnitOfMeasure(); });
  instrument_def_msg.method("underlying", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Underlying(); });
  instrument_def_msg.method("strike_price_currency", [](const databento::InstrumentDefMsg& m) -> std::string { return m.StrikePriceCurrency(); });
  instrument_def_msg.method("index_ts", [](const databento::InstrumentDefMsg& m) { return m.IndexTs(); });
  instrument_def_msg.method("to_string", [](const databento::InstrumentDefMsg& m) -> std::string { return databento::ToString(m); });

  // ImbalanceMsg
  imbalance_msg.method("hd", [](const databento::ImbalanceMsg& m) { return m.hd; });
  imbalance_msg.method("ts_recv", [](const databento::ImbalanceMsg& m) { return m.ts_recv; });
  imbalance_msg.method("ref_price", [](const databento::ImbalanceMsg& m) { return m.ref_price; });
  imbalance_msg.method("auction_time", [](const databento::ImbalanceMsg& m) { return m.auction_time; });
  imbalance_msg.method("cont_book_clr_price", [](const databento::ImbalanceMsg& m) { return m.cont_book_clr_price; });
  imbalance_msg.method("auct_interest_clr_price", [](const databento::ImbalanceMsg& m) { return m.auct_interest_clr_price; });
  imbalance_msg.method("ssr_filling_price", [](const databento::ImbalanceMsg& m) { return m.ssr_filling_price; });
  imbalance_msg.method("ind_match_price", [](const databento::ImbalanceMsg& m) { return m.ind_match_price; });
  imbalance_msg.method("upper_collar", [](const databento::ImbalanceMsg& m) { return m.upper_collar; });
  imbalance_msg.method("lower_collar", [](const databento::ImbalanceMsg& m) { return m.lower_collar; });
  imbalance_msg.method("paired_qty", [](const databento::ImbalanceMsg& m) { return m.paired_qty; });
  imbalance_msg.method("total_imbalance_qty", [](const databento::ImbalanceMsg& m) { return m.total_imbalance_qty; });
  imbalance_msg.method("market_imbalance_qty", [](const databento::ImbalanceMsg& m) { return m.market_imbalance_qty; });
  imbalance_msg.method("unpaired_qty", [](const databento::ImbalanceMsg& m) { return m.unpaired_qty; });
  imbalance_msg.method("side", [](const databento::ImbalanceMsg& m) { return m.side; });
  imbalance_msg.method("auction_status", [](const databento::ImbalanceMsg& m) { return static_cast<std::uint32_t>(m.auction_status); });
  imbalance_msg.method("index_ts", [](const databento::ImbalanceMsg& m) { return m.IndexTs(); });
  imbalance_msg.method("to_string", [](const databento::ImbalanceMsg& m) -> std::string { return databento::ToString(m); });

  // OhlcvMsg
  ohlcv_msg.method("hd", [](const databento::OhlcvMsg& m) { return m.hd; });
  ohlcv_msg.method("open", [](const databento::OhlcvMsg& m) { return m.open; });
  ohlcv_msg.method("high", [](const databento::OhlcvMsg& m) { return m.high; });
  ohlcv_msg.method("low", [](const databento::OhlcvMsg& m) { return m.low; });
  ohlcv_msg.method("close", [](const databento::OhlcvMsg& m) { return m.close; });
  ohlcv_msg.method("volume", [](const databento::OhlcvMsg& m) { return m.volume; });
  ohlcv_msg.method("to_string", [](const databento::OhlcvMsg& m) -> std::string { return databento::ToString(m); });

  // StatusMsg
  status_msg.method("hd", [](const databento::StatusMsg& m) { return m.hd; });
  status_msg.method("ts_recv", [](const databento::StatusMsg& m) { return m.ts_recv; });
  status_msg.method("action", [](const databento::StatusMsg& m) { return m.action; });
  status_msg.method("reason", [](const databento::StatusMsg& m) { return m.reason; });
  status_msg.method("trading_event", [](const databento::StatusMsg& m) { return m.trading_event; });
  status_msg.method("is_trading", [](const databento::StatusMsg& m) { return m.is_trading; });
  status_msg.method("is_quoting", [](const databento::StatusMsg& m) { return m.is_quoting; });
  status_msg.method("is_short_sell_restricted", [](const databento::StatusMsg& m) { return m.is_short_sell_restricted; });
  status_msg.method("to_string", [](const databento::StatusMsg& m) -> std::string { return databento::ToString(m); });

  // StatMsg
  stat_msg.method("hd", [](const databento::StatMsg& m) { return m.hd; });
  stat_msg.method("ts_recv", [](const databento::StatMsg& m) { return m.ts_recv; });
  stat_msg.method("ts_ref", [](const databento::StatMsg& m) { return m.ts_ref; });
  stat_msg.method("price", [](const databento::StatMsg& m) { return m.price; });
  stat_msg.method("quantity", [](const databento::StatMsg& m) { return m.quantity; });
  stat_msg.method("stat_type", [](const databento::StatMsg& m) { return m.stat_type; });
  stat_msg.method("update_action", [](const databento::StatMsg& m) { return m.update_action; });
  stat_msg.method("sequence", [](const databento::StatMsg& m) { return m.sequence; });
  stat_msg.method("ts_in_delta", [](const databento::StatMsg& m) { return m.ts_in_delta; });
  stat_msg.method("to_string", [](const databento::StatMsg& m) -> std::string { return databento::ToString(m); });

  // BboMsg
  bbo_msg.method("hd", [](const databento::BboMsg& m) { return m.hd; });
  bbo_msg.method("price", [](const databento::BboMsg& m) { return m.price; });
  bbo_msg.method("size", [](const databento::BboMsg& m) { return m.size; });
  bbo_msg.method("side", [](const databento::BboMsg& m) { return m.side; });
  bbo_msg.method("flags", [](const databento::BboMsg& m) { return m.flags; });
  bbo_msg.method("ts_recv", [](const databento::BboMsg& m) { return m.ts_recv; });
  bbo_msg.method("sequence", [](const databento::BboMsg& m) { return m.sequence; });
  bbo_msg.method("level", [](const databento::BboMsg& m, std::size_t i) { return m.levels.at(i); });
  bbo_msg.method("to_string", [](const databento::BboMsg& m) -> std::string { return databento::ToString(m); });

  // Cmbp1Msg
  cmbp1_msg.method("hd", [](const databento::Cmbp1Msg& m) { return m.hd; });
  cmbp1_msg.method("price", [](const databento::Cmbp1Msg& m) { return m.price; });
  cmbp1_msg.method("size", [](const databento::Cmbp1Msg& m) { return m.size; });
  cmbp1_msg.method("action", [](const databento::Cmbp1Msg& m) { return m.action; });
  cmbp1_msg.method("side", [](const databento::Cmbp1Msg& m) { return m.side; });
  cmbp1_msg.method("flags", [](const databento::Cmbp1Msg& m) { return m.flags; });
  cmbp1_msg.method("ts_recv", [](const databento::Cmbp1Msg& m) { return m.ts_recv; });
  cmbp1_msg.method("ts_in_delta", [](const databento::Cmbp1Msg& m) { return m.ts_in_delta; });
  cmbp1_msg.method("level", [](const databento::Cmbp1Msg& m, std::size_t i) { return m.levels.at(i); });

  // CbboMsg
  cbbo_msg.method("hd", [](const databento::CbboMsg& m) { return m.hd; });
  cbbo_msg.method("price", [](const databento::CbboMsg& m) { return m.price; });
  cbbo_msg.method("size", [](const databento::CbboMsg& m) { return m.size; });
  cbbo_msg.method("side", [](const databento::CbboMsg& m) { return m.side; });
  cbbo_msg.method("flags", [](const databento::CbboMsg& m) { return m.flags; });
  cbbo_msg.method("ts_recv", [](const databento::CbboMsg& m) { return m.ts_recv; });
  cbbo_msg.method("level", [](const databento::CbboMsg& m, std::size_t i) { return m.levels.at(i); });

  // ConsolidatedBidAskPair
  consolidated_bid_ask_pair.method("bid_px", [](const databento::ConsolidatedBidAskPair& p) { return p.bid_px; });
  consolidated_bid_ask_pair.method("ask_px", [](const databento::ConsolidatedBidAskPair& p) { return p.ask_px; });
  consolidated_bid_ask_pair.method("bid_sz", [](const databento::ConsolidatedBidAskPair& p) { return p.bid_sz; });
  consolidated_bid_ask_pair.method("ask_sz", [](const databento::ConsolidatedBidAskPair& p) { return p.ask_sz; });
  consolidated_bid_ask_pair.method("bid_pb", [](const databento::ConsolidatedBidAskPair& p) { return p.bid_pb; });
  consolidated_bid_ask_pair.method("ask_pb", [](const databento::ConsolidatedBidAskPair& p) { return p.ask_pb; });

  // FieldDetail
  field_detail.method("name", [](const databento::FieldDetail& f) { return f.name; });
  field_detail.method("type", [](const databento::FieldDetail& f) { return f.type; });

  // HistoricalBuilder
  historical_builder.constructor<>()
    .method("set_key!", [](databento::HistoricalBuilder& builder, const std::string& key) -> databento::HistoricalBuilder& { return builder.SetKey(key); })
    .method("set_key_from_env!", [](databento::HistoricalBuilder& builder) -> databento::HistoricalBuilder& { return builder.SetKeyFromEnv(); })
    .method("build", [](databento::HistoricalBuilder& builder) -> databento::Historical { return builder.Build(); });

  // Historical
  historical.method("metadata_list_datasets", [](databento::Historical& client) -> std::vector<std::string> { return client.MetadataListDatasets(); })
    .method("metadata_list_schemas", [](databento::Historical& client, const std::string& dataset) -> std::vector<databento::Schema> { return client.MetadataListSchemas(dataset); })
    .method("metadata_list_fields", [](databento::Historical& client, databento::Encoding encoding, databento::Schema schema) -> std::vector<databento::FieldDetail> { return client.MetadataListFields(encoding, schema); })
    .method("symbology_resolve", [](databento::Historical& client, const std::string& dataset, const std::vector<std::string>& symbols, databento::SType stype_in, databento::SType stype_out, const std::string& start_date, const std::string& end_date) -> databento::SymbologyResolution {
      return client.SymbologyResolve(dataset, symbols, stype_in, stype_out, databento::DateRange{start_date, end_date});
    })
    .method("timeseries_get_range_to_file", [](databento::Historical& client, const std::string& dataset, const std::vector<std::string>& symbols, databento::Schema schema, const std::string& start, const std::string& end, databento::SType stype_in, databento::SType stype_out, std::uint64_t limit, const std::string& output_file) -> databento::DbnFileStore {
      return client.TimeseriesGetRangeToFile(dataset, databento::DateTimeRange<std::string>{start, end}, symbols, schema, stype_in, stype_out, limit, output_file);
    });

  // Record
  record.method("header", [](const databento::Record& r) -> const databento::RecordHeader& { return r.Header(); })
    .method("rtype", [](const databento::Record& r) -> databento::RType { return r.RType(); })
    .method("size", [](const databento::Record& r) -> std::size_t { return r.Size(); })
    .method("holds_mbo", [](const databento::Record& r) -> bool { return r.Holds<databento::MboMsg>(); })
    .method("holds_trade", [](const databento::Record& r) -> bool { return r.Holds<databento::TradeMsg>(); })
    .method("holds_mbp1", [](const databento::Record& r) -> bool { return r.Holds<databento::Mbp1Msg>(); })
    .method("holds_mbp10", [](const databento::Record& r) -> bool { return r.Holds<databento::Mbp10Msg>(); })
    .method("holds_imbalance", [](const databento::Record& r) -> bool { return r.Holds<databento::ImbalanceMsg>(); })
    .method("get_mbo_if", [](const databento::Record& r) -> const databento::MboMsg* { return r.GetIf<databento::MboMsg>(); })
    .method("get_trade_if", [](const databento::Record& r) -> const databento::TradeMsg* { return r.GetIf<databento::TradeMsg>(); })
    .method("get_mbp1_if", [](const databento::Record& r) -> const databento::Mbp1Msg* { return r.GetIf<databento::Mbp1Msg>(); })
    .method("get_mbp10_if", [](const databento::Record& r) -> const databento::Mbp10Msg* { return r.GetIf<databento::Mbp10Msg>(); })
    .method("get_imbalance_if", [](const databento::Record& r) -> const databento::ImbalanceMsg* { return r.GetIf<databento::ImbalanceMsg>(); })
    .method("get_instrument_def_if", [](const databento::Record& r) -> const databento::InstrumentDefMsg* { return r.GetIf<databento::InstrumentDefMsg>(); })
    .method("get_ohlcv_if", [](const databento::Record& r) -> const databento::OhlcvMsg* { return r.GetIf<databento::OhlcvMsg>(); })
    .method("get_status_if", [](const databento::Record& r) -> const databento::StatusMsg* { return r.GetIf<databento::StatusMsg>(); })
    .method("get_stat_if", [](const databento::Record& r) -> const databento::StatMsg* { return r.GetIf<databento::StatMsg>(); })
    .method("get_error_if", [](const databento::Record& r) -> const databento::ErrorMsg* { return r.GetIf<databento::ErrorMsg>(); })
    .method("get_symbol_mapping_if", [](const databento::Record& r) -> const databento::SymbolMappingMsg* { return r.GetIf<databento::SymbolMappingMsg>(); })
    .method("get_system_if", [](const databento::Record& r) -> const databento::SystemMsg* { return r.GetIf<databento::SystemMsg>(); })
    .method("get_bbo_if", [](const databento::Record& r) -> const databento::BboMsg* { return r.GetIf<databento::BboMsg>(); })
    .method("get_cmbp1_if", [](const databento::Record& r) -> const databento::Cmbp1Msg* { return r.GetIf<databento::Cmbp1Msg>(); })
    .method("get_cbbo_if", [](const databento::Record& r) -> const databento::CbboMsg* { return r.GetIf<databento::CbboMsg>(); });

  // Metadata
  metadata.method("version", [](const databento::Metadata& m) -> std::uint8_t { return m.version; })
    .method("dataset", [](const databento::Metadata& m) -> std::string { return m.dataset; })
    .method("has_mixed_schema", [](const databento::Metadata& m) -> bool { return m.has_mixed_schema; })
    .method("schema", [](const databento::Metadata& m) -> databento::Schema { return m.schema; })
    .method("start_nanos", [](const databento::Metadata& m) -> std::uint64_t { return m.start.time_since_epoch().count(); })
    .method("end_nanos", [](const databento::Metadata& m) -> std::uint64_t { return m.end.time_since_epoch().count(); })
    .method("limit", [](const databento::Metadata& m) -> std::uint64_t { return m.limit; })
    .method("stype_in", [](const databento::Metadata& m) -> databento::SType { return m.stype_in; })
    .method("stype_out", [](const databento::Metadata& m) -> databento::SType { return m.stype_out; })
    .method("ts_out", [](const databento::Metadata& m) -> bool { return m.ts_out; })
    .method("symbols", [](const databento::Metadata& m) -> std::vector<std::string> { return m.symbols; })
    .method("partial", [](const databento::Metadata& m) -> std::vector<std::string> { return m.partial; })
    .method("not_found", [](const databento::Metadata& m) -> std::vector<std::string> { return m.not_found; });

  // DbnFileStore
  dbn_file_store.constructor<const std::string&>()
    .method("get_metadata", [](databento::DbnFileStore& store) -> const databento::Metadata& { return store.GetMetadata(); })
    .method("next_record", [](databento::DbnFileStore& store) -> const databento::Record* { return store.NextRecord(); });
}