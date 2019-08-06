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
		/* ί�оܾ��ر���Ӧ */
		virtual void        OnBusinessReject(int32 errorCode, const ZpquantOrdReject *pOrderReject) = 0;
		/* ί�����ջر��ص� */
		virtual void        OnOrderInsert(const ZpquantOrdCnfm *pOrderInsert) = 0;
		/* ί��ȷ�ϻر��ص� */
		virtual void        OnOrderReport(int32 errorCode, const ZpquantOrdCnfm *pOrderReport) = 0;
		/* �ɽ�ȷ�ϻر��ص� */
		virtual void        OnTradeReport(const ZpquantTrdCnfm *pTradeReport) = 0;
		/* �ʽ�䶯�ر��ص� */
		virtual void        OnCashAssetVariation(const ZpquantCashAssetItem *pCashAssetItem) = 0;
		/* �ֱֲ䶯�ر��ص� */
		virtual void        OnStockHoldingVariation(const ZpquantStkHoldingItem *pStkHoldingItem) = 0;
		/* �����ί�оܾ��ر��ص� */
		virtual void        OnFundTrsfReject(int32 errorCode, const ZpquantFundTrsfReject *pFundTrsfReject) = 0;
		/* �����ί��ִ�лر��ص� */
		virtual void        OnFundTrsfReport(int32 errorCode, const ZpquantFundTrsfReport *pFundTrsfReport) = 0;
		/* �г�״̬��Ϣ�ر��ص� */
		virtual void        OnMarketState(const ZpquantMarketStateInfo *pMarketStateItem) = 0;

		/* ��ѯί����Ϣ�ص� */
		virtual void        OnQueryOrder(const ZpquantOrdItem *pOrder, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ�ɽ���Ϣ�ص� */
		virtual void        OnQueryTrade(const ZpquantTrdItem *pTrade, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ�ʽ���Ϣ�ص� */
		virtual void        OnQueryCashAsset(const ZpquantCashAssetItem *pCashAsset, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ�ֲ���Ϣ�ص� */
		virtual void        OnQueryStkHolding(const ZpquantStkHoldingItem *pStkHolding, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ�ͻ���Ϣ�ص� */
		virtual void        OnQueryCustInfo(const ZpquantCustItemT *pCust, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ�ɶ��˻���Ϣ�ص� */
		virtual void        OnQueryInvAcct(const ZpquantInvAcctItem *pInvAcct, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ�������ˮ��Ϣ�ص� */
		virtual void        OnQueryFundTransferSerial(const ZpquantFundTransferSerialItemT *pFundTrsf, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ֤ȯ��Ϣ�ص� */
		virtual void        OnQueryStock(const ZpquantStockBaseInfo *pStock, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
		/* ��ѯ�г�״̬��Ϣ�ص� */
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

		/* ע��spi�ص��ӿ� */
		void                RegisterSpi(CZpquantTradeSpi *pSpi);
		//��ʼ������Դ
		bool                InitTraderSource(ZpquantUserLoginField* userLogin);
		/* ������ͨ����ͨ�� */
		bool                Start();
		/* ֹͣ��ͨ����ͨ�� */
		void                Stop();
		/* ���ͽ���ί�����󣬷��̰߳�ȫʵ�� */
		int32               SendOrder(const ZpquantOrdReqT *pOrderReq);
		/* ���ͳ������󣬷��̰߳�ȫʵ�� */
		int32               SendCancelOrder(const ZpquantOrdCancelReqT *pCancelReq);
		/* ���ͳ�������󣬷��̰߳�ȫʵ�� */
		int32               SendFundTrsf(const ZpquantOrdCancelReqT *pFundTrsfReq);
		/* ��ȡ�������� */
		int32               GetTradingDay(void);
		/* ��ȡ�ͻ���������Ϣ */
		//int32               GetClientOverview(OesClientOverviewT *pClientOverview);
		/* ��ѯ����ί����Ϣ�����̰߳�ȫʵ�� */
		//int32               QueryOrder(const OesQryOrdFilterT *pQryFilter, int32 requestId = 0);
		/* ��ѯ�ɽ���Ϣ�����̰߳�ȫʵ�� */
		int32               QueryTrade(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		/* ��ѯ�ͻ��ʽ���Ϣ�����̰߳�ȫʵ�� */
		int32               QueryCashAsset(int32 requestId = 0);
		/* ��ѯ��Ʊ�ֲ���Ϣ�����̰߳�ȫʵ�� */
		int32               QueryStkHolding(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		// /* ��ѯ�ͻ���Ϣ�����̰߳�ȫʵ�� */
		// int32               QueryCustInfo(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		/* ��ѯ֤ȯ�˻���Ϣ�����̰߳�ȫʵ�� */
		int32               QueryInvAcct(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);
		/* ��ѯ�ֻ���Ʒ��Ϣ�����̰߳�ȫʵ�� */
		int32               QueryStock(const ZpquantQryTrd *pQryFilter, int32 requestId = 0);

	private:
		/* ��ֹ�������캯�� */
		CZpquantTradeApi(const CZpquantTradeApi&);
		/* ��ֹ��ֵ���� */
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
