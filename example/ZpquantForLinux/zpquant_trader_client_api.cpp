#include <iostream>
#include <string.h>
#include "ZpquantTraderApi.h"
#include "redox.hpp"

redox::Subscriber subscriber;
redox::Redox publisher;


namespace Zpquant {


CZpquantTradeApi::CZpquantTradeApi() {
    pSpi = NULL;
    memset(sendJsonDataStr, 0, sizeof(sendJsonDataStr)*4096);
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
void
CZpquantTradeApi::InitTraderSource(ZpquantUserLoginField* userLogin) {
    
}

//采集报单回调数据
bool
CZpquantTradeApi::Start() {
    if(!subscriber.connect()) return false;
    if(!publisher.connect()) return false;

    //执行报告消息进行接收和处理
    subscriber.subscribe("oes_resp", [this](const string& topic, const string& msg) {
      cout << "...client,oes_resp...subscribe,topic:" << topic << ",msg: " << msg << endl;

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
    publisher.publish("order2server", sendJsonDataStr);

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
    publisher.publish("order2server", str);
    return 0;
}

//查询 上证或深圳 股票的持仓
int32
CZpquantTradeApi:: QueryStkHolding(const ZpquantQryTrd *pQryFilter, int32 requestId) 
{
    sprintf(sendJsonDataStr, 
          "{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"%s\",\"sclb\":\"%d\"}",pQryFilter->code,pQryFilter->sclb);
    cout << "...stkHolding...sendJsonDataStr: " << sendJsonDataStr << endl;
    publisher.publish("order2server", sendJsonDataStr);
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
    publisher.publish("order2server", sendJsonDataStr);
    return 0;
}

}

