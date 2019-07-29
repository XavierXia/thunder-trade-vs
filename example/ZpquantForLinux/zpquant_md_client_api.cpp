#include <iostream>
#include <string.h>
#include "ZpquantMdApi.h"

using namespace std;

namespace Zpquant {


CZpquantMdApi::CZpquantMdApi() {
    pSpi = NULL;
    //memset(sendJsonDataStr, 0, sizeof(sendJsonDataStr)*4096);
}


CZpquantMdApi::~CZpquantMdApi() {
    /* Do nothing */
}

/**
 * 注册spi回调接口
 *
 * @param   pSpi        spi回调接口
 *
 * @retval  TRUE: 设置成功
 * @retval  FALSE: 设置失败
 */
void
CZpquantMdApi::RegisterSpi(CZpquantMdSpi *pSpi) {
    this->pSpi = pSpi;
}

//初始化交易源
void
CZpquantMdApi::InitMdSource(ZpquantUserLoginField* userLogin) {
    
}

//采集报单回调数据
/*
MdsL2TradeT:
...client,mds_data_onTrade...subscribe,topic:mds_data_onTrade,
msg: {"msgType":22, "sendDCT":53533711, "LastRecvT":1564037533,
 "mktData":{"exchId":2,"securityType":1,"tradeDate":20190430,"TransactTime":132421330,
 "ChannelNo":2011,"ApplSeqNum":8976101,"SecurityID":"000001","ExecType":"F","TradeBSFlag":"N",
 "TradePrice":139400,"TradeQty":300,"TradeMoney":41820000,"BidApplSeqNum":8976100,
 "OfferApplSeqNum":8976040,"__origTickSeq":91555155,"__channelNo":8,"__origNetTime":132420994,
 "__recvTime":1564037533.700277,"__collectedTime":1564037533.700277,"__processedTime":1564037533.700333,
 "__pushingTime":1564037533.700370}}
 
MdsL2OrderT:
 ...client,mds_data_onOrder...subscribe,topic:mds_data_onOrder,msg: {"msgType":23, "sendDCT":60222207, 
 "LastRecvT":1564044222, "mktData":{"exchId":2,"securityType":1,"tradeDate":20190430,"TransactTime":141521560,
 "ChannelNo":2011,"ApplSeqNum":11395784,"SecurityID":"000001","Side":"2","OrderType":"2","Price":138400,
 "OrderQty":1000,"__origTickSeq":116227503,"__channelNo":8,"__origNetTime":141521261,"__recvTime":1564044222.199936,
 "__collectedTime":1564044222.199936,"__processedTime":1564044222.199991,"__pushingTime":1564044222.200038}}

MdsMktDataSnapshotT:
...client,mds_data_onTick...subscribe,topic:mds_data_onTick,msg: 
{"msgType":20, "sendDCT":60126251, "LastRecvT":1564044126, 
"mktData":{"head":{"exchId":2,"securityType":1,"tradeDate":20190430,"updateTime":141442000,"mdStreamType":20,
"__origMdSource":5,"__dataVersion":3953,"__origTickSeq":607488764396705094,"__channelNo":4,
"__origNetTime":141442152,"__recvTime":1564044126.244194,"__collectedTime":1564044126.244194,
"__processedTime":1564044126.244213,"__pushingTime":1564044126.244319},"body":{"SecurityID":"002415",
"TradingPhaseCode":"T0      ","NumTrades":35400,"TotalVolumeTraded":26480882,"TotalValueTraded":8615623012700,
"PrevClosePx":320300,"TradePx":327200,"OpenPx":321900,"ClosePx":0,"HighPx":328200,"LowPx":318200,"IOPV":0,"NAV":0,
"TotalLongPosition":0,"TotalBidQty":2365473,"TotalOfferQty":5489346,"WeightedAvgBidPx":318000,
"WeightedAvgOfferPx":336700,"BidPriceLevel":0,"OfferPriceLevel":0,
"bidPrice":[327200,327100,327000,326900,326800,326600,326500,326400,326300,326200],
"bidNumberOfOrders":[3,10,7,2,2,3,7,1,2,1],"bidOrderQty":[6800,22800,6800,800,14500,2400,33900,2600,1500,10000],
"offerPrice":[327300,327400,327500,327600,327700,327800,327900,328000,328100,328200],
"offerNumberOfOrders":[18,13,40,16,10,25,34,154,33,104],
"offerOrderQty":[16430,41200,101700,10500,7400,37100,40091,112384,26300,113700]}}}

*/

bool
CZpquantMdApi::Start() {
    if(!subscriber.connect()) return false;
    if(!publisher.connect()) return false;

    //执行报告消息进行接收和处理
    subscriber.subscribe("mds_data_onTrade", [this](const string& topic, const string& msg) {
      std::cout << "...client,mds_data_onTrade...subscribe,topic:" << topic << ",msg: " << msg << endl;

        ptree c_Config;
        std::stringstream jmsg(msg.c_str());  
        try {
            boost::property_tree::read_json(jmsg, c_Config);
        }
        catch(std::exception & e){
            fprintf(stdout, "cannot parse from string 'msg(mds_data_onTrade)' \n");
            return false;
        }

      uint8 exchId;
      uint8 securityType;
      int32 tradeDate;
      int32 TransactTime;
      int32 ChannelNo;
      int32 ApplSeqNum;
      string SecurityID;

      string ExecType;
      string TradeBSFlag;
      int32 TradePrice;
      int32 TradeQty;

      int64 TradeMoney;
      int64 BidApplSeqNumNode;
      int64 OfferApplSeqNum;

      auto mktDataConfig = c_Config.find("mktData");
      if (mktDataConfig != c_Config.not_found())
      {
        auto exchIdNode = mktDataConfig->second.find("exchId");
        if(!(exchIdNode== mktDataConfig->second.not_found())) exchId = exchIdNode->second.data();

        auto securityTypeNode = mktDataConfig->second.find("securityType");
        if(!(securityTypeNode== mktDataConfig->second.not_found())) securityType = securityTypeNode->second.data();
        auto tradeDateNode = mktDataConfig->second.find("tradeDate");
        if(!(tradeDateNode== mktDataConfig->second.not_found())) tradeDate = tradeDateNode->second.data();
        auto TransactTimeNode = mktDataConfig->second.find("TransactTime");
        if(!(TransactTimeNode== mktDataConfig->second.not_found())) TransactTime = TransactTimeNode->second.data();
        auto ChannelNoNode = mktDataConfig->second.find("ChannelNo");
        if(!(ChannelNoNode== mktDataConfig->second.not_found())) ChannelNo = ChannelNoNode->second.data();
        auto ApplSeqNumNode = mktDataConfig->second.find("ApplSeqNum");
        if(!(ApplSeqNumNode== mktDataConfig->second.not_found())) ApplSeqNum = ApplSeqNumNode->second.data();
        auto SecurityIDNode = mktDataConfig->second.find("SecurityID");
        if(!(SecurityIDNode== mktDataConfig->second.not_found())) SecurityID = SecurityIDNode->second.data();
        auto ExecTypeNode = mktDataConfig->second.find("ExecType");
        if(!(ExecTypeNode== mktDataConfig->second.not_found())) ExecType = ExecTypeNode->second.data();
        auto TradeBSFlagNode = mktDataConfig->second.find("TradeBSFlag");
        if(!(TradeBSFlagNode== mktDataConfig->second.not_found())) TradeBSFlag = TradeBSFlagNode->second.data();
        auto TradePriceNode = mktDataConfig->second.find("TradePrice");
        if(!(TradePriceNode== mktDataConfig->second.not_found())) TradePrice = TradePriceNode->second.data();
        auto TradeQtyNode = mktDataConfig->second.find("TradeQty");
        if(!(TradeQtyNode== mktDataConfig->second.not_found())) TradeQty = TradeQtyNode->second.data();

        auto TradeMoneyNode = mktDataConfig->second.find("TradeMoney");
        if(!(TradeMoneyNode== mktDataConfig->second.not_found())) TradeMoney = TradeMoneyNode->second.data();
        auto BidApplSeqNumNode = mktDataConfig->second.find("BidApplSeqNum");
        if(!(BidApplSeqNumNode== mktDataConfig->second.not_found())) BidApplSeqNum = BidApplSeqNumNode->second.data();
        auto OfferApplSeqNumNode = mktDataConfig->second.find("OfferApplSeqNum");
        if(!(OfferApplSeqNumNode== mktDataConfig->second.not_found())) OfferApplSeqNum = OfferApplSeqNumNode->second.data();
      }

      MdsMktRspMsgBodyT msgBody;
      msgBody.trade.exchId = exchId;
      msgBody.trade.securityType = securityType;
      msgBody.trade.tradeDate = tradeDate;
      msgBody.trade.TransactTime = TransactTime;
      msgBody.trade.ChannelNo = ChannelNo;
      msgBody.trade.ApplSeqNum = ApplSeqNum;
      if (SecurityID) strncpy(msgBody.trade.SecurityID, SecurityID, sizeof(SecurityID) - 1);
      if (ExecType) strncpy(msgBody.trade.ExecType, ExecType, sizeof(ExecType) - 1);
      if (TradeBSFlag) strncpy(msgBody.trade.TradeBSFlag, TradeBSFlag, sizeof(TradeBSFlag) - 1);
      msgBody.trade.TradePrice = TradePrice;
      msgBody.trade.TradeQty = TradeQty;
      msgBody.trade.TradeMoney = TradeMoney;
      msgBody.trade.BidApplSeqNum = BidApplSeqNum;
      msgBody.trade.OfferApplSeqNum = OfferApplSeqNum;

      this->spi.OnTradeRtnDepthMarketData(&msgBody);

    });

    subscriber.subscribe("mds_data_onOrder", [this](const string& topic, const string& msg) {
      std::cout << "...client,mds_data_onOrder...subscribe,topic:" << topic << ",msg: " << msg << endl;

        ptree c_Config;
        std::stringstream jmsg(msg.c_str());  
        try {
            boost::property_tree::read_json(jmsg, c_Config);
        }
        catch(std::exception & e){
            fprintf(stdout, "cannot parse from string 'msg(mds_data_onOrder)' \n");
            return false;
        }

      uint8 exchId;
      uint8 securityType;
      int32 tradeDate;
      int32 TransactTime;
      int32 ChannelNo;
      int32 ApplSeqNum;
      string SecurityID;

      string Side;
      string OrderType;
      int32 Price;
      int32 OrderQty;

      auto mktDataConfig = c_Config.find("mktData");
      if (mktDataConfig != c_Config.not_found())
      {
        auto exchIdNode = mktDataConfig->second.find("exchId");
        if(!(exchIdNode== mktDataConfig->second.not_found())) exchId = exchIdNode->second.data();

        auto securityTypeNode = mktDataConfig->second.find("securityType");
        if(!(securityTypeNode== mktDataConfig->second.not_found())) securityType = securityTypeNode->second.data();
        auto tradeDateNode = mktDataConfig->second.find("tradeDate");
        if(!(tradeDateNode== mktDataConfig->second.not_found())) tradeDate = tradeDateNode->second.data();
        auto TransactTimeNode = mktDataConfig->second.find("TransactTime");
        if(!(TransactTimeNode== mktDataConfig->second.not_found())) TransactTime = TransactTimeNode->second.data();
        auto ChannelNoNode = mktDataConfig->second.find("ChannelNo");
        if(!(ChannelNoNode== mktDataConfig->second.not_found())) ChannelNo = ChannelNoNode->second.data();
        auto ApplSeqNumNode = mktDataConfig->second.find("ApplSeqNum");
        if(!(ApplSeqNumNode== mktDataConfig->second.not_found())) ApplSeqNum = ApplSeqNumNode->second.data();
        auto SecurityIDNode = mktDataConfig->second.find("SecurityID");
        if(!(SecurityIDNode== mktDataConfig->second.not_found())) SecurityID = SecurityIDNode->second.data();

        auto SideNode = mktDataConfig->second.find("Side");
        if(!(SideNode== mktDataConfig->second.not_found())) Side = SideNode->second.data();
        auto OrderTypeNode = mktDataConfig->second.find("OrderType");
        if(!(OrderTypeNode== mktDataConfig->second.not_found())) OrderType = OrderTypeNode->second.data();
        auto PriceNode = mktDataConfig->second.find("Price");
        if(!(PriceNode== mktDataConfig->second.not_found())) Price = PriceNode->second.data();
        auto OrderQtyNode = mktDataConfig->second.find("OrderQty");
        if(!(OrderQtyNode== mktDataConfig->second.not_found())) OrderQty = OrderQtyNode->second.data();
      }

      MdsMktRspMsgBodyT msgBody;
      msgBody.order.exchId = exchId;
      msgBody.order.securityType = securityType;
      msgBody.order.tradeDate = tradeDate;
      msgBody.order.TransactTime = TransactTime;
      msgBody.order.ChannelNo = ChannelNo;
      msgBody.order.ApplSeqNum = ApplSeqNum;

      if (SecurityID) strncpy(msgBody.order.SecurityID, SecurityID, sizeof(SecurityID) - 1);
      if (Side) strncpy(msgBody.order.Side, Side, sizeof(Side) - 1);
      if (OrderType) strncpy(msgBody.order.OrderType, OrderType, sizeof(OrderType) - 1);
      msgBody.order.Price = Price;
      msgBody.order.OrderQty = OrderQty;

      this->spi.OnOrderRtnDepthMarketData(&msgBody);

    });

    subscriber.subscribe("mds_data_onTick", [this](const string& topic, const string& msg) {
      std::cout << "...client,mds_data_onTick...subscribe,topic:" << topic << ",msg: " << msg << endl;

        ptree c_Config;
        std::stringstream jmsg(msg.c_str());  
        try {
            boost::property_tree::read_json(jmsg, c_Config);
        }
        catch(std::exception & e){
            fprintf(stdout, "cannot parse from string 'msg(mds_data_onTick)' \n");
            return false;
        }

      uint8 exchId;
      uint8 securityType;
      int32 tradeDate;
      int32 updateTime;
      int32 mdStreamType;

      string SecurityID;
      string TradingPhaseCode;
      uint64 NumTrades;
      uint64 TotalVolumeTraded;
      int64 TotalValueTraded;
      int32 PrevClosePx;
      int32 OpenPx;
      int32 HighPx;
      int32 LowPx;
      int32 TradePx;
      int32 ClosePx;
      int32 IOPV;
      int32 NAV;
      uint64 TotalLongPosition;
      int64 TotalBidQty;            /**< 委托买入总量 */
      int64 TotalOfferQty;          /**< 委托卖出总量 */
      int32 WeightedAvgBidPx;       /**< 加权平均委买价格 */
      int32 WeightedAvgOfferPx;     /**< 加权平均委卖价格 */
      int32 BidPriceLevel;          
      int32 OfferPriceLevel;
      /** 十档买盘价位信息 */
      MdsPriceLevelEntryT BidLevels[10];
      /** 十档卖盘价位信息 */
      MdsPriceLevelEntryT OfferLevels[10];        


      auto mktDataConfig = c_Config.find("mktData");
      if (mktDataConfig != c_Config.not_found())
      {

        auto head = mktDataConfig->second.find("head");
        auto exchIdTypeNode = head->second.find("exchId");
        if(exchIdTypeNode != head->second.not_found()) exchId = exchIdTypeNode->second.data();
        // if (head->second.find("exchId") != head->second.not_found())
        //     exchId = head->second.find("exchId")->second.data();
        auto securityTypeNode = head->second.find("securityType");
        if(securityTypeNode != head->second.not_found()) securityType = securityTypeNode->second.data();
        auto tradeDateNode = head->second.find("tradeDate");
        if(tradeDateNode != head->second.not_found()) tradeDate = tradeDateNode->second.data();
        auto updateTimeNode = head->second.find("updateTime");
        if(updateTimeNode != head->second.not_found()) updateTime = updateTimeNode->second.data();
        auto mdStreamTypeNode = head->second.find("mdStreamType");
        if(mdStreamTypeNode != head->second.not_found()) mdStreamType = mdStreamTypeNode->second.data();

        auto body = mktDataConfig->second.find("body");

        auto SecurityIDNode = body->second.find("SecurityID");
        if(SecurityIDNode != body->second.not_found()) SecurityID = SecurityIDNode->second.data();
        auto TradingPhaseCodeNode = body->second.find("TradingPhaseCode");
        if(TradingPhaseCodeNode != body->second.not_found()) TradingPhaseCode = TradingPhaseCodeNode->second.data();
        auto NumTradesNode = body->second.find("NumTrades");
        if(NumTradesNode != body->second.not_found()) NumTrades = NumTradesNode->second.data();
        auto TotalVolumeTradedNode = body->second.find("TotalVolumeTraded");
        if(TotalVolumeTradedNode != body->second.not_found()) TotalVolumeTraded = TotalVolumeTradedNode->second.data();
        auto TotalValueTradedNode = body->second.find("TotalValueTraded");
        if(TotalValueTradedNode != body->second.not_found()) TotalValueTraded = TotalValueTradedNode->second.data();
        auto PrevClosePxNode = body->second.find("PrevClosePx");
        if(PrevClosePxNode != body->second.not_found()) PrevClosePx = PrevClosePxNode->second.data();
        auto TradePxNode = body->second.find("TradePx");
        if(TradePxNode != body->second.not_found()) TradePx = TradePxNode->second.data();

        auto OpenPxNode = body->second.find("OpenPx");
        if(OpenPxNode != body->second.not_found()) OpenPx = OpenPxNode->second.data();
        auto ClosePxNode = body->second.find("ClosePx");
        if(ClosePxNode != body->second.not_found()) ClosePx = ClosePxNode->second.data();
        auto HighPxIDNode = body->second.find("HighPx");
        if(HighPxIDNode != body->second.not_found()) HighPx = HighPxIDNode->second.data();

        auto LowPxNode = body->second.find("LowPx");
        if(LowPxNode != body->second.not_found()) LowPx = LowPxNode->second.data();
        auto IOPVNode = body->second.find("IOPV");
        if(IOPVNode != body->second.not_found()) IOPV = IOPVNode->second.data();
        auto NAVNode = body->second.find("NAV");
        if(NAVNode != body->second.not_found()) NAV = NAVNode->second.data();

        auto TotalLongPositionNode = body->second.find("TotalLongPosition");
        if(TotalLongPositionNode != body->second.not_found()) TotalLongPosition = TotalLongPositionNode->second.data();
        auto TotalBidQtyNode = body->second.find("TotalBidQty");
        if(TotalBidQtyNode != body->second.not_found()) TotalBidQty = TotalBidQtyNode->second.data();
        auto TotalOfferQtyNode = body->second.find("TotalOfferQty");
        if(TotalOfferQtyNode != body->second.not_found()) TotalOfferQty = TotalOfferQtyNode->second.data();
        auto WeightedAvgBidPxNode = body->second.find("WeightedAvgBidPx");
        if(WeightedAvgBidPxNode != body->second.not_found()) WeightedAvgBidPx = WeightedAvgBidPxNode->second.data();
        auto WeightedAvgOfferPxNode = body->second.find("WeightedAvgOfferPx");
        if(WeightedAvgOfferPxNode != body->second.not_found()) WeightedAvgOfferPx = WeightedAvgOfferPxNode->second.data();
        auto BidPriceLevelNode = body->second.find("BidPriceLevel");
        if(BidPriceLevelNode != body->second.not_found()) BidPriceLevel = BidPriceLevelNode->second.data();
        auto OfferPriceLevelNode = body->second.find("OfferPriceLevel");
        if(OfferPriceLevelNode != body->second.not_found()) OfferPriceLevel = OfferPriceLevelNode->second.data();

      }

      MdsMktRspMsgBodyT msgBody;
      msgBody.mktDataSnapshot.exchId = exchId;
      msgBody.mktDataSnapshot.securityType = securityType;
      msgBody.mktDataSnapshot.tradeDate = tradeDate;
      msgBody.mktDataSnapshot.updateTime = updateTime;
      msgBody.mktDataSnapshot.ChannelNo = ChannelNo;
      msgBody.mktDataSnapshot.ApplSeqNum = ApplSeqNum;
      msgBody.mktDataSnapshot.mdStreamType = mdStreamType;

      if (SecurityID) strncpy(msgBody.mktDataSnapshot.SecurityID, SecurityID, sizeof(SecurityID) - 1);
      if (TradingPhaseCode) strncpy(msgBody.mktDataSnapshot.TradingPhaseCode, TradingPhaseCode, sizeof(TradingPhaseCode) - 1);

      msgBody.mktDataSnapshot.NumTrades = NumTrades;
      msgBody.mktDataSnapshot.TotalVolumeTraded = TotalVolumeTraded;
      msgBody.mktDataSnapshot.TotalValueTraded = TotalValueTraded;
      msgBody.mktDataSnapshot.PrevClosePx = PrevClosePx;
      msgBody.mktDataSnapshot.TradePx = TradePx;
      msgBody.mktDataSnapshot.OpenPx = OpenPx;
      msgBody.mktDataSnapshot.ClosePx = ClosePx;
      msgBody.mktDataSnapshot.HighPx = HighPx;
      msgBody.mktDataSnapshot.LowPx = LowPx;
      msgBody.mktDataSnapshot.IOPV = IOPV;
      msgBody.mktDataSnapshot.NAV = NAV;
      msgBody.mktDataSnapshot.TotalLongPosition = TotalLongPosition;
      msgBody.mktDataSnapshot.TotalBidQty = TotalBidQty;
      msgBody.mktDataSnapshot.TotalOfferQty = TotalOfferQty;
      msgBody.mktDataSnapshot.WeightedAvgBidPx = WeightedAvgBidPx;
      msgBody.mktDataSnapshot.WeightedAvgOfferPx = WeightedAvgOfferPx;
      msgBody.mktDataSnapshot.BidPriceLevel = BidPriceLevel;
      msgBody.mktDataSnapshot.OfferPriceLevel = OfferPriceLevel;

      ptree child_Bid = pt.get_child("mktData.body.bidPrice");
      int i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.BidLevels[i].Price = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = pt.get_child("mktData.body.bidNumberOfOrders");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.BidLevels[i].NumberOfOrders = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = pt.get_child("mktData.body.bidOrderQty");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.BidLevels[i].OrderQty = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = pt.get_child("mktData.body.offerPrice");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.OfferLevels[i].Price = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = pt.get_child("mktData.body.offerNumberOfOrders");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.OfferLevels[i].NumberOfOrders = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = pt.get_child("mktData.body.offerOrderQty");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.OfferLevels[i].OrderQty = vt.second.get_value<int32>();
        i++;
      }


      this->spi.OnTickRtnDepthMarketData(&msgBody);
    });
    return true;
}

