#include    <iostream>
#include    "demo_md_client_spi.h"
#include    <stdio.h>
#include    <string.h>

#define  __SPK_FMT_LL__                  "I64"
#define  __SPK_FMT_HH__                  ""
#define  __SPK_FMT_SHORT__               ""


void 
OnTickRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData)
{

}


void 
OnTradeRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData)
{

}

void 
OnOrderRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData)
{

}

demoMdClientSpi::demoMdClientSpi(int32 something) {
    this->something = something;
    fprintf(stdout, "demoMdClientSpi::demoMdClientSpi");
    //memset(sendJsonDataStr, 0, sizeof(sendJsonDataStr)*4096);
}


demoMdClientSpi::~demoMdClientSpi() {
    /* do nothing */
}
