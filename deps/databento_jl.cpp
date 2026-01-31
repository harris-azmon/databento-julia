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
}

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
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
  mod.set_const("CONTINUOUS", databento::SType::Continuous);
  mod.set_const("PARENT", databento::SType::Parent);
  mod.set_const("NASDAQ_SYMBOL", databento::SType::NasdaqSymbol);
  mod.set_const("CMS_SYMBOL", databento::SType::CmsSymbol);

  // SType ToString function
  mod.method("string", [](databento::SType s) -> std::string {
    return std::string(databento::ToString(s));
  });

  // Dataset Enum - Data sources
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

  // Dataset ToString function
  mod.method("string", [](databento::Dataset d) -> std::string {
    return std::string(databento::ToString(d));
  });

  // Publisher Enum - Specific venue from a data source
  mod.add_bits<databento::Publisher>("Publisher", jlcxx::julia_type("CppEnum"));

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
  mod.add_type<databento::FlagSet>("FlagSet")
    .method("is_last", [](const databento::FlagSet& f) { return f.IsLast(); })
    .method("is_tob", [](const databento::FlagSet& f) { return f.IsTob(); })
    .method("is_snapshot", [](const databento::FlagSet& f) { return f.IsSnapshot(); })
    .method("is_mbp", [](const databento::FlagSet& f) { return f.IsMbp(); })
    .method("is_bad_ts_recv", [](const databento::FlagSet& f) { return f.IsBadTsRecv(); })
    .method("is_maybe_bad_book", [](const databento::FlagSet& f) { return f.IsMaybeBadBook(); })
    .method("raw", [](const databento::FlagSet& f) { return f.Raw(); })
    .method("any", [](const databento::FlagSet& f) { return f.Any(); })
    .method("is_empty", [](const databento::FlagSet& f) { return f.IsEmpty(); });

  // UnixNanos - Timestamp type (nanoseconds since Unix epoch)
  mod.add_type<databento::UnixNanos>("UnixNanos")
    .method("time_since_epoch", [](const databento::UnixNanos& ts) -> std::uint64_t {
      return ts.time_since_epoch().count();
    });

  // TimeDeltaNanos - Time delta in nanoseconds
  mod.add_type<databento::TimeDeltaNanos>("TimeDeltaNanos")
    .method("count", [](const databento::TimeDeltaNanos& td) -> std::int32_t {
      return td.count();
    });

  // BidAskPair - Price level data
  mod.add_type<databento::BidAskPair>("BidAskPair")
    .method("bid_px", [](const databento::BidAskPair& p) { return p.bid_px; })
    .method("ask_px", [](const databento::BidAskPair& p) { return p.ask_px; })
    .method("bid_sz", [](const databento::BidAskPair& p) { return p.bid_sz; })
    .method("ask_sz", [](const databento::BidAskPair& p) { return p.ask_sz; })
    .method("bid_ct", [](const databento::BidAskPair& p) { return p.bid_ct; })
    .method("ask_ct", [](const databento::BidAskPair& p) { return p.ask_ct; });

  // RecordHeader - Common header for all records
  mod.add_type<databento::RecordHeader>("RecordHeader")
    .method("length", [](const databento::RecordHeader& h) { return h.length; })
    .method("rtype", [](const databento::RecordHeader& h) { return h.rtype; })
    .method("publisher_id", [](const databento::RecordHeader& h) { return h.publisher_id; })
    .method("instrument_id", [](const databento::RecordHeader& h) { return h.instrument_id; })
    .method("ts_event", [](const databento::RecordHeader& h) { return h.ts_event; })
    .method("size", [](const databento::RecordHeader& h) { return h.Size(); })
    .method("publisher_enum", [](const databento::RecordHeader& h) { return h.Publisher(); });

  // MboMsg - Market By Order message
  mod.add_type<databento::MboMsg>("MboMsg")
    .method("hd", [](const databento::MboMsg& m) { return m.hd; })
    .method("order_id", [](const databento::MboMsg& m) { return m.order_id; })
    .method("price", [](const databento::MboMsg& m) { return m.price; })
    .method("size", [](const databento::MboMsg& m) { return m.size; })
    .method("flags", [](const databento::MboMsg& m) { return m.flags; })
    .method("channel_id", [](const databento::MboMsg& m) { return m.channel_id; })
    .method("action", [](const databento::MboMsg& m) { return m.action; })
    .method("side", [](const databento::MboMsg& m) { return m.side; })
    .method("ts_recv", [](const databento::MboMsg& m) { return m.ts_recv; })
    .method("ts_in_delta", [](const databento::MboMsg& m) { return m.ts_in_delta; })
    .method("sequence", [](const databento::MboMsg& m) { return m.sequence; })
    .method("index_ts", [](const databento::MboMsg& m) { return m.IndexTs(); })
    .method("to_string", [](const databento::MboMsg& m) -> std::string {
      return databento::ToString(m);
    });

  // TradeMsg - Trade message
  mod.add_type<databento::TradeMsg>("TradeMsg")
    .method("hd", [](const databento::TradeMsg& m) { return m.hd; })
    .method("price", [](const databento::TradeMsg& m) { return m.price; })
    .method("size", [](const databento::TradeMsg& m) { return m.size; })
    .method("action", [](const databento::TradeMsg& m) { return m.action; })
    .method("side", [](const databento::TradeMsg& m) { return m.side; })
    .method("flags", [](const databento::TradeMsg& m) { return m.flags; })
    .method("depth", [](const databento::TradeMsg& m) { return m.depth; })
    .method("ts_recv", [](const databento::TradeMsg& m) { return m.ts_recv; })
    .method("ts_in_delta", [](const databento::TradeMsg& m) { return m.ts_in_delta; })
    .method("sequence", [](const databento::TradeMsg& m) { return m.sequence; })
    .method("index_ts", [](const databento::TradeMsg& m) { return m.IndexTs(); })
    .method("to_string", [](const databento::TradeMsg& m) -> std::string {
      return databento::ToString(m);
    });

  // Mbp1Msg - Market By Price Level 1 message
  mod.add_type<databento::Mbp1Msg>("Mbp1Msg")
    .method("hd", [](const databento::Mbp1Msg& m) { return m.hd; })
    .method("price", [](const databento::Mbp1Msg& m) { return m.price; })
    .method("size", [](const databento::Mbp1Msg& m) { return m.size; })
    .method("action", [](const databento::Mbp1Msg& m) { return m.action; })
    .method("side", [](const databento::Mbp1Msg& m) { return m.side; })
    .method("flags", [](const databento::Mbp1Msg& m) { return m.flags; })
    .method("depth", [](const databento::Mbp1Msg& m) { return m.depth; })
    .method("ts_recv", [](const databento::Mbp1Msg& m) { return m.ts_recv; })
    .method("ts_in_delta", [](const databento::Mbp1Msg& m) { return m.ts_in_delta; })
    .method("sequence", [](const databento::Mbp1Msg& m) { return m.sequence; })
    .method("index_ts", [](const databento::Mbp1Msg& m) { return m.IndexTs(); })
    .method("to_string", [](const databento::Mbp1Msg& m) -> std::string {
      return databento::ToString(m);
    });

  // Mbp10Msg - Market By Price Level 10 message
  mod.add_type<databento::Mbp10Msg>("Mbp10Msg")
    .method("hd", [](const databento::Mbp10Msg& m) { return m.hd; })
    .method("price", [](const databento::Mbp10Msg& m) { return m.price; })
    .method("size", [](const databento::Mbp10Msg& m) { return m.size; })
    .method("action", [](const databento::Mbp10Msg& m) { return m.action; })
    .method("side", [](const databento::Mbp10Msg& m) { return m.side; })
    .method("flags", [](const databento::Mbp10Msg& m) { return m.flags; })
    .method("depth", [](const databento::Mbp10Msg& m) { return m.depth; })
    .method("ts_recv", [](const databento::Mbp10Msg& m) { return m.ts_recv; })
    .method("ts_in_delta", [](const databento::Mbp10Msg& m) { return m.ts_in_delta; })
    .method("sequence", [](const databento::Mbp10Msg& m) { return m.sequence; })
    .method("index_ts", [](const databento::Mbp10Msg& m) { return m.IndexTs(); })
    .method("to_string", [](const databento::Mbp10Msg& m) -> std::string {
      return databento::ToString(m);
    });

  // InstrumentDefMsg - Instrument definition message
  mod.add_type<databento::InstrumentDefMsg>("InstrumentDefMsg")
    .method("hd", [](const databento::InstrumentDefMsg& m) { return m.hd; })
    .method("ts_recv", [](const databento::InstrumentDefMsg& m) { return m.ts_recv; })
    .method("min_price_increment", [](const databento::InstrumentDefMsg& m) { return m.min_price_increment; })
    .method("display_factor", [](const databento::InstrumentDefMsg& m) { return m.display_factor; })
    .method("expiration", [](const databento::InstrumentDefMsg& m) { return m.expiration; })
    .method("activation", [](const databento::InstrumentDefMsg& m) { return m.activation; })
    .method("high_limit_price", [](const databento::InstrumentDefMsg& m) { return m.high_limit_price; })
    .method("low_limit_price", [](const databento::InstrumentDefMsg& m) { return m.low_limit_price; })
    .method("max_price_variation", [](const databento::InstrumentDefMsg& m) { return m.max_price_variation; })
    .method("trading_reference_price", [](const databento::InstrumentDefMsg& m) { return m.trading_reference_price; })
    .method("unit_of_measure_qty", [](const databento::InstrumentDefMsg& m) { return m.unit_of_measure_qty; })
    .method("min_price_increment_amount", [](const databento::InstrumentDefMsg& m) { return m.min_price_increment_amount; })
    .method("price_ratio", [](const databento::InstrumentDefMsg& m) { return m.price_ratio; })
    .method("strike_price", [](const databento::InstrumentDefMsg& m) { return m.strike_price; })
    .method("underlying_id", [](const databento::InstrumentDefMsg& m) { return m.underlying_id; })
    .method("raw_instrument_id", [](const databento::InstrumentDefMsg& m) { return m.raw_instrument_id; })
    .method("market_depth_implied", [](const databento::InstrumentDefMsg& m) { return m.market_depth_implied; })
    .method("market_depth", [](const databento::InstrumentDefMsg& m) { return m.market_depth; })
    .method("market_segment_id", [](const databento::InstrumentDefMsg& m) { return m.market_segment_id; })
    .method("max_trade_vol", [](const databento::InstrumentDefMsg& m) { return m.max_trade_vol; })
    .method("min_lot_size", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size; })
    .method("min_lot_size_block", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size_block; })
    .method("min_lot_size_round_lot", [](const databento::InstrumentDefMsg& m) { return m.min_lot_size_round_lot; })
    .method("min_trade_vol", [](const databento::InstrumentDefMsg& m) { return m.min_trade_vol; })
    .method("contract_multiplier", [](const databento::InstrumentDefMsg& m) { return m.contract_multiplier; })
    .method("decay_quantity", [](const databento::InstrumentDefMsg& m) { return m.decay_quantity; })
    .method("original_contract_size", [](const databento::InstrumentDefMsg& m) { return m.original_contract_size; })
    .method("trading_reference_date", [](const databento::InstrumentDefMsg& m) { return m.trading_reference_date; })
    .method("maturity_year", [](const databento::InstrumentDefMsg& m) { return m.maturity_year; })
    .method("decay_start_date", [](const databento::InstrumentDefMsg& m) { return m.decay_start_date; })
    .method("channel_id", [](const databento::InstrumentDefMsg& m) { return m.channel_id; })
    .method("currency", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Currency(); })
    .method("settl_currency", [](const databento::InstrumentDefMsg& m) -> std::string { return m.SettlCurrency(); })
    .method("secsubtype", [](const databento::InstrumentDefMsg& m) -> std::string { return m.SecSubType(); })
    .method("raw_symbol", [](const databento::InstrumentDefMsg& m) -> std::string { return m.RawSymbol(); })
    .method("group", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Group(); })
    .method("exchange", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Exchange(); })
    .method("asset", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Asset(); })
    .method("cfi", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Cfi(); })
    .method("security_type", [](const databento::InstrumentDefMsg& m) -> std::string { return m.SecurityType(); })
    .method("unit_of_measure", [](const databento::InstrumentDefMsg& m) -> std::string { return m.UnitOfMeasure(); })
    .method("underlying", [](const databento::InstrumentDefMsg& m) -> std::string { return m.Underlying(); })
    .method("strike_price_currency", [](const databento::InstrumentDefMsg& m) -> std::string { return m.StrikePriceCurrency(); })
    .method("index_ts", [](const databento::InstrumentDefMsg& m) { return m.IndexTs(); })
    .method("to_string", [](const databento::InstrumentDefMsg& m) -> std::string {
      return databento::ToString(m);
    });

  // ImbalanceMsg - Imbalance message
  mod.add_type<databento::ImbalanceMsg>("ImbalanceMsg")
    .method("hd", [](const databento::ImbalanceMsg& m) { return m.hd; })
    .method("ts_recv", [](const databento::ImbalanceMsg& m) { return m.ts_recv; })
    .method("ref_price", [](const databento::ImbalanceMsg& m) { return m.ref_price; })
    .method("auction_time", [](const databento::ImbalanceMsg& m) { return m.auction_time; })
    .method("cont_book_clr_price", [](const databento::ImbalanceMsg& m) { return m.cont_book_clr_price; })
    .method("auct_interest_clr_price", [](const databento::ImbalanceMsg& m) { return m.auct_interest_clr_price; })
    .method("ssr_filling_price", [](const databento::ImbalanceMsg& m) { return m.ssr_filling_price; })
    .method("ind_match_price", [](const databento::ImbalanceMsg& m) { return m.ind_match_price; })
    .method("upper_collar", [](const databento::ImbalanceMsg& m) { return m.upper_collar; })
    .method("lower_collar", [](const databento::ImbalanceMsg& m) { return m.lower_collar; })
    .method("paired_qty", [](const databento::ImbalanceMsg& m) { return m.paired_qty; })
    .method("total_imbalance_qty", [](const databento::ImbalanceMsg& m) { return m.total_imbalance_qty; })
    .method("market_imbalance_qty", [](const databento::ImbalanceMsg& m) { return m.market_imbalance_qty; })
    .method("unpaired_qty", [](const databento::ImbalanceMsg& m) { return m.unpaired_qty; })
    .method("side", [](const databento::ImbalanceMsg& m) { return m.side; })
    .method("auction_status", [](const databento::ImbalanceMsg& m) { return static_cast<std::uint32_t>(m.auction_status); })
    .method("index_ts", [](const databento::ImbalanceMsg& m) { return m.IndexTs(); })
    .method("to_string", [](const databento::ImbalanceMsg& m) -> std::string {
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
    .method("metadata_list_fields", [](databento::Historical& client, databento::Encoding encoding, databento::Schema schema) -> std::vector<databento::FieldDetail> {
      return client.MetadataListFields(encoding, schema);
    })
    // Symbology methods
    .method("symbology_resolve", [](databento::Historical& client,
                                     const std::string& dataset,
                                     const std::vector<std::string>& symbols,
                                     databento::SType stype_in,
                                     databento::SType stype_out,
                                     const std::string& start_date,
                                     const std::string& end_date) -> databento::SymbologyResolution {
      return client.SymbologyResolve(dataset, symbols, stype_in, stype_out, databento::DateRange{start_date, end_date});
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
                                                std::uint64_t limit,
                                                const std::string& output_file) -> databento::DbnFileStore {
      return client.TimeseriesGetRangeToFile(dataset, databento::DateTimeRange<std::string>{start, end}, symbols, schema, stype_in, stype_out, limit, output_file);
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

  // FieldDetail mapping
  mod.add_type<databento::FieldDetail>("FieldDetail")
    .method("name", [](const databento::FieldDetail& f) { return f.name; })
    .method("type", [](const databento::FieldDetail& f) { return f.type; });

  // SymbologyResolution mapping
  mod.add_type<databento::SymbologyResolution>("SymbologyResolution");

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
    .method("start_nanos", [](const databento::Metadata& m) -> std::uint64_t {
      return m.start.time_since_epoch().count();
    })
    .method("end_nanos", [](const databento::Metadata& m) -> std::uint64_t {
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