void
CZpquantMdApi::Stop() {
    subscriber.disconnect();
    publisher.disconnect();
}

/**
 * 对执行报告消息进行处理的回调函数
 *
 * @param   pRptChannel     回报通道的会话信息
 * @param   pMsgHead        消息头
 * @param   pMsgBody        消息体数据
 * @param   pCallbackParams 外部传入的参数
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
// int32
// CZpquantTradeApi::HandleReportMsg(OesApiSessionInfoT *pRptChannel,
//         SMsgHeadT *pMsgHead, void *pMsgBody, void *pCallbackParams) {
//     OesClientSpi        *pSpi = (OesClientSpi *) pCallbackParams;
//     OesRspMsgBodyT      *pRspMsg = (OesRspMsgBodyT *) pMsgBody;
//     OesRptMsgT          *pRptMsg = &pRspMsg->rptMsg;

//     assert(pRptChannel && pMsgHead && pRspMsg);

//     switch (pMsgHead->msgId) {
//     case OESMSG_RPT_ORDER_INSERT:               /* OES委托已生成 (已通过风控检查) */
//         pSpi->OnOrderInsert(&pRptMsg->rptBody.ordInsertRsp);
//         break;

//     case OESMSG_RPT_BUSINESS_REJECT:            /* OES业务拒绝 (未通过风控检查等) */
//         pSpi->OnBusinessReject(pRptMsg->rptHead.ordRejReason,
//                 &pRptMsg->rptBody.ordRejectRsp);
//         break;

