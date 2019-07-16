#include "KR_QUANT_TDPlugin.h"
#include <stdarg.h>
#include <thread>
#include "OrderRefResolve.h"
#include "AutoPend.h"
#include <iostream>

const string CKR_QUANT_TDPlugin::s_strAccountKeyword = "serveraddress;username;";
extern char ProcessName[256];
#define NOTIFY_LOGIN_SUCCEED {m_boolIsOnline = true; std::unique_lock<std::mutex> lk(m_mtxLoginSignal);m_cvLoginSignalCV.notify_all();}
#define NOTIFY_LOGIN_FAILED  {m_boolIsOnline = false;std::unique_lock<std::mutex> lk(m_mtxLoginSignal);m_cvLoginSignalCV.notify_all();}
const char THE_CONFIG_FILE_NAME[100]="/root/thunder-trade-vs/third/Kr360Quant/conf/oes_client.conf";


#define NAME ("kr_quant_td")

date CKR_QUANT_TDPlugin::GetTradeday(ptime _Current)
{
	if (_Current.time_of_day() < time_duration(12, 0, 0, 0))//Õâ¸öµØ·½²»Òª¿¨µÄÌ«ËÀ
		return _Current.date();
	else
	{
		if (_Current.date().day_of_week().as_enum() == Friday)
			return _Current.date() + days(3);
		else
			return _Current.date() + days(1);
	}
}
CKR_QUANT_TDPlugin::CKR_QUANT_TDPlugin():m_abIsPending(false)
{

}

CKR_QUANT_TDPlugin::~CKR_QUANT_TDPlugin()
{

}

bool CKR_QUANT_TDPlugin::IsPedding()
{
	return m_abIsPending.load();
}

bool CKR_QUANT_TDPlugin::IsOnline()
{
	return true;
}

void CKR_QUANT_TDPlugin::IncreaseRefCount()
{
	m_intRefCount++;
}

void CKR_QUANT_TDPlugin::DescreaseRefCount()
{
	m_intRefCount--;
}

int CKR_QUANT_TDPlugin::GetRefCount()
{
	return m_intRefCount;
}

void CKR_QUANT_TDPlugin::CheckSymbolValidity(const unordered_map<string, string> & insConfig)
{
	
}

string CKR_QUANT_TDPlugin::GetCurrentKeyword()
{
	return NAME;
}

string CKR_QUANT_TDPlugin::GetProspectiveKeyword(const ptree & in)
{
	string retKey = NAME;
	return retKey;
}

void CKR_QUANT_TDPlugin::GetState(ptree & out)
{
	out.put("online", "true");
}

void CKR_QUANT_TDPlugin::TDInit(const ptree & in, MTradePluginContextInterface * pTradePluginContext, unsigned int AccountNumber)
{
    pOesApi = new Quant360::OesClientApi();
    pOesSpi = new OesClientMySpi();

    if (!pOesApi || !pOesSpi) 
    {
        ShowMessage(severity_levels::error,
				"pOesApi or pOesSpi error");
        return;
    }

    /* 打印API版本信息 */
    ShowMessage(severity_levels::normal, "OesClientApi 版本: %s\n",
            Quant360::OesClientApi::GetVersion());

    /* 注册spi回调接口 */
    pOesApi->RegisterSpi(pOesSpi);

    /* 加载配置文件 */
    if (! pOesApi->LoadCfg(THE_CONFIG_FILE_NAME)) {
        ShowMessage(severity_levels::error,"加载配置文件失败!");
        return;
    }

    /*
     * 设置客户端本地的设备序列号
     * @note 为满足监管需求，需要设置客户端本机的硬盘序列号
     */
    pOesApi->SetCustomizedDriverId("C02TL13QGVC8");

    Start();

}

void CKR_QUANT_TDPlugin::TDHotUpdate(const ptree &)
{

}

void CKR_QUANT_TDPlugin::TimerHandler(boost::asio::deadline_timer* timer, const boost::system::error_code& err)
{
	
}

// void *CKR_QUANT_TDPlugin::subThreadMain(void *pParams) 
// {
// 	CKR_QUANT_TDPlugin  *tdPlugin = (CKR_QUANT_TDPlugin *) pParams;
// 	if(!tdPlugin->subscriber.connect()) return;
	
