#include <jlcxx/jlcxx.hpp>
#include <databento/enums.hpp>
#include <databento/record.hpp>

namespace jlcxx {
  // Enums are true scalars - OK for IsMirroredType
  template<> struct IsMirroredType<databento::Schema> : std::true_type {};
  template<> struct IsMirroredType<databento::Encoding> : std::true_type {};
  template<> struct IsMirroredType<databento::SType> : std::true_type {};
  template<> struct IsMirroredType<databento::RType> : std::true_type {};
  template<> struct IsMirroredType<databento::Action> : std::true_type {};
  template<> struct IsMirroredType<databento::Side> : std::true_type {};

  // Message types - NOT mirrored, use add_type with methods instead
  // This allows CxxWrap to marshal complex types with chrono members
  template<> struct IsMirroredType<databento::BidAskPair> : std::false_type {};
}

JLCXX_MODULE define_databento_module(jlcxx::Module& mod)
{
  // ============================================================================
  // PHASE 1: Core Enums Only
  // ============================================================================

  // Schema Enum - Market data schemas
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
  mod.add_bits<databento::SType>("SType");
  mod.set_const("INSTRUMENT_ID", databento::SType::InstrumentId);
  mod.set_const("RAW_SYMBOL", databento::SType::RawSymbol);
  mod.set_const("CONTINUOUS", databento::SType::Continuous);
  mod.set_const("PARENT", databento::SType::Parent);
  mod.set_const("NASDAQ_SYMBOL", databento::SType::NasdaqSymbol);
  mod.set_const("CMS_SYMBOL", databento::SType::CmsSymbol);

  // RType Enum - Record Type Identifiers
  mod.add_bits<databento::RType>("RType");
  mod.set_const("RTYPE_MBP0", databento::RType::Mbp0);
  mod.set_const("RTYPE_MBP1", databento::RType::Mbp1);
  mod.set_const("RTYPE_MBP10", databento::RType::Mbp10);
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
  // PHASE 3: Status & Status-related Enums
  // ============================================================================

  // StatusAction Enum - Trading status action
  mod.add_bits<databento::StatusAction>("StatusAction");
  mod.set_const("STATUS_ACTION_NONE", databento::StatusAction::None);
  mod.set_const("STATUS_ACTION_PRE_OPEN", databento::StatusAction::PreOpen);
  mod.set_const("STATUS_ACTION_PRE_CROSS", databento::StatusAction::PreCross);
  mod.set_const("STATUS_ACTION_QUOTING", databento::StatusAction::Quoting);
  mod.set_const("STATUS_ACTION_CROSS", databento::StatusAction::Cross);
  mod.set_const("STATUS_ACTION_ROTATION", databento::StatusAction::Rotation);
  mod.set_const("STATUS_ACTION_NEW_PRICE_INDICATION", databento::StatusAction::NewPriceIndication);
  mod.set_const("STATUS_ACTION_TRADING", databento::StatusAction::Trading);
  mod.set_const("STATUS_ACTION_HALT", databento::StatusAction::Halt);
  mod.set_const("STATUS_ACTION_PAUSE", databento::StatusAction::Pause);
  mod.set_const("STATUS_ACTION_SUSPEND", databento::StatusAction::Suspend);
  mod.set_const("STATUS_ACTION_PRE_CLOSE", databento::StatusAction::PreClose);
  mod.set_const("STATUS_ACTION_CLOSE", databento::StatusAction::Close);
  mod.set_const("STATUS_ACTION_POST_CLOSE", databento::StatusAction::PostClose);
  mod.set_const("STATUS_ACTION_SSR_CHANGE", databento::StatusAction::SsrChange);
  mod.set_const("STATUS_ACTION_NOT_AVAILABLE", databento::StatusAction::NotAvailableForTrading);

  // StatusReason Enum - Reason for status change
  mod.add_bits<databento::StatusReason>("StatusReason");
  mod.set_const("STATUS_REASON_NONE", databento::StatusReason::None);
  mod.set_const("STATUS_REASON_SCHEDULED", databento::StatusReason::Scheduled);
  mod.set_const("STATUS_REASON_SURVEILLANCE", databento::StatusReason::SurveillanceIntervention);
  mod.set_const("STATUS_REASON_MARKET_EVENT", databento::StatusReason::MarketEvent);
  mod.set_const("STATUS_REASON_INSTRUMENT_ACTIVATION", databento::StatusReason::InstrumentActivation);

  // TradingEvent Enum - Market trading event
  mod.add_bits<databento::TradingEvent>("TradingEvent");
  mod.set_const("TRADING_EVENT_NONE", databento::TradingEvent::None);
  mod.set_const("TRADING_EVENT_NO_CANCEL", databento::TradingEvent::NoCancel);
  mod.set_const("TRADING_EVENT_CHANGE_SESSION", databento::TradingEvent::ChangeTradingSession);
  mod.set_const("TRADING_EVENT_IMPLIED_MATCHING_ON", databento::TradingEvent::ImpliedMatchingOn);
  mod.set_const("TRADING_EVENT_IMPLIED_MATCHING_OFF", databento::TradingEvent::ImpliedMatchingOff);

  // TriState Enum - Three-state boolean
  mod.add_bits<databento::TriState>("TriState");
  mod.set_const("TRISTATE_YES", databento::TriState::Yes);
  mod.set_const("TRISTATE_NO", databento::TriState::No);
  mod.set_const("TRISTATE_NOT_AVAILABLE", databento::TriState::NotAvailable);

  // ============================================================================
  // PHASE 2: Market Data Message Types
  // ============================================================================

  // MboMsg - Market-by-order message
  mod.add_type<databento::MboMsg>("MboMsg");
  // Header accessors
  mod.method("hd_length", [](const databento::MboMsg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::MboMsg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::MboMsg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::MboMsg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::MboMsg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("order_id", [](const databento::MboMsg& msg) { return msg.order_id; });
  mod.method("price", [](const databento::MboMsg& msg) { return msg.price; });
  mod.method("size", [](const databento::MboMsg& msg) { return msg.size; });
  mod.method("channel_id", [](const databento::MboMsg& msg) { return msg.channel_id; });
  mod.method("action", [](const databento::MboMsg& msg) { return msg.action; });
  mod.method("side", [](const databento::MboMsg& msg) { return msg.side; });
  mod.method("ts_recv", [](const databento::MboMsg& msg) { return msg.ts_recv.time_since_epoch().count(); });
  mod.method("ts_in_delta", [](const databento::MboMsg& msg) { return msg.ts_in_delta.count(); });
  mod.method("sequence", [](const databento::MboMsg& msg) { return msg.sequence; });

  // TradeMsg - Trade message (Mbp0)
  mod.add_type<databento::TradeMsg>("TradeMsg");
  // Header accessors
  mod.method("hd_length", [](const databento::TradeMsg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::TradeMsg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::TradeMsg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::TradeMsg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::TradeMsg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("price", [](const databento::TradeMsg& msg) { return msg.price; });
  mod.method("size", [](const databento::TradeMsg& msg) { return msg.size; });
  mod.method("action", [](const databento::TradeMsg& msg) { return msg.action; });
  mod.method("side", [](const databento::TradeMsg& msg) { return msg.side; });
  mod.method("depth", [](const databento::TradeMsg& msg) { return msg.depth; });
  mod.method("ts_recv", [](const databento::TradeMsg& msg) { return msg.ts_recv.time_since_epoch().count(); });
  mod.method("ts_in_delta", [](const databento::TradeMsg& msg) { return msg.ts_in_delta.count(); });
  mod.method("sequence", [](const databento::TradeMsg& msg) { return msg.sequence; });

  // OhlcvMsg - OHLCV (Open/High/Low/Close/Volume) data
  mod.add_type<databento::OhlcvMsg>("OhlcvMsg");
  // Header accessors
  mod.method("hd_length", [](const databento::OhlcvMsg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::OhlcvMsg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::OhlcvMsg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::OhlcvMsg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::OhlcvMsg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("open", [](const databento::OhlcvMsg& msg) { return msg.open; });
  mod.method("high", [](const databento::OhlcvMsg& msg) { return msg.high; });
  mod.method("low", [](const databento::OhlcvMsg& msg) { return msg.low; });
  mod.method("close", [](const databento::OhlcvMsg& msg) { return msg.close; });
  mod.method("volume", [](const databento::OhlcvMsg& msg) { return msg.volume; });

  // ImbalanceMsg - Imbalance information
  mod.add_type<databento::ImbalanceMsg>("ImbalanceMsg");
  // Header accessors
  mod.method("hd_length", [](const databento::ImbalanceMsg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::ImbalanceMsg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::ImbalanceMsg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::ImbalanceMsg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::ImbalanceMsg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("ts_recv", [](const databento::ImbalanceMsg& msg) { return msg.ts_recv.time_since_epoch().count(); });
  mod.method("ref_price", [](const databento::ImbalanceMsg& msg) { return msg.ref_price; });
  mod.method("auction_time", [](const databento::ImbalanceMsg& msg) { return msg.auction_time.time_since_epoch().count(); });
  mod.method("cont_book_clr_price", [](const databento::ImbalanceMsg& msg) { return msg.cont_book_clr_price; });
  mod.method("auct_interest_clr_price", [](const databento::ImbalanceMsg& msg) { return msg.auct_interest_clr_price; });
  mod.method("ssr_filling_price", [](const databento::ImbalanceMsg& msg) { return msg.ssr_filling_price; });
  mod.method("ind_match_price", [](const databento::ImbalanceMsg& msg) { return msg.ind_match_price; });
  mod.method("upper_collar", [](const databento::ImbalanceMsg& msg) { return msg.upper_collar; });
  mod.method("lower_collar", [](const databento::ImbalanceMsg& msg) { return msg.lower_collar; });
  mod.method("paired_qty", [](const databento::ImbalanceMsg& msg) { return msg.paired_qty; });
  mod.method("total_imbalance_qty", [](const databento::ImbalanceMsg& msg) { return msg.total_imbalance_qty; });
  mod.method("market_imbalance_qty", [](const databento::ImbalanceMsg& msg) { return msg.market_imbalance_qty; });
  mod.method("unpaired_qty", [](const databento::ImbalanceMsg& msg) { return msg.unpaired_qty; });
  mod.method("auction_type", [](const databento::ImbalanceMsg& msg) { return msg.auction_type; });
  mod.method("side", [](const databento::ImbalanceMsg& msg) { return msg.side; });
  mod.method("auction_status", [](const databento::ImbalanceMsg& msg) { return msg.auction_status; });
  mod.method("freeze_status", [](const databento::ImbalanceMsg& msg) { return msg.freeze_status; });
  mod.method("num_extensions", [](const databento::ImbalanceMsg& msg) { return msg.num_extensions; });
  mod.method("unpaired_side", [](const databento::ImbalanceMsg& msg) { return msg.unpaired_side; });
  mod.method("significant_imbalance", [](const databento::ImbalanceMsg& msg) { return msg.significant_imbalance; });

  // StatMsg - Statistics messages
  mod.add_type<databento::StatMsg>("StatMsg");
  // Header accessors
  mod.method("hd_length", [](const databento::StatMsg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::StatMsg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::StatMsg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::StatMsg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::StatMsg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("ts_recv", [](const databento::StatMsg& msg) { return msg.ts_recv.time_since_epoch().count(); });
  mod.method("ts_ref", [](const databento::StatMsg& msg) { return msg.ts_ref.time_since_epoch().count(); });
  mod.method("price", [](const databento::StatMsg& msg) { return msg.price; });
  mod.method("quantity", [](const databento::StatMsg& msg) { return msg.quantity; });
  mod.method("sequence", [](const databento::StatMsg& msg) { return msg.sequence; });
  mod.method("ts_in_delta", [](const databento::StatMsg& msg) { return msg.ts_in_delta.count(); });
  mod.method("stat_type", [](const databento::StatMsg& msg) { return static_cast<uint8_t>(msg.stat_type); });
  mod.method("channel_id", [](const databento::StatMsg& msg) { return msg.channel_id; });
  mod.method("update_action", [](const databento::StatMsg& msg) { return static_cast<uint8_t>(msg.update_action); });

  // ============================================================================
  // PHASE 3: Market-by-Price Message Types with Bid/Ask Levels
  // ============================================================================

  // BidAskPair - Bid/Ask level data (used in Mbp1, Mbp10, etc.)
  mod.add_type<databento::BidAskPair>("BidAskPair");
  mod.method("bid_px", [](const databento::BidAskPair& level) { return level.bid_px; });
  mod.method("ask_px", [](const databento::BidAskPair& level) { return level.ask_px; });
  mod.method("bid_sz", [](const databento::BidAskPair& level) { return level.bid_sz; });
  mod.method("ask_sz", [](const databento::BidAskPair& level) { return level.ask_sz; });
  mod.method("bid_ct", [](const databento::BidAskPair& level) { return level.bid_ct; });
  mod.method("ask_ct", [](const databento::BidAskPair& level) { return level.ask_ct; });

  // Mbp1Msg - Market-by-price level 1 (with 1 bid/ask level)
  mod.add_type<databento::Mbp1Msg>("Mbp1Msg");
  // Header accessors
  mod.method("hd_length", [](const databento::Mbp1Msg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::Mbp1Msg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::Mbp1Msg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::Mbp1Msg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::Mbp1Msg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("price", [](const databento::Mbp1Msg& msg) { return msg.price; });
  mod.method("size", [](const databento::Mbp1Msg& msg) { return msg.size; });
  mod.method("action", [](const databento::Mbp1Msg& msg) { return msg.action; });
  mod.method("side", [](const databento::Mbp1Msg& msg) { return msg.side; });
  mod.method("depth", [](const databento::Mbp1Msg& msg) { return msg.depth; });
  mod.method("ts_recv", [](const databento::Mbp1Msg& msg) { return msg.ts_recv.time_since_epoch().count(); });
  mod.method("ts_in_delta", [](const databento::Mbp1Msg& msg) { return msg.ts_in_delta.count(); });
  mod.method("sequence", [](const databento::Mbp1Msg& msg) { return msg.sequence; });
  // Level 0 accessors (Mbp1 has 1 level)
  mod.method("level_0_bid_px", [](const databento::Mbp1Msg& msg) { return msg.levels[0].bid_px; });
  mod.method("level_0_ask_px", [](const databento::Mbp1Msg& msg) { return msg.levels[0].ask_px; });
  mod.method("level_0_bid_sz", [](const databento::Mbp1Msg& msg) { return msg.levels[0].bid_sz; });
  mod.method("level_0_ask_sz", [](const databento::Mbp1Msg& msg) { return msg.levels[0].ask_sz; });
  mod.method("level_0_bid_ct", [](const databento::Mbp1Msg& msg) { return msg.levels[0].bid_ct; });
  mod.method("level_0_ask_ct", [](const databento::Mbp1Msg& msg) { return msg.levels[0].ask_ct; });

  // Mbp10Msg - Market-by-price level 10 (with 10 bid/ask levels)
  mod.add_type<databento::Mbp10Msg>("Mbp10Msg");
  // Header accessors
  mod.method("hd_length", [](const databento::Mbp10Msg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::Mbp10Msg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::Mbp10Msg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::Mbp10Msg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::Mbp10Msg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("price", [](const databento::Mbp10Msg& msg) { return msg.price; });
  mod.method("size", [](const databento::Mbp10Msg& msg) { return msg.size; });
  mod.method("action", [](const databento::Mbp10Msg& msg) { return msg.action; });
  mod.method("side", [](const databento::Mbp10Msg& msg) { return msg.side; });
  mod.method("depth", [](const databento::Mbp10Msg& msg) { return msg.depth; });
  mod.method("ts_recv", [](const databento::Mbp10Msg& msg) { return msg.ts_recv.time_since_epoch().count(); });
  mod.method("ts_in_delta", [](const databento::Mbp10Msg& msg) { return msg.ts_in_delta.count(); });
  mod.method("sequence", [](const databento::Mbp10Msg& msg) { return msg.sequence; });
  // Level accessors for all 10 levels
  for (int i = 0; i < 10; ++i) {
    std::string level_num = std::to_string(i);
    mod.method("level_" + level_num + "_bid_px", [i](const databento::Mbp10Msg& msg) { return msg.levels[i].bid_px; });
    mod.method("level_" + level_num + "_ask_px", [i](const databento::Mbp10Msg& msg) { return msg.levels[i].ask_px; });
    mod.method("level_" + level_num + "_bid_sz", [i](const databento::Mbp10Msg& msg) { return msg.levels[i].bid_sz; });
    mod.method("level_" + level_num + "_ask_sz", [i](const databento::Mbp10Msg& msg) { return msg.levels[i].ask_sz; });
    mod.method("level_" + level_num + "_bid_ct", [i](const databento::Mbp10Msg& msg) { return msg.levels[i].bid_ct; });
    mod.method("level_" + level_num + "_ask_ct", [i](const databento::Mbp10Msg& msg) { return msg.levels[i].ask_ct; });
  }

  // StatusMsg - Trading status message
  mod.add_type<databento::StatusMsg>("StatusMsg");
  // Header accessors
  mod.method("hd_length", [](const databento::StatusMsg& msg) { return msg.hd.length; });
  mod.method("hd_rtype", [](const databento::StatusMsg& msg) { return msg.hd.rtype; });
  mod.method("hd_publisher_id", [](const databento::StatusMsg& msg) { return msg.hd.publisher_id; });
  mod.method("hd_instrument_id", [](const databento::StatusMsg& msg) { return msg.hd.instrument_id; });
  mod.method("hd_ts_event", [](const databento::StatusMsg& msg) { return msg.hd.ts_event.time_since_epoch().count(); });
  // Data accessors
  mod.method("ts_recv", [](const databento::StatusMsg& msg) { return msg.ts_recv.time_since_epoch().count(); });
  mod.method("action", [](const databento::StatusMsg& msg) { return msg.action; });
  mod.method("reason", [](const databento::StatusMsg& msg) { return msg.reason; });
  mod.method("trading_event", [](const databento::StatusMsg& msg) { return msg.trading_event; });
  mod.method("is_trading", [](const databento::StatusMsg& msg) { return msg.is_trading; });
  mod.method("is_quoting", [](const databento::StatusMsg& msg) { return msg.is_quoting; });
  mod.method("is_short_sell_restricted", [](const databento::StatusMsg& msg) { return msg.is_short_sell_restricted; });
}
