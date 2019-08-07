#include    <iostream>
#include    "unistd.h"
#include    <string.h>
#include    "ZpquantTraderApi.h"
#include    "demo_trader_client_spi.h"
#include    "ZpquantMdApi.h"
#include    "demo_md_client_spi.h"

int
main(void) {

    //连接后端server TOD
    //connectServer("127.0.0.1","8800");
    fprintf(stdout, "...zpquant_client_main_demo start\n");

    //交易
    Zpquant::CZpquantTradeApi  *pZpquantTradeApi = new Zpquant::CZpquantTradeApi();
    Zpquant::CZpquantTradeSpi  *pZpquantTradeSpi = new demoTraderClientSpi();
    //行情
    Zpquant::CZpquantMdApi  *pZpquantMdApi = new Zpquant::CZpquantMdApi();
    Zpquant::CZpquantMdSpi  *pZpquantMdSpi = new demoMdClientSpi();

    if (!pZpquantTradeApi || !pZpquantTradeSpi) {
        fprintf(stderr, "pZpquantTradeApi/pZpquantTradeSpi内存不足!\n");
        return ENOMEM;
    }

    if (!pZpquantMdApi || !pZpquantMdSpi) {
        fprintf(stderr, "pZpquantMdSpi/pZpquantMdSpi 内存不足!\n");
        return ENOMEM;
    }

        /*
    *************************************
    行情类接口实现
    */
    /* 打印API版本信息 */
    fprintf(stdout, "ZpquantClientApi 版本: %s\n",
            Zpquant::CZpquantMdApi::GetVersion());
    /* 注册spi回调接口 */
    pZpquantMdApi->RegisterSpi(pZpquantMdSpi);

    ZpquantUserLoginField userLoginT;
    strncpy(userLoginT.UserID, "hqtest",sizeof(userLoginT.UserID) - 1);
    strncpy(userLoginT.UserPassword, "123456",sizeof(userLoginT.UserPassword) - 1);
    strncpy(userLoginT.strIP, "47.105.111.100",sizeof(userLoginT.strIP) - 1);
    userLoginT.uPort = 8800;

    // if(!pZpquantMdApi->InitMdSource(&userLoginT)){
    //     fprintf(stderr, "InitMdSource失败!\n");
    //     return EINVAL;
    // }

    //sleep(100);

    if (! pZpquantMdApi->Start()) {
        fprintf(stderr, "启动API失败!\n");
        return EINVAL;
    }

    //pZpquantMdApi->SubscribeMarketData("601881,600519,000001,002415",MDS_SUB_MODE_SET);
	string codelist("601899,600000");
    pZpquantMdApi->SubscribeMarketData(codelist.c_str(),MDS_SUB_MODE_SET);
    //追加
    //pZpquantMdApi->SubscribeMarketData("600519",MDS_SUB_MODE_APPEND);

    /*
    *************************************
    交易类接口实现
    */

    /* 打印API版本信息 */
    fprintf(stdout, "pZpquantTradeApi 版本: %s\n",
            Zpquant::CZpquantTradeApi::GetVersion());

    /* 注册spi回调接口 */
    pZpquantTradeApi->RegisterSpi(pZpquantTradeSpi);

    ZpquantUserLoginField userLoginTradeT;
    strncpy(userLoginTradeT.UserID, "jytest",sizeof(userLoginTradeT.UserID) - 1);
    strncpy(userLoginTradeT.UserPassword, "123456",sizeof(userLoginTradeT.UserPassword) - 1);
    strncpy(userLoginTradeT.strIP, "47.105.111.100",sizeof(userLoginTradeT.strIP) - 1);
    userLoginTradeT.uPort = 8800;

    // if(!pZpquantTradeApi->InitTraderSource(&userLoginTradeT)){
    //     fprintf(stderr, "InitTraderSource!\n");
    //     return EINVAL;
    // }


    /*
     * 设置登录OES时使用的用户名和密码
     * @note 如通过API接口设置，则可以不在配置文件中配置;
     *          支持通过前缀指定密码类型, 如 md5:PASSWORD, txt:PASSWORD
     */
    // pOesApi->SetThreadUsername("customer1");
    // pOesApi->SetThreadPassword("txt:123456");
    // pOesApi->SetThreadPassword("md5:e10adc3949ba59abbe56e057f20f883e");

    /* 启动 */
    if (! pZpquantTradeApi->Start()) {
        fprintf(stderr, "启动API失败!\n");
        return EINVAL;
    }

    /* 打印当前交易日 */
    fprintf(stdout, "服务端交易日: %08d\n", pZpquantTradeApi->GetTradingDay());

            /*
    *************************************
    交易下单接口实例
    */
    /*
    {"msgId":"13", "seqNo":1,"isEnd":"Y", invAcctId":"A188800368","securityId":"600000","mktId":1,
    "originalHld":1000000, totalBuyHld":0,"totalSellHld":0,"sellFrzHld":0, totalTrsfInHld":0,"totalTrsfOutHld":0,
    "trsfOutFrzHld":0,"lockHld":0, lockFrzHld":0,"unlockFrzHld":0,"coveredFrzHld":0,"coveredHld":0, 
    coveredAvlHld":0,"sumHld":1000000,"sellAvlHld":1000000,"trsfOutAvlHld":1000000,lockAvlHld":1000000}
    */
    //查询特定某只股票持仓
    ZpquantQryTrd zQryTrdA;
    strncpy(zQryTrdA.code, "600000",sizeof(zQryTrdA.code) - 1);
    zQryTrdA.sclb = 1;
    pZpquantTradeApi->QueryStkHolding(&zQryTrdA,0);

/*
>>> 查询到股票持仓信息: index[1], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[510050], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[2], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[600000], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[3], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[600004], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[4], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[600006], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[5], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[600007], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[6], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[600008], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[7], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[600009], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[8], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[600010], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[9], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[688001], 日初持仓[100000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[100000], 当前可卖[100000], 当前可转换付出[100000], 当前可锁定[100000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[10], isEnd[N], 证券账户[A188800368], 市场代码[1], 产品代码[688002], 日初持仓[100000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[100000], 当前可卖[100000], 当前可转换付出[100000], 当前可锁定[100000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[11], isEnd[N], 证券账户[0188800368], 市场代码[2], 产品代码[159901], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[12], isEnd[N], 证券账户[0188800368], 市场代码[2], 产品代码[000001], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[13], isEnd[N], 证券账户[0188800368], 市场代码[2], 产品代码[000002], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[14], isEnd[N], 证券账户[0188800368], 市场代码[2], 产品代码[000004], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[15], isEnd[N], 证券账户[0188800368], 市场代码[2], 产品代码[000005], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[16], isEnd[N], 证券账户[0188800368], 市场代码[2], 产品代码[000006], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[17], isEnd[N], 证券账户[0188800368], 市场代码[2], 产品代码[000007], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
...OesClientMySpi::OnQueryStkHolding>>> 查询到股票持仓信息: index[18], isEnd[Y], 证券账户[0188800368], 市场代码[2], 产品代码[000008], 日初持仓[1000000], 日中累计买入[0], 日中累计卖出[0], 当前卖出冻结[0], 日中累计转换获得[0], 日中累计转换付出[0], 当前转换付出冻结[0], 当前已锁定[0], 当前锁定冻结[0], 当前解锁定冻结[0], 当前备兑冻结[0], 当前已备兑使用[0], 当前可备兑/解锁[0], 当前总持仓[1000000], 当前可卖[1000000], 当前可转换付出[1000000], 当前可锁定[1000000]
 */   
    //查询账号中两市场的持仓信息
    ZpquantQryTrd zQryTrdB;
    strncpy(zQryTrdB.code, "allStk",sizeof(zQryTrdB.code) - 1);
    zQryTrdB.sclb = 1;
    pZpquantTradeApi->QueryStkHolding(&zQryTrdB,0);


    //下单买入
    ZpquantOrdReqT zOrdReqT;
    strncpy(zOrdReqT.pSecurityId, "601899",sizeof(zOrdReqT.pSecurityId) - 1);
    zOrdReqT.mktId = 1;
    zOrdReqT.ordType = 0;
    zOrdReqT.bsType = 1; //买入
    zOrdReqT.ordQty = 200;
    zOrdReqT.ordPrice = 33000;
    pZpquantTradeApi->SendOrder(&zOrdReqT);

    // //下单卖出
    // ZpquantOrdReqT zOrdReqTB;
    // strncpy(zOrdReqTB.pSecurityId, "600000",sizeof(zOrdReqTB.pSecurityId) - 1);
    // zOrdReqTB.mktId = 1;
    // zOrdReqTB.ordType = 0;
    // zOrdReqTB.bsType = 2; //卖出
    // zOrdReqTB.ordQty = 100;
    // zOrdReqTB.ordPrice = 32000;
    // pZpquantTradeApi->SendOrder(&zOrdReqTB);

    /* 等待回报消息接收完成 */
    while(1)
    {
		;
    }

    // /* 停止 */
    // pZpquantTradeApi->Stop();

    // delete pZpquantTradeApi;
    // delete pZpquantTradeSpi;
    // delete pZpquantMdApi;
    // delete pZpquantMdSpi;

    return 0;
}