// 	subscriber.subscribe("allHQData", [tdPlugin](const string& topic, const string& msg) {
//       cout << "...subscribe,topic:" << topic << ",msg: " << msg << endl;
//     });
// }

bool CKR_QUANT_TDPlugin::Start()
{
	CAutoPend pend(m_abIsPending);

	/* 启动 */
    if (! pOesApi->Start()) 
    {
        ShowMessage(severity_levels::error,"启动API失败!");
        return false;
    }

    /* 打印当前交易日 */
    ShowMessage(severity_levels::normal,"服务端交易日: %08d",pOesApi->GetTradingDay());

    // pthread_t       subThreadId;
    // int32           ret = 0;

    // ret = pthread_create(&subThreadId, NULL, subThreadMain, (void *) this);
    // if (ret != 0) 
    // {
    //     ShowMessage(severity_levels::error,"创建订阅下单等命令线程失败! error[%d - %s]\n",
    //             ret, strerror(ret));
    // }

   	if(!subscriber.connect()) return false;
	
	subscriber.subscribe("order2server", [this](const string& topic, const string& msg) {
      	this->ShowMessage(severity_levels::normal,"...subscribe,topic:%s,msg:%s",topic.c_str(),msg.c_str());

        //example
        if(msg == "querycc") //查询持仓
        {
            OesClientMain_QueryStkHolding(pOesApi, OES_MKT_ID_UNDEFINE, NULL);
        }

        if(msg == "buy")
        {
            OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SH_A, "601881", NULL,
                OES_ORD_TYPE_LMT, OES_BS_TYPE_BUY, 200, 126700);
        }

        if(msg == "sell")
        {
            OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SH_A, "601881", NULL,
                OES_ORD_TYPE_LMT, OES_BS_TYPE_SELL, 100, 86700);

            OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SZ_A, "000001", NULL,
                OES_ORD_TYPE_LMT, OES_BS_TYPE_SELL, 300, 16700);
        }

    });

	return true;
}

bool CKR_QUANT_TDPlugin::Stop()
{
	CAutoPend pend(m_abIsPending);

	/* 停止 */
    pOesApi->Stop();
    subscriber.disconnect();

    delete pOesApi;
    delete pOesSpi;

	return true;
}

void CKR_QUANT_TDPlugin::TDUnload()
{
	
	Stop();
}

void CKR_QUANT_TDPlugin::ShowMessage(severity_levels lv, const char * fmt, ...)
{
	char buf[512];
	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buf, 512, fmt, arg);
	va_end(arg);
	boost::log::sources::severity_logger< severity_levels > m_Logger;
	BOOST_LOG_SEV(m_Logger, lv) << ProcessName << ": " << buf << " [" << to_iso_string(microsec_clock::universal_time()) << "]";
}


/**
 * 发送委托请求
 *
 * @param   pOesApi         oes客户端
 * @param   mktId           市场代码 @see eOesMarketIdT
 * @param   pSecurityId     股票代码 (char[6]/char[8])
 * @param   pInvAcctId      股东账户代码 (char[10])，可 NULL
 * @param   ordType         委托类型 @see eOesOrdTypeT, eOesOrdTypeShT, eOesOrdTypeSzT
 * @param   bsType          买卖类型 @sse eOesBuySellTypeT
 * @param   ordQty          委托数量 (单位为股/张)
 * @param   ordPrice        委托价格 (单位精确到元后四位，即1元 = 10000)
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
int32
CKR_QUANT_TDPlugin::OesClientMain_SendOrder(Quant360::OesClientApi *pOesApi,
        uint8 mktId, const char *pSecurityId, const char *pInvAcctId,
        uint8 ordType, uint8 bsType, int32 ordQty, int32 ordPrice) {
    OesOrdReqT          ordReq = {NULLOBJ_OES_ORD_REQ};

    ordReq.clSeqNo = (int32) ++ pOesApi->apiEnv.ordChannel.lastOutMsgSeq;
    ordReq.mktId = mktId;
    ordReq.ordType = ordType;
    ordReq.bsType = bsType;

    strncpy(ordReq.securityId, pSecurityId, sizeof(ordReq.securityId) - 1);
    if (pInvAcctId) {
        /* 股东账户可不填 */
        strncpy(ordReq.invAcctId, pInvAcctId, sizeof(ordReq.invAcctId) - 1);
    }

    ordReq.ordQty = ordQty;
    ordReq.ordPrice = ordPrice;

    return pOesApi->SendOrder(&ordReq);
}


