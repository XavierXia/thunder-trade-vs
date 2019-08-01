#include <iostream>
#include <string.h>
#include "ZpquantTraderApi.h"

void Communicate(const char * address, unsigned int port, const std::stringstream & in, std::stringstream & out);

namespace Zpquant {

CZpquantTradeApi::CZpquantTradeApi() {
    pSpi = NULL;
    //memset(sendJsonDataStr, 0, sizeof(sendJsonDataStr)*4096);
}


CZpquantTradeApi::~CZpquantTradeApi() {
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
CZpquantTradeApi::RegisterSpi(CZpquantTradeSpi *pSpi) {
    this->pSpi = pSpi;
}

//初始化交易源
bool
CZpquantTradeApi::InitTraderSource(ZpquantUserLoginField* userLogin) {
    stringstream in, out;
    boost::property_tree::ptree root, result;
    root.put("type", "reqaddtradesource");
    root.put("sourcetype", "kr_td_quant");
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
        std::cout << "...InitTraderSource,result:" << node.first << endl;
    }

    return true;
}

//采集报单回调数据
bool
CZpquantTradeApi::Start() {
    if(!subscriber.connect()) return false;
    if(!publisher.connect()) return false;

    //执行报告消息进行接收和处理
    subscriber.subscribe("oes_resp", [this](const string& topic, const string& msg) {
      cout << "...client,oes_resp...subscribe,topic:" << topic << ",msg: " << msg << endl;

        ptree c_Config;
        std::stringstream jmsg(msg.c_str());  
        try {
            boost::property_tree::read_json(jmsg, c_Config);
        }
        catch(std::exception & e){
            fprintf(stdout, "cannot parse from string 'msg(client,oes_resp)' \n");
            return false;
        }


      int32 msgId = c_Config.get<int32>("msgId");
      cout << "...msgId:" << msgId << endl;
      switch(msgId)
      {
        //查询持仓
        /*
        {"msgId":100, "seqNo":1,"isEnd":"Y", "invAcctId":"A188800368","securityId":"600000","mktId":1,
        "originalHld":1000000, "totalBuyHld":0,"totalSellHld":0,"sellFrzHld":0, "totalTrsfInHld":0,
        "totalTrsfOutHld":0,"trsfOutFrzHld":0,"lockHld":0, "lockFrzHld":0,"unlockFrzHld":0,"coveredFrzHld":0,
        "coveredHld":0, "coveredAvlHld":0,"sumHld":1000000,"sellAvlHld":1000000,"trsfOutAvlHld":1000000,"lockAvlHld":1000000}
          */
        case OESMSG_QRYMSG_STK_HLD:
        {
            ZpquantStkHoldingItem msgBody;
            ZpquantQryCursor pCursor;

            pCursor.seqNo = c_Config.get<int32>("seqNo");
            cout << "...seqNo:" << pCursor.seqNo << endl;

            char isEnd = c_Config.get<char>("isEnd");
            cout << "...isEnd:" << isEnd << endl;
            pCursor.isEnd = isEnd;
            string invAcctId = c_Config.get<string>("invAcctId");
            string securityId = c_Config.get<string>("securityId");
            //if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
            //if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

            msgBody.mktId = c_Config.get<uint8>("mktId");
            msgBody.originalHld = c_Config.get<int64>("originalHld");
            msgBody.totalBuyHld = c_Config.get<int64>("totalBuyHld");
            msgBody.totalSellHld = c_Config.get<int64>("totalSellHld");
            msgBody.sellFrzHld = c_Config.get<int64>("sellFrzHld");
            msgBody.totalTrsfInHld = c_Config.get<int64>("totalTrsfInHld");
            msgBody.totalTrsfOutHld = c_Config.get<int64>("totalTrsfOutHld");
            msgBody.trsfOutFrzHld = c_Config.get<int64>("trsfOutFrzHld");
            msgBody.lockHld = c_Config.get<int64>("lockHld");
            msgBody.lockFrzHld = c_Config.get<int64>("lockFrzHld");
            msgBody.unlockFrzHld = c_Config.get<int64>("unlockFrzHld");
            msgBody.coveredFrzHld = c_Config.get<int64>("coveredFrzHld");
            msgBody.coveredHld = c_Config.get<int64>("coveredHld");
            msgBody.coveredAvlHld = c_Config.get<int64>("coveredAvlHld");
            msgBody.sumHld = c_Config.get<int64>("sumHld");
            msgBody.sellAvlHld = c_Config.get<int64>("sellAvlHld");
            msgBody.trsfOutAvlHld = c_Config.get<int64>("trsfOutAvlHld");
            msgBody.lockAvlHld = c_Config.get<int64>("lockAvlHld");


            this->pSpi->OnQueryStkHolding(&msgBody,&pCursor,0);
            break; 
        }
        /*
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"7","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
         securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":1,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":0, 
         ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":0, cumAmt":0,"cumInterest":0,"cumFee":0,"cumInterest":6600000, cumFee":0,
         "frzAmt":50132,"frzInterest":0,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":0, }
        cannot parse from string 'msg(client,oes_resp)'
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"12","cashAcctId":"1888000368","custId":"1888000368","currType":0,
        "cashType":0, cashAcctStatus":0,"beginningBal":1316134912,"beginningAvailableBal":1316134912,"beginningDrawableBal":1316134912, 
        disableBal":0,"totalDepositAmt":0,"totalWithdrawAmt":0,"withdrawFrzAmt":0, totalSellAmt":0,"totalBuyAmt":3300000,
        "buyFrzAmt":9900000,"totalFeeAmt":50066, feeFrzAmt":50198,"marginAmt":0,"marginFrzAmt":0,"currentTotalBal":1312784846, 
        currentAvailableBal":1302834648,"currentDrawableBal":1302834648 ,}
        cannot parse from string 'msg(client,oes_resp)'
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"8","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
         securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":3,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":145428627, 
         ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":100, cumAmt":3300000,"cumInterest":0,"cumFee":50066, cumFee":3300000,"frzAmt":0,
         "frzInterest":66,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":1302834648, }
        cannot parse from string 'msg(client,oes_resp)'
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"9","exchTrdNum":1981,"clOrdId":1375,"clEnvId":0,"clSeqNo":2,
        "invAcctId":"A188800368", securityId":"601899","mktId":1,"trdSide":1, ordBuySellType":1,"trdDate":20190430,"trdTime":145428627, 
        trdQty":100,"trdPrice":33000,"trdAmt":3300000,"cumQty":100, cumAmt":3300000,"cumInterest":0,cumFee":50066,"pbuId":88822, }
        cannot parse from string 'msg(client,oes_resp)'
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"12","cashAcctId":"1888000368","custId":"1888000368","currType":0,
        "cashType":0, cashAcctStatus":0,"beginningBal":1316134912,"beginningAvailableBal":1316134912,"beginningDrawableBal":1316134912,
         disableBal":0,"totalDepositAmt":0,"totalWithdrawAmt":0,"withdrawFrzAmt":0, totalSellAmt":0,"totalBuyAmt":6600000,"buyFrzAmt":6600000,
         "totalFeeAmt":100132, feeFrzAmt":132,"marginAmt":0,"marginFrzAmt":0,"currentTotalBal":1309434780, currentAvailableBal":1302834648,
         "currentDrawableBal":1302834648 ,}
        cannot parse from string 'msg(client,oes_resp)'
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"13","invAcctId":"A188800368","securityId":"601899","mktId":1,
        "originalHld":0, totalBuyHld":200,"totalSellHld":0,"sellFrzHld":0,"manualFrzHld":0, totalTrsfInHld":0,"totalTrsfOutHld":0,
        "trsfOutFrzHld":0,"lockHld":0, lockFrzHld":0,"unlockFrzHld":0,"coveredFrzHld":0,"coveredHld":0, originalCostAmt":0,
        "totalBuyAmt":6600000,"totalSellAmt":0,"totalBuyFee":100132, totalSellFee":0,"costPrice":33501,"sumHld":200,"sellAvlHld":0, 
        trsfOutAvlHld":200,"lockAvlHld":200,"coveredAvlHld":0, }
        cannot parse from string 'msg(client,oes_resp)'
        */

        /* 委托已收回报 */
        /*...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"7","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
         securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":1,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":0, 
         ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":0, cumAmt":0,"cumInterest":0,"cumFee":0,"cumInterest":6600000, cumFee":0,
         "frzAmt":50132,"frzInterest":0,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":0, }*/
        case OESMSG_RPT_ORDER_INSERT:
        /* 委托确认回报 */
        /*
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"8","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
         securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":3,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":145428627, 
         ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":100, cumAmt":3300000,"cumInterest":0,"cumFee":50066, cumFee":3300000,"frzAmt":0,
         "frzInterest":66,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":1302834648, }
        */
        case OESMSG_RPT_ORDER_REPORT:
        {
            ZpquantOrdCnfm msgBody;

            string invAcctId = c_Config.get<string>("invAcctId");
            string securityId = c_Config.get<string>("securityId");
            //if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
            //if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

            msgBody.clSeqNo = c_Config.get<int32>("clSeqNo");
            msgBody.mktId = c_Config.get<uint8>("mktId");
            msgBody.ordType = c_Config.get<uint8>("ordType");
            msgBody.bsType = c_Config.get<uint8>("bsType");
            string invAcctId = c_Config.get<string>("invAcctId");
            string securityId = c_Config.get<string>("securityId"); 
                       
            msgBody.ordQty = c_Config.get<int32>("ordQty");
            msgBody.ordPrice = c_Config.get<int32>("ordPrice");
            msgBody.origClOrdId = c_Config.get<int64>("origClOrdId");
            msgBody.clOrdId = c_Config.get<int64>("clOrdId");
            msgBody.clientId = c_Config.get<int16>("clientId");

            msgBody.clEnvId = c_Config.get<int8>("clEnvId");
            msgBody.origClEnvId = c_Config.get<int8>("origClEnvId");
            msgBody.origClSeqNo = c_Config.get<int32>("origClSeqNo");
            msgBody.ordDate = c_Config.get<int32>("ordDate");
            msgBody.ordTime = c_Config.get<int32>("ordTime");
            msgBody.ordCnfmTime = c_Config.get<int32>("ordCnfmTime");

            msgBody.ordStatus = c_Config.get<uint8>("ordStatus");
            msgBody.ordCnfmSts = c_Config.get<uint8>("ordCnfmSts");
            msgBody.securityType = c_Config.get<uint8>("securityType");
            string exchOrdId = c_Config.get<string>("exchOrdId");
            msgBody.frzAmt = c_Config.get<int64>("frzAmt");
            msgBody.frzInterest = c_Config.get<int64>("frzInterest");
            msgBody.frzFee = c_Config.get<int64>("frzFee");
            msgBody.cumAmt = c_Config.get<int64>("cumAmt");
            msgBody.cumInterest = c_Config.get<int64>("cumInterest");

            msgBody.cumFee = c_Config.get<int64>("cumFee");
            msgBody.cumQty = c_Config.get<int32>("cumQty");
            msgBody.canceledQty = c_Config.get<int32>("canceledQty");
            msgBody.ordRejReason = c_Config.get<int32>("ordRejReason");
            msgBody.exchErrCode = c_Config.get<int32>("exchErrCode");
            
            if(msgId == OESMSG_RPT_ORDER_INSERT)
            {
                this->pSpi->OnOrderInsert(&msgBody);
            }
            else /* 委托确认回报 */
            {
                this->pSpi->OnOrderReport(&msgBody);
            }           
            break; 
        }
        /*
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"12","cashAcctId":"1888000368","custId":"1888000368","currType":0,
        "cashType":0, cashAcctStatus":0,"beginningBal":1316134912,"beginningAvailableBal":1316134912,"beginningDrawableBal":1316134912, 
        disableBal":0,"totalDepositAmt":0,"totalWithdrawAmt":0,"withdrawFrzAmt":0, totalSellAmt":0,"totalBuyAmt":3300000,
        "buyFrzAmt":9900000,"totalFeeAmt":50066, feeFrzAmt":50198,"marginAmt":0,"marginFrzAmt":0,"currentTotalBal":1312784846, 
        currentAvailableBal":1302834648,"currentDrawableBal":1302834648 ,}
        */
        /* 资金变动回报 */
       case OESMSG_RPT_CASH_ASSET_VARIATION:
        {
            ZpquantCashAssetItem msgBody;

            string cashAcctId = c_Config.get<string>("cashAcctId");
            string custId = c_Config.get<string>("custId");
            //if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
            //if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

            msgBody.currType = c_Config.get<uint8>("currType");
            msgBody.cashType = c_Config.get<uint8>("cashType");
            msgBody.cashAcctStatus = c_Config.get<uint8>("cashAcctStatus");
                       
            msgBody.beginningBal = c_Config.get<int64>("beginningBal");
            msgBody.beginningAvailableBal = c_Config.get<int64>("beginningAvailableBal");
            msgBody.beginningDrawableBal = c_Config.get<int64>("beginningDrawableBal");
            msgBody.disableBal = c_Config.get<int64>("disableBal");
            msgBody.totalDepositAmt = c_Config.get<int64>("totalDepositAmt");

            msgBody.totalWithdrawAmt = c_Config.get<int64>("totalWithdrawAmt");
            msgBody.withdrawFrzAmt = c_Config.get<int64>("withdrawFrzAmt");
            msgBody.totalBuyAmt = c_Config.get<int64>("totalBuyAmt");
            msgBody.totalSellAmt = c_Config.get<int64>("totalSellAmt");
            msgBody.buyFrzAmt = c_Config.get<int64>("buyFrzAmt");
            msgBody.totalFeeAmt = c_Config.get<int64>("totalFeeAmt");

            msgBody.feeFrzAmt = c_Config.get<int64>("feeFrzAmt");
            msgBody.marginAmt = c_Config.get<int64>("marginAmt");
            msgBody.marginFrzAmt = c_Config.get<int64>("marginFrzAmt");
            msgBody.currentTotalBal = c_Config.get<int64>("currentTotalBal");
            msgBody.currentAvailableBal = c_Config.get<int64>("currentAvailableBal");
            msgBody.currentDrawableBal = c_Config.get<int64>("currentDrawableBal");
                      
            this->pSpi->OnCashAssetVariation(&msgBody);
            break; 
        }
        /*
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"9","exchTrdNum":1981,"clOrdId":1375,"clEnvId":0,"clSeqNo":2,
        "invAcctId":"A188800368", securityId":"601899","mktId":1,"trdSide":1, ordBuySellType":1,"trdDate":20190430,"trdTime":145428627, 
        trdQty":100,"trdPrice":33000,"trdAmt":3300000,"cumQty":100, cumAmt":3300000,"cumInterest":0,cumFee":50066,"pbuId":88822, }
        cannot parse from string 'msg(client,oes_resp)'
        */
        /* 成交回报 */
        case OESMSG_RPT_TRADE_REPORT:
        {
            ZpquantTrdCnfm msgBody;

            string invAcctId = c_Config.get<string>("invAcctId");
            string securityId = c_Config.get<string>("securityId");
            //if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
            //if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

            msgBody.exchTrdNum = c_Config.get<int64>("exchTrdNum");
            msgBody.clOrdId = c_Config.get<int64>("clOrdId");
            msgBody.clEnvId = c_Config.get<int8>("clEnvId");
            msgBody.clSeqNo = c_Config.get<int32>("clSeqNo");

            msgBody.mktId = c_Config.get<uint8>("mktId");
            msgBody.trdSide = c_Config.get<uint8>("trdSide");
            msgBody.ordBuySellType = c_Config.get<int64>("ordBuySellType");
            msgBody.trdDate = c_Config.get<int32>("trdDate");

            msgBody.trdTime = c_Config.get<int32>("trdTime");
            msgBody.trdQty = c_Config.get<int32>("trdQty");
            msgBody.trdPrice = c_Config.get<int32>("trdPrice");
            msgBody.trdAmt = c_Config.get<int64>("trdAmt");
            msgBody.cumAmt = c_Config.get<int64>("cumAmt");
            msgBody.cumInterest = c_Config.get<int64>("cumInterest");

            msgBody.cumFee = c_Config.get<int64>("cumFee");
            msgBody.pbuId = c_Config.get<int64>("pbuId");
                      
            this->pSpi->OnTradeReport(&msgBody);
            break; 
        }        
        default:
        break;
      }

    });
    return true;
}

void
CZpquantTradeApi::Stop() {
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

/**
 * 发送委托申报请求
 */
int32
CZpquantTradeApi::SendOrder(const ZpquantOrdReqT *pOrderReq) 
{
    string buyorsell;
    if(pOrderReq->bsType == 1) //买入
    {
        buyorsell = "buy";
    }else if(pOrderReq->bsType == 2){ //卖出
        buyorsell = "sell";
    }else{ //其他
        buyorsell = "other";
    }

    sprintf(sendJsonDataStr, 
          "{\"type\":\"%s\",\"code\":\"%s\",\"sclb\":\"%d\",\"wtfs\":\"%d\",\"amount\":\"%d\",\"price\":\"%d\"}",
          buyorsell.c_str(),pOrderReq->pSecurityId,pOrderReq->mktId,pOrderReq->ordType,pOrderReq->ordQty,pOrderReq->ordPrice);
    cout << "...SendOrder...sendJsonDataStr: " << sendJsonDataStr << endl;
    publisher.publish("order2server_td", sendJsonDataStr);

    return 0;
}


/**
 * 发送撤单请求
 */
int32
CZpquantTradeApi::SendCancelOrder(const ZpquantOrdCancelReqT *pCancelReq) 
{
    return 0;
}


/**
 * 发送出入金委托请求
 */
int32
CZpquantTradeApi::SendFundTrsf(const ZpquantOrdCancelReqT *pFundTrsfReq)
{
    return 0;
}

/**
 * 获取API的发行版本号
 *
 * @return  API的发行版本号 (如: "0.15.3")
 */
const char *
CZpquantTradeApi::GetVersion(void) {
    string vStr = "0.0.1";
    return vStr.c_str();
}


/**
 * 获取当前交易日
 *
 * @retval  >0          当前交易日 (格式：YYYYMMDD)
 * @retval  0           尚未连接查询服务
 * @retval  <0          失败 (负的错误号)
 */
int32
CZpquantTradeApi::GetTradingDay(void) {
    return 0;
}

/*
 * 查询客户资金信息
 */
int32
CZpquantTradeApi::QueryCashAsset(int32 requestId)
{
    string str = "{\"type\":\"query\",\"category\":\"cashAsset\",\"code\":\"\",\"sclb\":\"\"}";
    cout << "...QueryCashAsset...str: " << str << endl;
    publisher.publish("order2server_td", str);
    return 0;
}

//查询 上证或深圳 股票的持仓
int32
CZpquantTradeApi:: QueryStkHolding(const ZpquantQryTrd *pQryFilter, int32 requestId) 
{
    sprintf(sendJsonDataStr, 
          "{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"%s\",\"sclb\":\"%d\"}",pQryFilter->code,pQryFilter->sclb);
    cout << "...stkHolding...sendJsonDataStr: " << sendJsonDataStr << endl;
    publisher.publish("order2server_td", sendJsonDataStr);
    return 0;
}

/**
 * 查询证券账户信息
 */
int32
CZpquantTradeApi::QueryInvAcct(const ZpquantQryTrd *pQryFilter, int32 requestId)
{
    return 0;
}

/**
 * 查询现货产品信息
 */
int32
CZpquantTradeApi::QueryStock(const ZpquantQryTrd *pQryFilter, int32 requestId) 
{
    sprintf(sendJsonDataStr, 
          "{\"type\":\"query\",\"category\":\"stkInfo\",\"code\":\"%s\",\"sclb\":\"%d\"}",pQryFilter->code,pQryFilter->sclb);
    cout << "...stkInfo...sendJsonDataStr: " << sendJsonDataStr << endl;
    publisher.publish("order2server_td", sendJsonDataStr);
    return 0;
}

}