//     case OESMSG_RPT_ORDER_REPORT:               /* 交易所委托回报 (包括交易所委托拒绝、委托确认和撤单完成通知) */
//         pSpi->OnOrderReport(pRptMsg->rptHead.ordRejReason,
//                 &pRptMsg->rptBody.ordCnfm);
//         break;

//     case OESMSG_RPT_TRADE_REPORT:               /* 交易所成交回报 */
//         pSpi->OnTradeReport(&pRptMsg->rptBody.trdCnfm);
//         break;

//     case OESMSG_RPT_CASH_ASSET_VARIATION:       /* 资金变动信息 */
//         pSpi->OnCashAssetVariation(&pRptMsg->rptBody.cashAssetRpt);
//         break;

//     case OESMSG_RPT_STOCK_HOLDING_VARIATION:    /* 持仓变动信息 (股票) */
//         pSpi->OnStockHoldingVariation(&pRptMsg->rptBody.stkHoldingRpt);
//         break;

//     case OESMSG_RPT_FUND_TRSF_REJECT:           /* 出入金委托响应-业务拒绝 */
//         pSpi->OnFundTrsfReject(pRptMsg->rptHead.ordRejReason,
//                 &pRptMsg->rptBody.fundTrsfRejectRsp);
//         break;

//     case OESMSG_RPT_FUND_TRSF_REPORT:           /* 出入金委托执行报告 */
//         pSpi->OnFundTrsfReport(pRptMsg->rptHead.ordRejReason,
//                 &pRptMsg->rptBody.fundTrsfCnfm);
//         break;

