#include <iostream>
#include <string.h>
#include "ZpquantTraderApi.h"
#include <nanomsg/pair.h>

#define SOCKET_ADDRESS "tcp://127.0.0.1:8001"
#define ADDRESS1 "inproc://test"
#define ADDRESS2 "tcp://*:8001"
#define ADDRESS3 "ipc:///tmp/reqrep.ipc"
#define ADDRESS4 "tcp://*:8002"


void Communicate(const char * address, unsigned int port, const std::stringstream & in, std::stringstream & out);

namespace Zpquant {

CZpquantTradeApi::CZpquantTradeApi() {
    pSpi = NULL;
    tdnnsocket.socket_set(AF_SP, NN_PAIR);
    tdnnsocket.bind(ADDRESS2);

    tdnnsocket_resp.socket_set(AF_SP, NN_PAIR);
    tdnnsocket_resp.bind(ADDRESS4);
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

void* CZpquantTradeApi::tradeThreadMain(void *pParams)
{
  CZpquantTradeApi *tdapi = (CZpquantTradeApi *) pParams;  
  CZpquantTradeSpi *tdspi = tdapi->pSpi;
  char buf[4096];
  while(1)
  {
        int rc = tdapi->tdnnsocket_resp.recv(buf, sizeof(buf), 0);
        cout<<"...CZpquantMdApi,tradeThreadMain recv: " << buf << endl;

        ptree c_Config;
        std::stringstream jmsg(buf);  
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

            tdspi->OnQueryStkHolding(&msgBody,&pCursor,0);
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

        /* 委托已收回报 114*/
        /*...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"7","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
         securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":1,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":0, 
         ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":0, cumAmt":0,"cumInterest":0,"cumFee":0,"cumInterest":6600000, cumFee":0,
         "frzAmt":50132,"frzInterest":0,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":0, }*/
        case OESMSG_RPT_ORDER_INSERT:
        /* 委托确认回报 115*/
        /*
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"8","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
         securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":3,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":145428627, 
         ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":100, cumAmt":3300000,"cumInterest":0,"cumFee":50066, cumFee":3300000,"frzAmt":0,
         "frzInterest":66,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":1302834648, }
        */
        case OESMSG_RPT_ORDER_REPORT:
        {
            ZpquantOrdCnfm msgBody;

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

            msgBody.clEnvId = c_Config.get<int8>("clEnvId");
            //msgBody.origClEnvId = c_Config.get<int8>("origClEnvId");
            //msgBody.origClSeqNo = c_Config.get<int32>("origClSeqNo");
            msgBody.ordDate = c_Config.get<int32>("ordDate");
            msgBody.ordTime = c_Config.get<int32>("ordTime");
            msgBody.ordCnfmTime = c_Config.get<int32>("ordCnfmTime");

            msgBody.ordStatus = c_Config.get<uint8>("ordStatus");
            //msgBody.ordCnfmSts = c_Config.get<uint8>("ordCnfmSts");
            //msgBody.securityType = c_Config.get<uint8>("securityType");
            //string exchOrdId = c_Config.get<string>("exchOrdId");
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
                tdspi->OnOrderInsert(&msgBody);
            }
            else /* 委托确认回报 */
            {
                tdspi->OnOrderReport(0,&msgBody);
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
        /* 资金变动回报 119*/
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
                      
            tdspi->OnCashAssetVariation(&msgBody);
            break; 
        }
        /*
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"9","exchTrdNum":1981,"clOrdId":1375,"clEnvId":0,"clSeqNo":2,
        "invAcctId":"A188800368", securityId":"601899","mktId":1,"trdSide":1, ordBuySellType":1,"trdDate":20190430,"trdTime":145428627, 
        trdQty":100,"trdPrice":33000,"trdAmt":3300000,"cumQty":100, cumAmt":3300000,"cumInterest":0,cumFee":50066,"pbuId":88822, }
        cannot parse from string 'msg(client,oes_resp)'
        */
        /* 成交回报 116*/
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
                      
            tdspi->OnTradeReport(&msgBody);
            break; 
        }
        /* 持仓变动回报 */
        /*
        ...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":120,"invAcctId":"A188800368","securityId":"601899",
        "mktId":1,"originalHld":0, "totalBuyHld":1400,"totalSellHld":0,"sellFrzHld":0,"manualFrzHld":0, 
        "totalTrsfInHld":0,"totalTrsfOutHld":0,"trsfOutFrzHld":0,"lockHld":0, "lockFrzHld":0,"unlockFrzHld":0,
        "coveredFrzHld":0,"coveredHld":0, "originalCostAmt":0,"totalBuyAmt":45830000,"totalSellAmt":0,
        "totalBuyFee":350915, "totalSellFee":0,"costPrice":32986,"sumHld":1400,"sellAvlHld":0, "trsfOutAvlHld":1400,
        "lockAvlHld":1400,"coveredAvlHld":0}
        */
        case OESMSG_RPT_STOCK_HOLDING_VARIATION:
        {
            ZpquantStkHoldingItem msgBody;

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

            tdspi->OnStockHoldingVariation(&msgBody);
            break;
        }
     
        default:
        break;
      }
  }
}