/**
 * 发送撤单请求
 *
 * @param   pOesApi         oes客户端
 * @param   mktId           被撤委托的市场代码 @see eOesMarketIdT
 * @param   pSecurityId     被撤委托股票代码 (char[6]/char[8]), 可空
 * @param   pInvAcctId      被撤委托股东账户代码 (char[10])，可空
 * @param   origClSeqNo     被撤委托的流水号 (若使用 origClOrdId, 则不必填充该字段)
 * @param   origClEnvId     被撤委托的客户端环境号 (小于等于0, 则使用当前会话的 clEnvId)
 * @param   origClOrdId     被撤委托的客户订单编号 (若使用 origClSeqNo, 则不必填充该字段)
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
int32
CKR_QUANT_TDPlugin::OesClientMain_CancelOrder(Quant360::OesClientApi *pOesApi,
        uint8 mktId, const char *pSecurityId, const char *pInvAcctId,
        int32 origClSeqNo, int8 origClEnvId, int64 origClOrdId) {
    OesOrdCancelReqT    cancelReq = {NULLOBJ_OES_ORD_CANCEL_REQ};

    cancelReq.clSeqNo = (int32) ++ pOesApi->apiEnv.ordChannel.lastOutMsgSeq;
    cancelReq.mktId = mktId;

    if (pSecurityId) {
        /* 撤单时被撤委托的股票代码可不填 */
        strncpy(cancelReq.securityId, pSecurityId, sizeof(cancelReq.securityId) - 1);
    }

    if (pInvAcctId) {
        /* 撤单时被撤委托的股东账户可不填 */
        strncpy(cancelReq.invAcctId, pInvAcctId, sizeof(cancelReq.invAcctId) - 1);
    }

    cancelReq.origClSeqNo = origClSeqNo;
    cancelReq.origClEnvId = origClEnvId;

    cancelReq.origClOrdId = origClOrdId;

    return pOesApi->SendCancelOrder(&cancelReq);
}

