#ifndef _DEMO_TRADER_CLIENT_SPI_H
#define _DEMO_TRADER_CLIENT_SPI_H


#include "ZpquantTraderApi.h"


class   demoTraderClientSpi: public Zpquant::CZpquantTradeSpi {
public:
    /* 委托拒绝回报回调 */
    virtual void        OnBusinessReject(int32 errorCode, const ZpquantOrdReject *pOrderReject);
    /* 委托已收回报回调 */
    virtual void        OnOrderInsert(const ZpquantOrdCnfm *pOrderInsert);
    /* 委托确认回报回调 */
    virtual void        OnOrderReport(int32 errorCode, const ZpquantOrdCnfm *pOrderReport);
    /* 成交确认回报回调 */
    virtual void        OnTradeReport(const ZpquantTrdCnfm *pTradeReport);
    /* 资金变动回报回调 */
    virtual void        OnCashAssetVariation(const ZpquantCashAssetItem *pCashAssetItem);
    /* 持仓变动回报回调 */
    virtual void        OnStockHoldingVariation(const ZpquantStkHoldingItem *pStkHoldingItem);
    /* 出入金委托拒绝回报回调 */
    virtual void        OnFundTrsfReject(int32 errorCode, const ZpquantFundTrsfReject *pFundTrsfReject);
    /* 出入金委托执行回报回调 */
    virtual void        OnFundTrsfReport(int32 errorCode, const ZpquantFundTrsfReport *pFundTrsfReport);
    /* 市场状态信息回报回调 */
    virtual void        OnMarketState(const ZpquantMarketStateInfo *pMarketStateItem);

    /* 查询委托信息回调 */
    virtual void        OnQueryOrder(const ZpquantOrdItem *pOrder, const ZpquantQryCursor *pCursor, int32 requestId);
    /* 查询成交信息回调 */
    virtual void        OnQueryTrade(const ZpquantTrdItem *pTrade, const ZpquantQryCursor *pCursor, int32 requestId);
    /* 查询资金信息回调 */
    virtual void        OnQueryCashAsset(const ZpquantCashAssetItem *pCashAsset, const ZpquantQryCursor *pCursor, int32 requestId);
    /* 查询持仓信息回调 */
    virtual void        OnQueryStkHolding(const ZpquantStkHoldingItem *pStkHolding, const ZpquantQryCursor *pCursor, int32 requestId);
    /* 查询客户信息回调 */
    virtual void        OnQueryCustInfo(const ZpquantCustItemT *pCust, const ZpquantQryCursor *pCursor, int32 requestId);
    /* 查询股东账户信息回调 */
    virtual void        OnQueryInvAcct(const ZpquantInvAcctItem *pInvAcct, const ZpquantQryCursor *pCursor, int32 requestId);
    /* 查询出入金流水信息回调 */
    virtual void        OnQueryFundTransferSerial(const ZpquantFundTransferSerialItemT *pFundTrsf, const ZpquantQryCursor *pCursor, int32 requestId) = 0;
    /* 查询证券信息回调 */
    virtual void        OnQueryStock(const ZpquantStockBaseInfo *pStock, const ZpquantQryCursor *pCursor, int32 requestId);
    /* 查询市场状态信息回调 */
    virtual void        OnQueryMarketState(const ZpquantMarketStateInfo *pMarketState, const ZpquantQryCursor *pCursor, int32 requestId);

public:
    demoTraderClientSpi(int32 something = 0);
    virtual ~demoTraderClientSpi();

private:
    int32               something;
    char sendJsonDataStr[4096];
};


#endif /* _DEMO_TRADER_CLIENT_SPI_H */
