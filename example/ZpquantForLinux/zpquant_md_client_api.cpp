#include <iostream>
#include <string.h>
#include <boost/foreach.hpp>
#include "ZpquantMdApi.h"

void Communicate(const char * address, unsigned int port, const std::stringstream & in, std::stringstream & out);

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
bool
CZpquantMdApi::InitMdSource(ZpquantUserLoginField* userLogin) {
    stringstream in, out;
    boost::property_tree::ptree root, result;
    root.put("type", "reqaddmarketdatasource");
    root.put("sourcetype", "kr_md_quant");
    root.put("username", userLogin->UserID);
    root.put("password", userLogin->UserPassword);
    try {
        boost::property_tree::write_json(in, root);
        Communicate(userLogin->strIP, userLogin->uPort, in, out);
        boost::property_tree::read_json(out, result);
    }
    catch (std::exception & err)
    {
        return false;
    }

    for(auto & node : result)
    {
        // auto MD = CString(CA2W(node.first.c_str()));
        // m_vecMDSource.push_back(MD);
        std::cout << "...InitMdSource,result:" << node.first << endl;
    }

    return true;
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

      if (c_Config.find("mktData") == c_Config.not_found()) return false;
      
      uint8 exchId = c_Config.get<uint8>("mktData.exchId");
      uint8 securityType = c_Config.get<uint8>("mktData.securityType");
      int32 tradeDate = c_Config.get<int32>("mktData.tradeDate");
      int32 TransactTime = c_Config.get<int32>("mktData.TransactTime");
      int32 ChannelNo = c_Config.get<int32>("mktData.ChannelNo");
      int32 ApplSeqNum = c_Config.get<int32>("mktData.ApplSeqNum");
      string SecurityID = c_Config.get<string>("mktData.SecurityID");
      string ExecType = c_Config.get<string>("mktData.ExecType");
      string TradeBSFlag = c_Config.get<string>("mktData.TradeBSFlag");
      int32 TradePrice = c_Config.get<int32>("mktData.TradePrice");
      int32 TradeQty = c_Config.get<int32>("mktData.TradeQty");
      int64 TradeMoney = c_Config.get<int64>("mktData.TradeMoney");
      int64 BidApplSeqNum = c_Config.get<int64>("mktData.BidApplSeqNum");
      int64 OfferApplSeqNum = c_Config.get<int64>("mktData.OfferApplSeqNum");

        // auto securityTypeNode = mktDataConfig->second.find("securityType");
        // if(!(securityTypeNode== mktDataConfig->second.not_found())) securityType = securityTypeNode->second.data();
        // auto tradeDateNode = mktDataConfig->second.find("tradeDate");
        // if(!(tradeDateNode== mktDataConfig->second.not_found())) tradeDate = tradeDateNode->second.data();
        // auto TransactTimeNode = mktDataConfig->second.find("TransactTime");
        // if(!(TransactTimeNode== mktDataConfig->second.not_found())) TransactTime = TransactTimeNode->second.data();
        // auto ChannelNoNode = mktDataConfig->second.find("ChannelNo");
        // if(!(ChannelNoNode== mktDataConfig->second.not_found())) ChannelNo = ChannelNoNode->second.data();
        // auto ApplSeqNumNode = mktDataConfig->second.find("ApplSeqNum");
        // if(!(ApplSeqNumNode== mktDataConfig->second.not_found())) ApplSeqNum = ApplSeqNumNode->second.data();
        // auto SecurityIDNode = mktDataConfig->second.find("SecurityID");
        // if(!(SecurityIDNode== mktDataConfig->second.not_found())) SecurityID = SecurityIDNode->second.data();
        // auto ExecTypeNode = mktDataConfig->second.find("ExecType");
        // if(!(ExecTypeNode== mktDataConfig->second.not_found())) ExecType = ExecTypeNode->second.data();
        // auto TradeBSFlagNode = mktDataConfig->second.find("TradeBSFlag");
        // if(!(TradeBSFlagNode== mktDataConfig->second.not_found())) TradeBSFlag = TradeBSFlagNode->second.data();
        // auto TradePriceNode = mktDataConfig->second.find("TradePrice");
        // if(!(TradePriceNode== mktDataConfig->second.not_found())) TradePrice = TradePriceNode->second.data();
        // auto TradeQtyNode = mktDataConfig->second.find("TradeQty");
        // if(!(TradeQtyNode== mktDataConfig->second.not_found())) TradeQty = TradeQtyNode->second.data();

        // auto TradeMoneyNode = mktDataConfig->second.find("TradeMoney");
        // if(!(TradeMoneyNode== mktDataConfig->second.not_found())) TradeMoney = TradeMoneyNode->second.data();
        // auto BidApplSeqNumNode = mktDataConfig->second.find("BidApplSeqNum");
        // if(!(BidApplSeqNumNode== mktDataConfig->second.not_found())) BidApplSeqNum = BidApplSeqNumNode->second.data();
        // auto OfferApplSeqNumNode = mktDataConfig->second.find("OfferApplSeqNum");
        // if(!(OfferApplSeqNumNode== mktDataConfig->second.not_found())) OfferApplSeqNum = OfferApplSeqNumNode->second.data();

      MdsMktRspMsgBodyT msgBody;
      msgBody.trade.exchId = exchId;
      msgBody.trade.securityType = securityType;
      msgBody.trade.tradeDate = tradeDate;
      msgBody.trade.TransactTime = TransactTime;
      msgBody.trade.ChannelNo = ChannelNo;
      msgBody.trade.ApplSeqNum = ApplSeqNum;
      // if (SecurityID == NULL) strcpy(msgBody.trade.SecurityID, SecurityID.c_str());
      // if (ExecType == NULL) strncpy(msgBody.trade.ExecType, ExecType.c_str());
      // if (TradeBSFlag == NULL) strncpy(msgBody.trade.TradeBSFlag, TradeBSFlag.c_str());
      msgBody.trade.TradePrice = TradePrice;
      msgBody.trade.TradeQty = TradeQty;
      msgBody.trade.TradeMoney = TradeMoney;
      msgBody.trade.BidApplSeqNum = BidApplSeqNum;
      msgBody.trade.OfferApplSeqNum = OfferApplSeqNum;

      this->pSpi->OnTradeRtnDepthMarketData(&msgBody);

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

      auto mktDataConfig = c_Config.find("mktData");
      if (mktDataConfig == c_Config.not_found()) return false;
  
      uint8 exchId = c_Config.get<uint8>("mktData.exchId");
      uint8 securityType = c_Config.get<uint8>("mktData.securityType");
      int32 tradeDate = c_Config.get<int32>("mktData.tradeDate");
      int32 TransactTime = c_Config.get<int32>("mktData.TransactTime");
      int32 ChannelNo = c_Config.get<int32>("mktData.ChannelNo");
      int32 ApplSeqNum = c_Config.get<int32>("mktData.ApplSeqNum");
      string SecurityID = c_Config.get<string>("mktData.SecurityID");

      string Side = c_Config.get<string>("mktData.Side");
      string OrderType = c_Config.get<string>("mktData.OrderType");
      int32 Price = c_Config.get<int32>("mktData.Price");
      int32 OrderQty = c_Config.get<int32>("mktData.OrderQty");

      MdsMktRspMsgBodyT msgBody;
      msgBody.order.exchId = exchId;
      msgBody.order.securityType = securityType;
      msgBody.order.tradeDate = tradeDate;
      msgBody.order.TransactTime = TransactTime;
      msgBody.order.ChannelNo = ChannelNo;
      msgBody.order.ApplSeqNum = ApplSeqNum;

      // if (SecurityID == NULL) strncpy(msgBody.order.SecurityID, SecurityID.c_str(), sizeof(msgBody.order.SecurityID) - 1);
      // if (Side == NULL) strncpy(msgBody.order.Side, Side.c_str(), sizeof(msgBody.order.Side) - 1);
      // if (OrderType == NULL) strncpy(msgBody.order.OrderType, OrderType.c_str(), sizeof(msgBody.order.OrderType) - 1);
      msgBody.order.Price = Price;
      msgBody.order.OrderQty = OrderQty;

      this->pSpi->OnOrderRtnDepthMarketData(&msgBody);

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

      /** 十档买盘价位信息 */
      //MdsPriceLevelEntryT BidLevels[10];
      /** 十档卖盘价位信息 */
      //MdsPriceLevelEntryT OfferLevels[10];        


      auto mktDataConfig = c_Config.find("mktData");
      if (mktDataConfig == c_Config.not_found()) return false;

      uint8 exchId = c_Config.get<uint8>("mktData.head.exchId");
      uint8 securityType = c_Config.get<uint8>("mktData.head.securityType");
      int32 tradeDate = c_Config.get<int32>("mktData.head.tradeDate");
      int32 updateTime = c_Config.get<int32>("mktData.head.updateTime");
      int32 mdStreamType = c_Config.get<int32>("mktData.head.mdStreamType");

      string SecurityID = c_Config.get<string>("mktData.body.SecurityID");
      string TradingPhaseCode = c_Config.get<string>("mktData.body.TradingPhaseCode");
      uint64 NumTrades = c_Config.get<uint64>("mktData.body.NumTrades");
      uint64 TotalVolumeTraded = c_Config.get<uint64>("mktData.body.TotalVolumeTraded");
      int64 TotalValueTraded = c_Config.get<int64>("mktData.body.TotalValueTraded");
      int32 PrevClosePx = c_Config.get<int32>("mktData.body.PrevClosePx");
      int32 OpenPx = c_Config.get<int32>("mktData.body.OpenPx");
      int32 HighPx = c_Config.get<int32>("mktData.body.HighPx");
      int32 LowPx = c_Config.get<int32>("mktData.body.LowPx");
      int32 TradePx = c_Config.get<int32>("mktData.body.TradePx");
      int32 ClosePx = c_Config.get<int32>("mktData.body.ClosePx");
      int32 IOPV = c_Config.get<int32>("mktData.body.IOPV");
      int32 NAV = c_Config.get<int32>("mktData.body.NAV");
      uint64 TotalLongPosition = c_Config.get<uint64>("mktData.body.TotalLongPosition");
      int64 TotalBidQty = c_Config.get<int64>("mktData.body.TotalBidQty");
      int64 TotalOfferQty = c_Config.get<int64>("mktData.body.TotalOfferQty");
      int32 WeightedAvgBidPx = c_Config.get<int32>("mktData.body.WeightedAvgBidPx");
      int32 WeightedAvgOfferPx = c_Config.get<int32>("mktData.body.WeightedAvgOfferPx");
      int32 BidPriceLevel = c_Config.get<int32>("mktData.body.BidPriceLevel");       
      int32 OfferPriceLevel = c_Config.get<int32>("mktData.body.OfferPriceLevel");

      MdsMktRspMsgBodyT msgBody;
      msgBody.mktDataSnapshot.head.exchId = exchId;
      msgBody.mktDataSnapshot.head.securityType = securityType;
      msgBody.mktDataSnapshot.head.tradeDate = tradeDate;
      msgBody.mktDataSnapshot.head.updateTime = updateTime;
      msgBody.mktDataSnapshot.head.mdStreamType = mdStreamType;

      // if (SecurityID == NULL) strncpy(msgBody.mktDataSnapshot.l2Stock.SecurityID, SecurityID.c_str(), sizeof(msgBody.mktDataSnapshot.l2Stock.SecurityID) - 1);
      // if (TradingPhaseCode == NULL) strncpy(msgBody.mktDataSnapshot.l2Stock.TradingPhaseCode, TradingPhaseCode.c_str(), sizeof(msgBody.mktDataSnapshot.l2Stock.TradingPhaseCode) - 1);

      msgBody.mktDataSnapshot.l2Stock.NumTrades = NumTrades;
      msgBody.mktDataSnapshot.l2Stock.TotalVolumeTraded = TotalVolumeTraded;
      msgBody.mktDataSnapshot.l2Stock.TotalValueTraded = TotalValueTraded;
      msgBody.mktDataSnapshot.l2Stock.PrevClosePx = PrevClosePx;
      msgBody.mktDataSnapshot.l2Stock.TradePx = TradePx;
      msgBody.mktDataSnapshot.l2Stock.OpenPx = OpenPx;
      msgBody.mktDataSnapshot.l2Stock.ClosePx = ClosePx;
      msgBody.mktDataSnapshot.l2Stock.HighPx = HighPx;
      msgBody.mktDataSnapshot.l2Stock.LowPx = LowPx;
      msgBody.mktDataSnapshot.l2Stock.IOPV = IOPV;
      msgBody.mktDataSnapshot.l2Stock.NAV = NAV;
      msgBody.mktDataSnapshot.l2Stock.TotalLongPosition = TotalLongPosition;
      msgBody.mktDataSnapshot.l2Stock.TotalBidQty = TotalBidQty;
      msgBody.mktDataSnapshot.l2Stock.TotalOfferQty = TotalOfferQty;
      msgBody.mktDataSnapshot.l2Stock.WeightedAvgBidPx = WeightedAvgBidPx;
      msgBody.mktDataSnapshot.l2Stock.WeightedAvgOfferPx = WeightedAvgOfferPx;
      msgBody.mktDataSnapshot.l2Stock.BidPriceLevel = BidPriceLevel;
      msgBody.mktDataSnapshot.l2Stock.OfferPriceLevel = OfferPriceLevel;

      ptree child_Bid = c_Config.get_child("mktData.body.bidPrice");
      int i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.l2Stock.BidLevels[i].Price = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = c_Config.get_child("mktData.body.bidNumberOfOrders");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.l2Stock.BidLevels[i].NumberOfOrders = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = c_Config.get_child("mktData.body.bidOrderQty");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.l2Stock.BidLevels[i].OrderQty = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = c_Config.get_child("mktData.body.offerPrice");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.l2Stock.OfferLevels[i].Price = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = c_Config.get_child("mktData.body.offerNumberOfOrders");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.l2Stock.OfferLevels[i].NumberOfOrders = vt.second.get_value<int32>();
        i++;
      }

      child_Bid = c_Config.get_child("mktData.body.offerOrderQty");
      i = 0;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, child_Bid) {
        msgBody.mktDataSnapshot.l2Stock.OfferLevels[i].OrderQty = vt.second.get_value<int32>();
        i++;
      }


      this->pSpi->OnTickRtnDepthMarketData(&msgBody);
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
    publisher.publish("order2server_md", sendJsonDataStr);

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