/**
 * 查询客户端总览信息
 *
 * @param   pSessionInfo    会话信息
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
int32
CKR_QUANT_TDPlugin::OesClientMain_QueryClientOverview(Quant360::OesClientApi *pOesApi) {
    OesClientOverviewT  clientOverview = {NULLOBJ_OES_CLIENT_OVERVIEW};
    int32               ret = 0;
    int32               i = 0;
    char sendJsonDataStr[4086];

    ret = pOesApi->GetClientOverview(&clientOverview);
    if (ret < 0) {
        return ret;
    }

    sprintf(sendJsonDataStr,">>> 客户端总览信息: {客户端编号[%d], 客户端类型[%hhu], " \
            "客户端状态[%hhu], 客户端名称[%s], 上海现货对应PBU[%d], " \
            "深圳现货对应PBU[%d], 委托通道流控阈值[%d], 查询通道流控阈值[%d], " \
            "关联的客户数量[%d]}\n",
            clientOverview.clientId, clientOverview.clientType,
            clientOverview.clientStatus, clientOverview.clientName,
            clientOverview.sseStkPbuId, clientOverview.szseStkPbuId,
            clientOverview.ordTrafficLimit, clientOverview.qryTrafficLimit,
            clientOverview.associatedCustCnt);
    publisher.publish("oes_resp",sendJsonDataStr);

    // for (i = 0; i < clientOverview.associatedCustCnt; i++) {
    //     sprintf(sendJsonDataStr,"    >>> 客户总览信息: {客户代码[%s], 客户状态[%hhu], " \
    //             "风险评级[%hhu], 营业部代码[%d], 客户姓名[%s]}\n",
    //             clientOverview.custItems[i].custId,
    //             clientOverview.custItems[i].status,
    //             clientOverview.custItems[i].riskLevel,
    //             clientOverview.custItems[i].branchId,
    //             clientOverview.custItems[i].custName);
    //     publisher.publish("oes_resp",sendJsonDataStr);

    //     if (clientOverview.custItems[i].spotCashAcct.isValid) {
    //         sprintf(sendJsonDataStr,"        >>> 资金账户总览: {资金账户[%s], " \
    //                 "资金类型[%hhu], 账户状态[%hhu], 出入金是否禁止[%hhu]}\n",
    //                 clientOverview.custItems[i].spotCashAcct.cashAcctId,
    //                 clientOverview.custItems[i].spotCashAcct.cashType,
    //                 clientOverview.custItems[i].spotCashAcct.cashAcctStatus,
    //                 clientOverview.custItems[i].spotCashAcct.isFundTrsfDisabled);
    //     }

    //     if (clientOverview.custItems[i].shSpotInvAcct.isValid) {
    //         sprintf(sendJsonDataStr,"        >>> 股东账户总览: {股东账户代码[%s], " \
    //                 "市场代码[%hhu], 账户状态[%hhu], 是否禁止交易[%hhu], 席位号[%d], " \
    //                 "当日累计有效交易类委托笔数[%d], 当日累计有效非交易类委托笔数[%d], " \
    //                 "当日累计有效撤单笔数[%d], 当日累计被OES拒绝的委托笔数[%d], " \
    //                 "当日累计被交易所拒绝的委托笔数[%d], 当日累计成交笔数[%d]}\n",
    //                 clientOverview.custItems[i].shSpotInvAcct.invAcctId,
    //                 clientOverview.custItems[i].shSpotInvAcct.mktId,
    //                 clientOverview.custItems[i].shSpotInvAcct.status,
    //                 clientOverview.custItems[i].shSpotInvAcct.isTradeDisabled,
    //                 clientOverview.custItems[i].shSpotInvAcct.pbuId,
    //                 clientOverview.custItems[i].shSpotInvAcct.trdOrdCnt,
    //                 clientOverview.custItems[i].shSpotInvAcct.nonTrdOrdCnt,
    //                 clientOverview.custItems[i].shSpotInvAcct.cancelOrdCnt,
    //                 clientOverview.custItems[i].shSpotInvAcct.oesRejectOrdCnt,
    //                 clientOverview.custItems[i].shSpotInvAcct.exchRejectOrdCnt,
    //                 clientOverview.custItems[i].shSpotInvAcct.trdCnt);
    //     }

    //     if (clientOverview.custItems[i].szSpotInvAcct.isValid) {
    //         sprintf(sendJsonDataStr,"        >>> 股东账户总览: {股东账户代码[%s], " \
    //                 "市场代码[%hhu], 账户状态[%hhu], 是否禁止交易[%hhu], 席位号[%d], " \
    //                 "当日累计有效交易类委托笔数[%d], 当日累计有效非交易类委托笔数[%d], " \
    //                 "当日累计有效撤单笔数[%d], 当日累计被OES拒绝的委托笔数[%d], " \
    //                 "当日累计被交易所拒绝的委托笔数[%d], 当日累计成交笔数[%d]}\n",
    //                 clientOverview.custItems[i].szSpotInvAcct.invAcctId,
    //                 clientOverview.custItems[i].szSpotInvAcct.mktId,
    //                 clientOverview.custItems[i].szSpotInvAcct.status,
    //                 clientOverview.custItems[i].szSpotInvAcct.isTradeDisabled,
    //                 clientOverview.custItems[i].szSpotInvAcct.pbuId,
    //                 clientOverview.custItems[i].szSpotInvAcct.trdOrdCnt,
    //                 clientOverview.custItems[i].szSpotInvAcct.nonTrdOrdCnt,
    //                 clientOverview.custItems[i].szSpotInvAcct.cancelOrdCnt,
    //                 clientOverview.custItems[i].szSpotInvAcct.oesRejectOrdCnt,
    //                 clientOverview.custItems[i].szSpotInvAcct.exchRejectOrdCnt,
    //                 clientOverview.custItems[i].szSpotInvAcct.trdCnt);
    //     }
    // }

    return 0;
}


/**
 * 查询市场状态
 *
 * @param   pOesApi         oes客户端
 * @param   exchId          交易所代码 @see eOesExchangeIdT
 * @param   platformId      交易平台类型 @see eOesPlatformIdT
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
int32
CKR_QUANT_TDPlugin::OesClientMain_QueryMarketStatus(Quant360::OesClientApi *pOesApi,
        uint8 exchId, uint8 platformId) {
    OesQryMarketStateFilterT    qryFilter = {NULLOBJ_OES_QRY_MARKET_STATE_FILTER};

    qryFilter.exchId = exchId;
    qryFilter.platformId = platformId;

    /* 也可直接使用 pOesApi->QueryMarketState(NULL, 0) 查询所有的市场状态 */
    return pOesApi->QueryMarketState(&qryFilter, 0);
}


