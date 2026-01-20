#include <jlcxx/jlcxx.hpp>
#include <databento/enums.hpp>
#include <databento/record.hpp>
#include <databento/datetime.hpp>
#include <databento/flag_set.hpp>
#include <databento/historical.hpp>
#include <databento/live.hpp>
#include <databento/live_blocking.hpp>
#include <databento/live_threaded.hpp>
#include <databento/live_subscription.hpp>
#include <databento/batch.hpp>
#include <databento/symbology.hpp>
#include <cstring>
#include <vector>
#include <functional>
#include <optional>
#include <chrono>
#include <unordered_map>

// ============================================================================
// IsMirroredType Template Specializations for Zero-Copy Access
// ============================================================================

namespace jlcxx {
  // Phase 1 Enums
  template<> struct IsMirroredType<databento::Schema> : std::true_type {};
  template<> struct IsMirroredType<databento::Encoding> : std::true_type {};
  template<> struct IsMirroredType<databento::SType> : std::true_type {};

  // Phase 2 Enums
  template<> struct IsMirroredType<databento::RType> : std::true_type {};
  template<> struct IsMirroredType<databento::Action> : std::true_type {};
  template<> struct IsMirroredType<databento::Side> : std::true_type {};

  // Phase 2 Supporting Types
  template<> struct IsMirroredType<databento::FlagSet> : std::true_type {};
  template<> struct IsMirroredType<databento::UnixNanos> : std::true_type {};
  template<> struct IsMirroredType<databento::TimeDeltaNanos> : std::true_type {};
  template<> struct IsMirroredType<databento::BidAskPair> : std::true_type {};

  // Phase 2 Record Types
  template<> struct IsMirroredType<databento::RecordHeader> : std::true_type {};
  template<> struct IsMirroredType<databento::MboMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::TradeMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::Mbp1Msg> : std::true_type {};
  template<> struct IsMirroredType<databento::Mbp10Msg> : std::true_type {};
  template<> struct IsMirroredType<databento::InstrumentDefMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::ImbalanceMsg> : std::true_type {};

  // Phase 4 Additional Message Types
  template<> struct IsMirroredType<databento::StatusMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::OhlcvMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::StatMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::ErrorMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::SymbolMappingMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::SystemMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::BboMsg> : std::true_type {};
  template<> struct IsMirroredType<databento::Cmbp1Msg> : std::true_type {};
  template<> struct IsMirroredType<databento::CbboMsg> : std::true_type {};

  // Phase 4 File Reader Types
  template<> struct IsMirroredType<databento::Metadata> : std::true_type {};
  template<> struct IsMirroredType<databento::Record> : std::true_type {};

  // Phase 5 Live Streaming
  template<> struct IsMirroredType<databento::LiveSubscription> : std::true_type {};

  // Phase 6 Batch/Metadata
  template<> struct IsMirroredType<databento::BatchJob> : std::true_type {};
  template<> struct IsMirroredType<databento::BatchFileDesc> : std::true_type {};
  template<> struct IsMirroredType<databento::PublisherDetail> : std::true_type {};
  template<> struct IsMirroredType<databento::FieldDetail> : std::true_type {};
  template<> struct IsMirroredType<databento::DatasetRange> : std::true_type {};
  template<> struct IsMirroredType<databento::DatasetConditionDetail> : std::true_type {};
}