//采集报单回调数据
bool
CZpquantTradeApi::Start() 
{
    pthread_t       rptThreadId;
    int32           ret = 0;

    /* 创建回报接收线程 */
    ret = pthread_create(&rptThreadId, NULL, tradeThreadMain, (void *) this);
    if (ret != 0) {
        fprintf(stderr, "创建交易回调接收线程失败! error[%d - %s]\n",
                ret, strerror(ret));
        return false;
    }

    return true;
}

void
CZpquantTradeApi::Stop() {

}

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

    char sendJsonDataStr[512];
    sprintf(sendJsonDataStr, 
          "{\"type\":\"%s\",\"code\":\"%s\",\"sclb\":\"%d\",\"wtfs\":\"%d\",\"amount\":\"%d\",\"price\":\"%d\"}",
          buyorsell.c_str(),pOrderReq->pSecurityId,pOrderReq->mktId,pOrderReq->ordType,pOrderReq->ordQty,pOrderReq->ordPrice);
    cout << "...SendOrder...sendJsonDataStr: " << sendJsonDataStr << endl;
    tdnnsocket.send(sendJsonDataStr,strlen(sendJsonDataStr)+1,0);

    return 0;
}


/**
 * 发送撤单请求
 */
int32
CZpquantTradeApi::SendCancelOrder(const ZpquantOrdCancelReqT *pCancelReq) 
{
    char sendJsonDataStr[256];
    string cancelOrd("cancelOrder");
    sprintf(sendJsonDataStr, 
          "{\"type\":\"%s\",\"mktId\":%d,\"origClSeqNo\":%d,\"origClEnvId\":%d,\"origClOrdId\":%d}",
          cancelOrd.c_str(),pCancelReq->mktId,pCancelReq->origClSeqNo,pCancelReq->origClEnvId,pCancelReq->origClOrdId);
    cout << "...SendCancelOrder...sendJsonDataStr: " << sendJsonDataStr << endl;
    tdnnsocket.send(sendJsonDataStr,strlen(sendJsonDataStr)+1,0);
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
    char sendJsonDataStr[256];
    string str = "{\"type\":\"query\",\"category\":\"cashAsset\",\"code\":\"\",\"sclb\":\"\"}";
    cout << "...QueryCashAsset...str: " << str << endl;
    tdnnsocket.send(sendJsonDataStr,strlen(sendJsonDataStr)+1,0);
    return 0;
}

//查询 上证或深圳 股票的持仓
int32
CZpquantTradeApi:: QueryStkHolding(const ZpquantQryTrd *pQryFilter, int32 requestId) 
{
    char sendJsonDataStr[256];
    sprintf(sendJsonDataStr, 
          "{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"%s\",\"sclb\":\"%d\"}",pQryFilter->code,pQryFilter->sclb);
    cout << "...stkHolding...sendJsonDataStr: " << sendJsonDataStr << endl;
    tdnnsocket.send(sendJsonDataStr,strlen(sendJsonDataStr)+1,0);
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
    char sendJsonDataStr[256];
    sprintf(sendJsonDataStr, 
          "{\"type\":\"query\",\"category\":\"stkInfo\",\"code\":\"%s\",\"sclb\":\"%d\"}",pQryFilter->code,pQryFilter->sclb);
    cout << "...stkInfo...sendJsonDataStr: " << sendJsonDataStr << endl;
    tdnnsocket.send(sendJsonDataStr,strlen(sendJsonDataStr)+1,0);
    return 0;
}

}

