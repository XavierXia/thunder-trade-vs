#include    <iostream>
#include    <unistd.h>
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
    pZpquantMdApi->SubscribeMarketData("601899",MDS_SUB_MODE_SET);
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
    //查询特定某只股票持仓
    ZpquantQryTrd zQryTrdA;
    strncpy(zQryTrdA.code, "601881",sizeof(zQryTrdA.code) - 1);
    zQryTrdA.sclb = 1;
    pZpquantTradeApi->QueryStkHolding(&zQryTrdA,0);

    //查询账号中两市场的持仓信息
    ZpquantQryTrd zQryTrdB;
    strncpy(zQryTrdB.code, "allStk",sizeof(zQryTrdB.code) - 1);
    zQryTrdB.sclb = 1;
    pZpquantTradeApi->QueryStkHolding(&zQryTrdB,0);


    /* 等待回报消息接收完成 */
    while(1)
    {
        sleep(100);
    }

    // /* 停止 */
    // pZpquantTradeApi->Stop();

    // delete pZpquantTradeApi;
    // delete pZpquantTradeSpi;
    // delete pZpquantMdApi;
    // delete pZpquantMdSpi;

    return 0;
}