JLCXX_MODULE define_databento_module(jlcxx::Module& mod)
{
  // ============================================================================
  // PHASE 1: Core Enums (using actual v0.30.0 enum values)
  // ============================================================================

  // Schema Enum - Market data schemas
  // Note: Using only enums that exist in databento-cpp v0.30.0
  mod.add_bits<databento::Schema>("Schema");
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

  // Encoding Enum - Data encoding formats
  mod.add_bits<databento::Encoding>("Encoding");
  mod.set_const("DBN", databento::Encoding::Dbn);
  mod.set_const("CSV", databento::Encoding::Csv);
  mod.set_const("JSON", databento::Encoding::Json);

  // SType Enum - Symbology types
  // Note: v0.30.0 only has these values (not all the ones we had before)
  mod.add_bits<databento::SType>("SType");
  mod.set_const("INSTRUMENT_ID", databento::SType::InstrumentId);
  mod.set_const("RAW_SYMBOL", databento::SType::RawSymbol);
  mod.set_const("CONTINUOUS", databento::SType::Continuous);
  mod.set_const("PARENT", databento::SType::Parent);
  mod.set_const("NASDAQ_SYMBOL", databento::SType::NasdaqSymbol);
  mod.set_const("CMS_SYMBOL", databento::SType::CmsSymbol);

  // ============================================================================
  // PHASE 2: Data Record Protocol (RType, Action, Side, Supporting Types, Messages)
  // ============================================================================

  // RType Enum - Record Type Identifiers
  mod.add_bits<databento::RType>("RType");
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

  // Action Enum - Market Event Actions
  mod.add_bits<databento::Action>("Action");
  mod.set_const("ACTION_MODIFY", databento::Action::Modify);
  mod.set_const("ACTION_TRADE", databento::Action::Trade);
  mod.set_const("ACTION_FILL", databento::Action::Fill);
  mod.set_const("ACTION_CANCEL", databento::Action::Cancel);
  mod.set_const("ACTION_ADD", databento::Action::Add);
  mod.set_const("ACTION_CLEAR", databento::Action::Clear);
  mod.set_const("ACTION_NONE", databento::Action::None);

  // Side Enum - Order Side
  mod.add_bits<databento::Side>("Side");
  mod.set_const("SIDE_ASK", databento::Side::Ask);
  mod.set_const("SIDE_BID", databento::Side::Bid);
  mod.set_const("SIDE_NONE", databento::Side::None);

  // ============================================================================
  // Phase 2: Supporting Types
  // ============================================================================

  // FlagSet - Bit flags
  mod.add_bits<databento::FlagSet>("FlagSet");
  mod.method("is_last", [](const databento::FlagSet& f) { return f.IsLast(); });
  mod.method("is_tob", [](const databento::FlagSet& f) { return f.IsTob(); });
  mod.method("is_snapshot", [](const databento::FlagSet& f) { return f.IsSnapshot(); });
  mod.method("is_mbp", [](const databento::FlagSet& f) { return f.IsMbp(); });
  mod.method("is_bad_ts_recv", [](const databento::FlagSet& f) { return f.IsBadTsRecv(); });
  mod.method("is_maybe_bad_book", [](const databento::FlagSet& f) { return f.IsMaybeBadBook(); });
  mod.method("raw", [](const databento::FlagSet& f) { return f.Raw(); });
  mod.method("any", [](const databento::FlagSet& f) { return f.Any(); });
  mod.method("is_empty", [](const databento::FlagSet& f) { return f.IsEmpty(); });

  // UnixNanos - Timestamp in nanoseconds
  mod.add_bits<databento::UnixNanos>("UnixNanos");
  mod.method("time_since_epoch", [](const databento::UnixNanos& ts) {
    return ts.time_since_epoch().count();
  });

  // TimeDeltaNanos - Time delta in nanoseconds
  mod.add_bits<databento::TimeDeltaNanos>("TimeDeltaNanos");
  mod.method("count", [](const databento::TimeDeltaNanos& td) { return td.count(); });

  // BidAskPair - Bid/Ask price level data
  mod.add_bits<databento::BidAskPair>("BidAskPair");
  mod.method("bid_px", [](const databento::BidAskPair& p) { return p.bid_px; });
  mod.method("ask_px", [](const databento::BidAskPair& p) { return p.ask_px; });
  mod.method("bid_sz", [](const databento::BidAskPair& p) { return p.bid_sz; });
  mod.method("ask_sz", [](const databento::BidAskPair& p) { return p.ask_sz; });
  mod.method("bid_ct", [](const databento::BidAskPair& p) { return p.bid_ct; });
  mod.method("ask_ct", [](const databento::BidAskPair& p) { return p.ask_ct; });

  // RecordHeader - Common header for all DBN records
  mod.add_bits<databento::RecordHeader>("RecordHeader");
  mod.method("length", [](const databento::RecordHeader& h) { return h.length; });
  mod.method("rtype", [](const databento::RecordHeader& h) { return h.rtype; });
  mod.method("publisher_id", [](const databento::RecordHeader& h) { return h.publisher_id; });
  mod.method("instrument_id", [](const databento::RecordHeader& h) { return h.instrument_id; });
  mod.method("ts_event", [](const databento::RecordHeader& h) { return h.ts_event; });
  mod.method("size", &databento::RecordHeader::Size);
  mod.method("publisher", [](const databento::RecordHeader& h) { return h.Publisher(); });

  // ============================================================================
  // Phase 2: Market Message Structures
  // ============================================================================

  // MboMsg - Market By Order
  mod.add_bits<databento::MboMsg>("MboMsg");
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

  // TradeMsg - Trade Messages
  mod.add_bits<databento::TradeMsg>("TradeMsg");
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

  // Mbp1Msg - Market By Price Level 1
  mod.add_bits<databento::Mbp1Msg>("Mbp1Msg");
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

  // Mbp10Msg - Market By Price Level 10
  mod.add_bits<databento::Mbp10Msg>("Mbp10Msg");
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

  // InstrumentDefMsg - Instrument Definition
  mod.add_bits<databento::InstrumentDefMsg>("InstrumentDefMsg");
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
  mod.method("strike_price", [](const databento::InstrumentDefMsg& m) { return m.strike_price; });
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
  mod.method("currency", [](const databento::InstrumentDefMsg& m) { return std::string(m.Currency()); });
  mod.method("settl_currency", [](const databento::InstrumentDefMsg& m) { return std::string(m.SettlCurrency()); });
  mod.method("secsubtype", [](const databento::InstrumentDefMsg& m) { return std::string(m.SecSubType()); });
  mod.method("raw_symbol", [](const databento::InstrumentDefMsg& m) { return std::string(m.RawSymbol()); });
  mod.method("group", [](const databento::InstrumentDefMsg& m) { return std::string(m.Group()); });
  mod.method("exchange", [](const databento::InstrumentDefMsg& m) { return std::string(m.Exchange()); });
  mod.method("asset", [](const databento::InstrumentDefMsg& m) { return std::string(m.Asset()); });
  mod.method("cfi", [](const databento::InstrumentDefMsg& m) { return std::string(m.Cfi()); });
  mod.method("security_type", [](const databento::InstrumentDefMsg& m) { return std::string(m.SecurityType()); });
  mod.method("unit_of_measure", [](const databento::InstrumentDefMsg& m) { return std::string(m.UnitOfMeasure()); });
  mod.method("underlying", [](const databento::InstrumentDefMsg& m) { return std::string(m.Underlying()); });
  mod.method("strike_price_currency", [](const databento::InstrumentDefMsg& m) { return std::string(m.StrikePriceCurrency()); });
  mod.method("instrument_class", [](const databento::InstrumentDefMsg& m) { return m.instrument_class; });
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

  // ImbalanceMsg - Imbalance Messages
  mod.add_bits<databento::ImbalanceMsg>("ImbalanceMsg");
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
  mod.method("freeze_status", [](const databento::ImbalanceMsg& m) { return m.freeze_status; });
  mod.method("num_extensions", [](const databento::ImbalanceMsg& m) { return m.num_extensions; });
  mod.method("unpaired_side", [](const databento::ImbalanceMsg& m) { return m.unpaired_side; });
  mod.method("significant_imbalance", [](const databento::ImbalanceMsg& m) { return m.significant_imbalance; });
  mod.method("index_ts", &databento::ImbalanceMsg::IndexTs);

  // ============================================================================
  // PHASE 3: Historical Client
  // ============================================================================

  // HistoricalBuilder - Builder pattern for constructing Historical client
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
    .method("metadata_list_datasets", [](databento::Historical& client) -> std::vector<std::string> {
      return client.MetadataListDatasets();
    })
    .method("metadata_list_schemas", [](databento::Historical& client, const std::string& dataset) -> std::vector<std::string> {
      return client.MetadataListSchemas(dataset);
    })
    .method("metadata_list_fields", [](databento::Historical& client, databento::Encoding encoding, databento::Schema schema) -> std::vector<std::string> {
      return client.MetadataListFields(encoding, schema);
    })
    .method("symbology_resolve", [](databento::Historical& client,
                                     const std::string& dataset,
                                     const std::vector<std::string>& symbols,
                                     databento::SType stype_in,
                                     const std::string& date) -> std::string {
      return client.SymbologyResolve(dataset, symbols, stype_in, date);
    })
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
  // PHASE 4: DBN File Reader
  // ============================================================================

  // Metadata - DBN file metadata
  mod.add_bits<databento::Metadata>("Metadata");
  mod.method("version", [](const databento::Metadata& m) { return m.version; });
  mod.method("dataset", [](const databento::Metadata& m) { return std::string(m.dataset.data()); });
  mod.method("schema", [](const databento::Metadata& m) { return m.schema; });
  mod.method("start", [](const databento::Metadata& m) { return m.start; });
  mod.method("end", [](const databento::Metadata& m) { return m.end; });
  mod.method("limit", [](const databento::Metadata& m) { return m.limit; });
  mod.method("stype_in", [](const databento::Metadata& m) { return m.stype_in; });
  mod.method("stype_out", [](const databento::Metadata& m) { return m.stype_out; });
  mod.method("ts_out", [](const databento::Metadata& m) { return m.ts_out; });
  mod.method("symbols", [](const databento::Metadata& m) { return m.symbols; });
  mod.method("partial", [](const databento::Metadata& m) { return m.partial; });
  mod.method("not_found", [](const databento::Metadata& m) { return m.not_found; });

  // Record - Universal record wrapper with type checking
  mod.add_bits<databento::Record>("Record");
  mod.method("header", [](const databento::Record& r) { return r.Header(); });
  mod.method("rtype", [](const databento::Record& r) { return r.RType(); });
  mod.method("size", [](const databento::Record& r) { return r.Size(); });
  mod.method("holds_mbo", [](const databento::Record& r) { return r.Holds<databento::MboMsg>(); });
  mod.method("holds_trade", [](const databento::Record& r) { return r.Holds<databento::TradeMsg>(); });
  mod.method("holds_mbp1", [](const databento::Record& r) { return r.Holds<databento::Mbp1Msg>(); });
  mod.method("holds_mbp10", [](const databento::Record& r) { return r.Holds<databento::Mbp10Msg>(); });
  mod.method("holds_ohlcv", [](const databento::Record& r) { return r.Holds<databento::OhlcvMsg>(); });
  mod.method("holds_status", [](const databento::Record& r) { return r.Holds<databento::StatusMsg>(); });
  mod.method("holds_instrument_def", [](const databento::Record& r) { return r.Holds<databento::InstrumentDefMsg>(); });
  mod.method("holds_imbalance", [](const databento::Record& r) { return r.Holds<databento::ImbalanceMsg>(); });
  mod.method("holds_stat", [](const databento::Record& r) { return r.Holds<databento::StatMsg>(); });
  mod.method("get_mbo_if", [](const databento::Record& r) { return r.GetIf<databento::MboMsg>(); });
  mod.method("get_trade_if", [](const databento::Record& r) { return r.GetIf<databento::TradeMsg>(); });
  mod.method("get_mbp1_if", [](const databento::Record& r) { return r.GetIf<databento::Mbp1Msg>(); });
  mod.method("get_mbp10_if", [](const databento::Record& r) { return r.GetIf<databento::Mbp10Msg>(); });
  mod.method("get_ohlcv_if", [](const databento::Record& r) { return r.GetIf<databento::OhlcvMsg>(); });
  mod.method("get_status_if", [](const databento::Record& r) { return r.GetIf<databento::StatusMsg>(); });
  mod.method("get_instrument_def_if", [](const databento::Record& r) { return r.GetIf<databento::InstrumentDefMsg>(); });
  mod.method("get_imbalance_if", [](const databento::Record& r) { return r.GetIf<databento::ImbalanceMsg>(); });
  mod.method("get_stat_if", [](const databento::Record& r) { return r.GetIf<databento::StatMsg>(); });

  // DbnFileStore - Main file reader
  mod.add_type<databento::DbnFileStore>("DbnFileStore")
    .constructor<const std::string&>()
    .method("get_metadata", [](databento::DbnFileStore& store) -> const databento::Metadata& {
      return store.GetMetadata();
    })
    .method("next_record", [](databento::DbnFileStore& store) -> databento::Record {
      return store.NextRecord();
    });

  // ============================================================================
  // PHASE 5: Live Streaming API
  // ============================================================================

  // LiveSubscription - Subscription specification
  mod.add_bits<databento::LiveSubscription>("LiveSubscription");
  mod.method("set_symbols!", [](databento::LiveSubscription& sub,
                                 const std::vector<std::string>& symbols) -> databento::LiveSubscription& {
    sub.symbols = symbols;
    return sub;
  });
  mod.method("set_schema!", [](databento::LiveSubscription& sub,
                                databento::Schema schema) -> databento::LiveSubscription& {
    sub.schema = schema;
    return sub;
  });
  mod.method("set_stype!", [](databento::LiveSubscription& sub,
                               databento::SType stype) -> databento::LiveSubscription& {
    sub.stype_in = stype;
    return sub;
  });
  mod.method("symbols", [](const databento::LiveSubscription& sub) {
    return sub.symbols;
  });
  mod.method("schema", [](const databento::LiveSubscription& sub) {
    return sub.schema;
  });
  mod.method("stype_in", [](const databento::LiveSubscription& sub) {
    return sub.stype_in;
  });

  // LiveBuilder - Builder pattern for Live clients
  mod.add_type<databento::LiveBuilder>("LiveBuilder")
    .constructor<>()
    .method("set_key!", [](databento::LiveBuilder& builder,
                           const std::string& key) -> databento::LiveBuilder& {
      return builder.SetKey(key);
    })
    .method("set_key_from_env!", [](databento::LiveBuilder& builder) -> databento::LiveBuilder& {
      return builder.SetKeyFromEnv();
    })
    .method("set_dataset!", [](databento::LiveBuilder& builder,
                               const std::string& dataset) -> databento::LiveBuilder& {
      return builder.SetDataset(dataset);
    })
    .method("set_send_ts_out!", [](databento::LiveBuilder& builder,
                                   bool send) -> databento::LiveBuilder& {
      return builder.SetSendTsOut(send);
    })
    .method("set_heartbeat_interval!", [](databento::LiveBuilder& builder,
                                         uint64_t interval_ms) -> databento::LiveBuilder& {
      return builder.SetHeartbeatInterval(std::chrono::milliseconds(interval_ms));
    })
    .method("build_blocking", [](databento::LiveBuilder& builder) -> databento::LiveBlocking {
      return builder.BuildBlocking();
    })
    .method("build_threaded", [](databento::LiveBuilder& builder) -> databento::LiveThreaded {
      return builder.BuildThreaded();
    });

  // LiveBlocking - Blocking/polling real-time API
  mod.add_type<databento::LiveBlocking>("LiveBlocking")
    .constructor<>()
    .method("subscribe!", [](databento::LiveBlocking& live,
                             const databento::LiveSubscription& sub) {
      live.Subscribe(sub);
      return std::ref(live);
    })
    .method("start!", [](databento::LiveBlocking& live) {
      live.Start();
      return std::ref(live);
    })
    .method("stop!", [](databento::LiveBlocking& live) {
      live.Stop();
      return std::ref(live);
    })
    .method("reconnect!", [](databento::LiveBlocking& live) {
      live.Reconnect();
      return std::ref(live);
    })
    .method("next_record", [](databento::LiveBlocking& live) -> databento::Record {
      return live.NextRecord();
    })
    .method("next_record_timeout", [](databento::LiveBlocking& live,
                                      uint64_t timeout_ms) -> std::optional<databento::Record> {
      auto result = live.NextRecord(std::chrono::milliseconds(timeout_ms));
      if (result) return *result;
      return std::nullopt;
    })
    .method("is_subscribed", [](const databento::LiveBlocking& live) {
      return !live.Subscriptions().empty();
    });

  // LiveThreaded - Event-driven threaded real-time API
  mod.add_type<databento::LiveThreaded>("LiveThreaded")
    .constructor<>()
    .method("subscribe!", [](databento::LiveThreaded& live,
                             const databento::LiveSubscription& sub) {
      live.Subscribe(sub);
      return std::ref(live);
    })
    .method("on_record", [](databento::LiveThreaded& live,
                            std::function<databento::KeepGoing(const databento::Record&)> cb) {
      live.Start(cb);
    })
    .method("on_record_and_metadata", [](databento::LiveThreaded& live,
                                         std::function<databento::KeepGoing(const databento::Record&)> rec_cb,
                                         std::function<void(databento::Metadata&&)> meta_cb) {
      live.Start(rec_cb, meta_cb);
    })
    .method("start!", [](databento::LiveThreaded& live) {
      return std::ref(live);
    })
    .method("block_for_stop", [](databento::LiveThreaded& live) {
      live.BlockForStop();
    })
    .method("block_for_stop_timeout", [](databento::LiveThreaded& live,
                                         uint64_t timeout_ms) -> bool {
      return live.BlockForStop(std::chrono::milliseconds(timeout_ms));
    });

  // ============================================================================
  // PHASE 6: Historical/Batch API
  // ============================================================================

  // PublisherDetail - Publisher metadata
  mod.add_bits<databento::PublisherDetail>("PublisherDetail");
  mod.method("publisher_id", [](const databento::PublisherDetail& p) {
    return p.publisher_id;
  });
  mod.method("dataset", [](const databento::PublisherDetail& p) {
    return std::string(p.dataset.data());
  });
  mod.method("venue", [](const databento::PublisherDetail& p) {
    return std::string(p.venue.data());
  });
  mod.method("description", [](const databento::PublisherDetail& p) {
    return std::string(p.description.data());
  });

  // FieldDetail - Record field metadata
  mod.add_bits<databento::FieldDetail>("FieldDetail");
  mod.method("name", [](const databento::FieldDetail& f) {
    return std::string(f.name.data());
  });
  mod.method("type", [](const databento::FieldDetail& f) {
    return std::string(f.type.data());
  });

  // DatasetRange - Date range availability
  mod.add_bits<databento::DatasetRange>("DatasetRange");
  mod.method("start_date", [](const databento::DatasetRange& r) {
    return std::string(r.start_date.data());
  });
  mod.method("end_date", [](const databento::DatasetRange& r) {
    return std::string(r.end_date.data());
  });

  // DatasetConditionDetail - Dataset quality metadata
  mod.add_bits<databento::DatasetConditionDetail>("DatasetConditionDetail");
  mod.method("date", [](const databento::DatasetConditionDetail& d) {
    return std::string(d.date.data());
  });
  mod.method("condition", [](const databento::DatasetConditionDetail& d) {
    return std::string(d.condition.data());
  });
  mod.method("last_modified_date", [](const databento::DatasetConditionDetail& d) {
    return std::string(d.last_modified_date.data());
  });

  // BatchFileDesc - File metadata within batch job
  mod.add_bits<databento::BatchFileDesc>("BatchFileDesc");
  mod.method("filename", [](const databento::BatchFileDesc& desc) {
    return std::string(desc.filename.data());
  });
  mod.method("size", [](const databento::BatchFileDesc& desc) {
    return desc.size;
  });
  mod.method("hash", [](const databento::BatchFileDesc& desc) {
    return std::string(desc.hash.data());
  });
  mod.method("https_url", [](const databento::BatchFileDesc& desc) {
    return std::string(desc.https_url.data());
  });
  mod.method("ftp_url", [](const databento::BatchFileDesc& desc) {
    return std::string(desc.ftp_url.data());
  });

  // BatchJob - Batch job metadata and status
  mod.add_bits<databento::BatchJob>("BatchJob");
  mod.method("id", [](const databento::BatchJob& job) { return job.id; });
  mod.method("user_id", [](const databento::BatchJob& job) { return job.user_id; });
  mod.method("bill_id", [](const databento::BatchJob& job) { return job.bill_id; });
  mod.method("cost_usd", [](const databento::BatchJob& job) { return job.cost_usd; });
  mod.method("dataset", [](const databento::BatchJob& job) { return std::string(job.dataset.data()); });
  mod.method("state", [](const databento::BatchJob& job) { return std::string(job.state.data()); });
  mod.method("schema", [](const databento::BatchJob& job) { return job.schema; });
  mod.method("encoding", [](const databento::BatchJob& job) { return job.encoding; });
  mod.method("compression", [](const databento::BatchJob& job) { return job.compression; });
  mod.method("stype_in", [](const databento::BatchJob& job) { return job.stype_in; });
  mod.method("stype_out", [](const databento::BatchJob& job) { return job.stype_out; });
  mod.method("ts_received", [](const databento::BatchJob& job) { return job.ts_received; });
  mod.method("ts_queued", [](const databento::BatchJob& job) { return job.ts_queued; });
  mod.method("ts_process_start", [](const databento::BatchJob& job) { return job.ts_process_start; });
  mod.method("ts_process_done", [](const databento::BatchJob& job) { return job.ts_process_done; });
  mod.method("ts_expiration", [](const databento::BatchJob& job) { return job.ts_expiration; });
  mod.method("record_count", [](const databento::BatchJob& job) { return job.record_count; });
  mod.method("billed_size", [](const databento::BatchJob& job) { return job.billed_size; });
  mod.method("actual_size", [](const databento::BatchJob& job) { return job.actual_size; });
  mod.method("package_size", [](const databento::BatchJob& job) { return job.package_size; });
  mod.method("symbols", [](const databento::BatchJob& job) {
    std::vector<std::string> result;
    for (const auto& s : job.symbols) {
      result.push_back(std::string(s.data()));
    }
    return result;
  });

  // SymbologyResolution - Symbol resolution results
  mod.add_type<databento::SymbologyResolution>("SymbologyResolution")
    .method("stype_in", [](const databento::SymbologyResolution& res) {
      return res.stype_in;
    })
    .method("stype_out", [](const databento::SymbologyResolution& res) {
      return res.stype_out;
    })
    .method("mappings", [](const databento::SymbologyResolution& res)
      -> std::vector<std::pair<std::string, std::vector<std::string>>> {
      std::vector<std::pair<std::string, std::vector<std::string>>> result;
      for (const auto& [symbol, intervals] : res.mappings) {
        std::vector<std::string> interval_strs;
        for (const auto& interval : intervals) {
          interval_strs.push_back(std::string(interval.symbol.data()));
        }
        result.push_back({std::string(symbol.data()), interval_strs});
      }
      return result;
    })
    .method("partial", [](const databento::SymbologyResolution& res) {
      std::vector<std::string> result(res.partial.begin(), res.partial.end());
      return result;
    })
    .method("not_found", [](const databento::SymbologyResolution& res) {
      std::vector<std::string> result(res.not_found.begin(), res.not_found.end());
      return result;
    });

  // ========== ENHANCED HISTORICAL CLIENT (Phase 3 → Phase 6) ==========

  // Replace Phase 3 Historical with expanded version
  mod.add_type<databento::Historical>("Historical")
    // ===== METADATA API =====
    .method("metadata_list_publishers", [](databento::Historical& client)
      -> std::vector<databento::PublisherDetail> {
      return client.MetadataListPublishers();
    })
    .method("metadata_list_datasets", [](databento::Historical& client)
      -> std::vector<std::string> {
      return client.MetadataListDatasets();
    })
    .method("metadata_list_datasets_with_range", [](databento::Historical& client,
                                                     const std::string& start_date,
                                                     const std::string& end_date)
      -> std::vector<std::string> {
      databento::DateRange range{start_date, end_date};
      return client.MetadataListDatasets(range);
    })
    .method("metadata_list_schemas", [](databento::Historical& client,
                                        const std::string& dataset)
      -> std::vector<std::string> {
      return client.MetadataListSchemas(dataset);
    })
    .method("metadata_list_fields", [](databento::Historical& client,
                                       databento::Encoding encoding,
                                       databento::Schema schema)
      -> std::vector<databento::FieldDetail> {
      return client.MetadataListFields(encoding, schema);
    })
    .method("metadata_list_unit_prices", [](databento::Historical& client,
                                            const std::string& dataset)
      -> std::vector<std::string> {
      return client.MetadataListUnitPrices(dataset);
    })
    .method("metadata_get_dataset_range", [](databento::Historical& client,
                                             const std::string& dataset)
      -> databento::DatasetRange {
      return client.MetadataGetDatasetRange(dataset);
    })
    .method("metadata_get_dataset_condition", [](databento::Historical& client,
                                                 const std::string& dataset,
                                                 const std::string& start_date,
                                                 const std::string& end_date)
      -> std::vector<databento::DatasetConditionDetail> {
      databento::DateRange range{start_date, end_date};
      return client.MetadataGetDatasetCondition(dataset, range);
    })
    // ===== RECORD COUNT API =====
    .method("metadata_get_record_count", [](databento::Historical& client,
                                            const std::string& dataset,
                                            const std::vector<std::string>& symbols,
                                            databento::Schema schema,
                                            const std::string& start_date,
                                            const std::string& end_date,
                                            databento::SType stype_in,
                                            databento::SType stype_out)
      -> uint64_t {
      databento::DateTimeRange<std::string> range{start_date, end_date};
      return client.MetadataGetRecordCount(dataset, symbols, schema, range,
                                          stype_in, stype_out);
    })
    // ===== BILLING API =====
    .method("metadata_get_billable_size", [](databento::Historical& client,
                                             const std::string& dataset,
                                             const std::vector<std::string>& symbols,
                                             databento::Schema schema,
                                             const std::string& start_date,
                                             const std::string& end_date,
                                             databento::SType stype_in,
                                             databento::SType stype_out,
                                             databento::Encoding encoding)
      -> uint64_t {
      databento::DateTimeRange<std::string> range{start_date, end_date};
      return client.MetadataGetBillableSize(dataset, symbols, schema, range,
                                           stype_in, stype_out, encoding);
    })
    .method("metadata_get_cost", [](databento::Historical& client,
                                    const std::string& dataset,
                                    const std::vector<std::string>& symbols,
                                    databento::Schema schema,
                                    const std::string& start_date,
                                    const std::string& end_date,
                                    databento::SType stype_in,
                                    databento::SType stype_out,
                                    databento::Encoding encoding)
      -> double {
      databento::DateTimeRange<std::string> range{start_date, end_date};
      return client.MetadataGetCost(dataset, symbols, schema, range,
                                   stype_in, stype_out, encoding);
    })
    // ===== SYMBOLOGY API =====
    .method("symbology_resolve", [](databento::Historical& client,
                                    const std::string& dataset,
                                    const std::vector<std::string>& symbols,
                                    databento::SType stype_in,
                                    const std::string& date)
      -> databento::SymbologyResolution {
      return client.SymbologyResolve(dataset, symbols, stype_in, date);
    })
    // ===== BATCH API =====
    .method("batch_submit_job", [](databento::Historical& client,
                                   const std::string& dataset,
                                   const std::vector<std::string>& symbols,
                                   databento::Schema schema,
                                   const std::string& start_date,
                                   const std::string& end_date,
                                   databento::SType stype_in,
                                   databento::SType stype_out,
                                   databento::Encoding encoding)
      -> databento::BatchJob {
      databento::DateTimeRange<std::string> range{start_date, end_date};
      return client.BatchSubmitJob(dataset, symbols, schema, range,
                                  stype_in, stype_out, encoding);
    })
    .method("batch_list_jobs", [](databento::Historical& client)
      -> std::vector<databento::BatchJob> {
      return client.BatchListJobs();
    })
    .method("batch_list_files", [](databento::Historical& client,
                                   const std::string& job_id)
      -> std::vector<databento::BatchFileDesc> {
      return client.BatchListFiles(job_id);
    })
    .method("batch_download_file", [](databento::Historical& client,
                                      const std::string& job_id,
                                      const std::string& filename,
                                      const std::string& output_path)
      -> std::string {
      return client.BatchDownload(job_id, filename, output_path);
    })
    // ===== TIMESERIES API =====
    .method("timeseries_get_range_to_file", [](databento::Historical& client,
                                               const std::string& dataset,
                                               const std::vector<std::string>& symbols,
                                               databento::Schema schema,
                                               const std::string& start_date,
                                               const std::string& end_date,
                                               databento::SType stype_in,
                                               databento::SType stype_out,
                                               databento::Encoding encoding,
                                               const std::string& output_file)
      -> std::string {
      databento::DateTimeRange<std::string> range{start_date, end_date};
      return client.TimeseriesGetRangeToFile(dataset, symbols, schema, range,
                                           stype_in, stype_out, encoding,
                                           output_file);
    })
    // Keep Phase 3 methods for backward compatibility
    .method("metadata_list_schemas_old", [](databento::Historical& client,
                                            const std::string& dataset)
      -> std::vector<std::string> {
      return client.MetadataListSchemas(dataset);
    })
    .method("symbology_resolve_old", [](databento::Historical& client,
                                        const std::string& dataset,
                                        const std::vector<std::string>& symbols,
                                        databento::SType stype_in,
                                        const std::string& date)
      -> databento::SymbologyResolution {
      return client.SymbologyResolve(dataset, symbols, stype_in, date);
    })
    .method("timeseries_get_range_to_file_old", [](databento::Historical& client,
                                                    const std::string& dataset,
                                                    const std::vector<std::string>& symbols,
                                                    databento::Schema schema,
                                                    const std::string& start_date,
                                                    const std::string& end_date,
                                                    databento::SType stype_in,
                                                    databento::SType stype_out,
                                                    const std::string& output_file)
      -> std::string {
      databento::DateTimeRange<std::string> range{start_date, end_date};
      return client.TimeseriesGetRangeToFile(dataset, symbols, schema, range,
                                           stype_in, stype_out,
                                           databento::Encoding::Dbn,
                                           output_file);
    });
}