//     case OESMSG_RPT_MARKET_STATE:               /* 市场状态信息 */
//         pSpi->OnMarketState(&pRspMsg->mktStateRpt);
//         break;

//     case OESMSG_RPT_REPORT_SYNCHRONIZATION: /* 回报同步响应 */
//         break;

//     case OESMSG_SESS_HEARTBEAT:
//         break;

//     default:
//         fprintf(stderr, "收到未定义处理方式的回报消息! msgId[0x%02X]\n",
//                 pMsgHead->msgId);
//         break;
//     }

//     return 0;
// }



int 
CZpquantMdApi::SubscribeMarketData(char *ppInstrumentIDStr,ZpquantMdsSubscribeMode mdsSubMode)
{
    char sendJsonDataStr[1024];
    sprintf(sendJsonDataStr, "{\"type\":\"SubscribeMd\",\"codelistStr\":\"%s\",\"mdsSubMode\":\"%d\"}",ppInstrumentIDStr,mdsSubMode);
    std::cout << "...SubscribeMd...SubscribeMarketData: " << sendJsonDataStr << endl;
    publisher.publish("order2server", sendJsonDataStr);

    return 0;
}

/**
 * 获取API的发行版本号
 *
 * @return  API的发行版本号 (如: "0.15.3")
 */
const char *
CZpquantMdApi::GetVersion(void) {
    string vStr = "0.0.1";
    return vStr.c_str();
}

}

