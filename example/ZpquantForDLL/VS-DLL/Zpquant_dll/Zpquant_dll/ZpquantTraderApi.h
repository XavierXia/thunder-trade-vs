#ifndef _OES_CLIENT_API_H
#define _OES_CLIENT_API_H


#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __declspec(dllexport)
#else
#define TRADER_API_EXPORT __declspec(dllimport)
#endif
#else
#define TRADER_API_EXPORT 
#endif

#include "ZpquantUserApiStruct.h"
#include "redox.hpp"

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp> 
using namespace boost::property_tree;
using namespace std;

namespace   Zpquant {

	class   CZpquantTradeSpi {
	public:
		/* 委托拒绝回报响应 */
		virtual void        OnBusinessReject(int32 errorCode, const ZpquantOrdReject *pOrderReject) = 0;
		/* 委托已收回报回调 */
		virtual void        OnOrderInsert(const ZpquantOrdCnfm *pOrderInsert) = 0;
		/* 委托确认回报回调 */
		virtual void        OnOrderReport(int32 errorCode, const ZpquantOrdCnfm *pOrderReport) = 0;
		/* 成交确认回报回调 */
		virtual void        OnTradeReport(const ZpquantTrdCnfm *pTradeReport) = 0;
		/* 资金变动回报回调 */
		virtual void        OnCashAssetVariation(const ZpquantCashAssetItem *pCashAssetItem) = 0;
		/* 持仓变动回报回调 */
		virtual void        OnStockHoldingVariation(const ZpquantStkHoldingItem *pStkHoldingItem) = 0;
		/* 出入金委托拒绝回报回调 */
		virtual void        OnFundTrsfReject(int32 errorCode, const ZpquantFundTrsfReject *pFundTrsfReject) = 0;
		/* 出入金委托执行回报回调 */
		virtual void        OnFundTrsfReport(int32 errorCode, const ZpquantFundTrsfReport *pFundTrsfReport) = 0;
		/* 市场状态信息回报回调 */
		virtual void        OnMarketState(const ZpquantMarketStateInfo *pMarketStateItem) = 0;

		/* 查询委托信息回调 */
		virtual void        OnQueryOrder(const ZpquantOrdItem *pOrder, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询成交信息回调 */
		virtual void        OnQueryTrade(const ZpquantTrdItem *pTrade, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询资金信息回调 */
		virtual void        OnQueryCashAsset(const ZpquantCashAssetItem *pCashAsset, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询持仓信息回调 */
		virtual void        OnQueryStkHolding(const ZpquantStkHoldingItem *pStkHolding, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询客户信息回调 */
		virtual void        OnQueryCustInfo(const ZpquantCustItemT *pCust, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询股东账户信息回调 */
		virtual void        OnQueryInvAcct(const ZpquantInvAcctItem *pInvAcct, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询出入金流水信息回调 */
		virtual void        OnQueryFundTransferSerial(const ZpquantFundTransferSerialItemT *pFundTrsf, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询证券信息回调 */
		virtual void        OnQueryStock(const ZpquantStockBaseInfo *pStock, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* 查询市场状态信息回调 */
		virtual void        OnQueryMarketState(const ZpquantMarketStateInfo *pMarketState, const ZpquantQryCursor *pCursor, int32 requestId) = 0;

	public:
		virtual ~CZpquantTradeSpi() {};

	public:
		int32               currentRequestId;
	};


	class  TRADER_API_EXPORT CZpquantTradeApi {
	public:
		static const char * GetVersion(void);
		redox::Subscriber subscriber;
		redox::Redox publisher;

	public:
		CZpquantTradeApi();

		virtual ~CZpquantTradeApi();

		/* 注册spi回调接口 */
		void                RegisterSpi(CZpquantTradeSpi *pSpi);
		//初始化交易源
		bool                InitTraderSource(ZpquantUserLoginField* userLogin);
		/* 启动连通数据通道 */
		bool                Start();
		/* 停止连通数据通道 */
		void                Stop();
		/* 发送交易委托请求，非线程安全实现 */
		int32               SendOrder(const ZpquantOrdReqT *pOrderReq);
		/* 发送撤单请求，非线程安全实现 */
		int32               SendCancelOrder(const ZpquantOrdCancelReqT *pCancelReq);
		/* 发送出入金请求，非线程安全实现 */
		int32               SendFundTrsf(const ZpquantOrdCancelReqT *pFundTrsfReq);
		/* 获取交易日期 */
		int32               GetTradingDay(void);
		/* 获取客户端总览信息 */
		//int32               GetClientOverview(OesClientOverviewT *pClientOverview);
		/* 查询所有委托信息，非线程安全实现 */
		//int32               QueryOrder(const OesQryOrdFilterT *pQryFilter, int32 requestId = 0);
		/* 查询成交信息，非线程安全实现 */
		int32               QueryTrade(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		/* 查询客户资金信息，非线程安全实现 */
		int32               QueryCashAsset(int32 requestId = 0);
		/* 查询股票持仓信息，非线程安全实现 */
		int32               QueryStkHolding(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		// /* 查询客户信息，非线程安全实现 */
		// int32               QueryCustInfo(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		/* 查询证券账户信息，非线程安全实现 */
		int32               QueryInvAcct(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		/* 查询现货产品信息，非线程安全实现 */
		int32               QueryStock(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);

	private:
		/* 禁止拷贝构造函数 */
		CZpquantTradeApi(const CZpquantTradeApi&);
		/* 禁止赋值函数 */
		CZpquantTradeApi&       operator=(const CZpquantTradeApi&);
		char sendJsonDataStr[4096];


	private:
		bool                isCfg;
		bool                isRunning;
		volatile int32      terminatedFlag;

		CZpquantTradeSpi        *pSpi;
	};


}


#endif /* _OES_CLIENT_SAMPLE_H */
#pragma once