/**
 * 查询资金
 *
 * @param   pOesApi         oes客户端
 * @param   pCashAcctId     资金账户代码
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
int32
CKR_QUANT_TDPlugin::OesClientMain_QueryCashAsset(Quant360::OesClientApi *pOesApi,
        const char *pCashAcctId) {
    OesQryCashAssetFilterT  qryFilter = {NULLOBJ_OES_QRY_CASH_ASSET_FILTER};

    if (pCashAcctId) {
        strncpy(qryFilter.cashAcctId, pCashAcctId,
                sizeof(qryFilter.cashAcctId) - 1);
    }

    /* 也可直接使用 pOesApi->QueryCashAsset(NULL, 0) 查询客户所有资金账户 */
    return pOesApi->QueryCashAsset(&qryFilter, 0);
}


/**
 * 查询产品
 *
 * @param   pOesApi         oes客户端
 * @param   pSecurityId     产品代码
 * @param   mktId           市场代码
 * @param   securityType    证券类别
 * @param   subSecurityType 证券子类别
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
int32
CKR_QUANT_TDPlugin::OesClientMain_QueryStock(Quant360::OesClientApi *pOesApi,
        const char *pSecurityId, uint8 mktId, uint8 securityType,
        uint8 subSecurityType) {
    OesQryStockFilterT  qryFilter = {NULLOBJ_OES_QRY_STOCK_FILTER};

    if (pSecurityId) {
        strncpy(qryFilter.securityId, pSecurityId,
                sizeof(qryFilter.securityId) - 1);
    }

    qryFilter.mktId = mktId;
    qryFilter.securityType = securityType;
    qryFilter.subSecurityType = subSecurityType;

    return pOesApi->QueryStock(&qryFilter, 0);
}


/**
 * 查询股票持仓
 *
 * @param   pOesApi         oes客户端
 * @param   mktId           市场代码 @see eOesMarketIdT
 * @param   pSecurityId     股票代码 (char[6]/char[8])
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
 */
int32
CKR_QUANT_TDPlugin::OesClientMain_QueryStkHolding(Quant360::OesClientApi *pOesApi,
        uint8 mktId, const char *pSecurityId) {
    OesQryStkHoldingFilterT qryFilter = {NULLOBJ_OES_QRY_STK_HOLDING_FILTER};

    qryFilter.mktId = mktId;
    if (pSecurityId) {
        strncpy(qryFilter.securityId, pSecurityId,
                sizeof(qryFilter.securityId) - 1);
    }

    /* 也可直接使用 pOesApi->QueryStkHolding(NULL, 0) 查询客户所有持仓 */
    return pOesApi->QueryStkHolding(&qryFilter, 0);
}


TOrderRefIdType CKR_QUANT_TDPlugin::TDBasicMakeOrder(
	TOrderType ordertype,
	unordered_map<string, string> & instrument,
	TOrderDirectionType direction,
	TOrderOffsetType offset,
	TVolumeType volume,
	TPriceType LimitPrice,
	TOrderRefIdType orderRefBase)
{
	
	return orderRefBase;
}

TLastErrorIdType CKR_QUANT_TDPlugin::TDBasicCancelOrder(TOrderRefIdType Ref, unordered_map<string, string> & instrument, TOrderSysIdType orderSysId)
{
	return LB1_NO_ERROR;
}

int CKR_QUANT_TDPlugin::TDGetRemainAmountOfCancelChances(const char * ins)
{
	return std::numeric_limits<int>::max();
}
