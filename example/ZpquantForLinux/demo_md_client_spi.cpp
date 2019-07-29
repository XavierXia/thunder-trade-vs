#include    <iostream>
#include    "demo_md_client_spi.h"
#include    <stdio.h>
#include    <string.h>

#define  __SPK_FMT_LL__                  "I64"
#define  __SPK_FMT_HH__                  ""
#define  __SPK_FMT_SHORT__               ""


void 
demoMdClientSpi::OnTickRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData)
{
    fprintf(stdout, ">>> OnTick: " \
            "证券代码[%s], " \
            "TradePx[%d], HighPx[%d], " \
            "\n",
            pDepthMarketData->mktDataSnapshot->securityId,
            pDepthMarketData->mktDataSnapshot->TradePx,pDepthMarketData->mktDataSnapshot->HighPx);
}


void 
demoMdClientSpi::OnTradeRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData)
{
    fprintf(stdout, ">>> OnTrade: " \
            "证券代码[%s], " \
            "TradePrice[%d], TradeQty[%d], " \
            "\n",
            pDepthMarketData->trade->securityId,
            pDepthMarketData->trade->TradePrice,pDepthMarketData->trade->TradeQty);
}

void 
demoMdClientSpi::OnOrderRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData)
{
    fprintf(stdout, ">>> OnOrder: " \
            "证券代码[%s], " \
            "Price[%d], OrderQty[%d], " \
            "\n",
            pDepthMarketData->order->securityId,
            pDepthMarketData->order->Price,pDepthMarketData->order->OrderQty);
}

demoMdClientSpi::demoMdClientSpi(int32 something) {
    this->something = something;
    fprintf(stdout, "demoMdClientSpi::demoMdClientSpi");
    //memset(sendJsonDataStr, 0, sizeof(sendJsonDataStr)*4096);
}


demoMdClientSpi::~demoMdClientSpi() {
    /* do nothing */
}
