#include    <iostream>
#include    "ZpquantTraderApi.h"
#include    "demo_trader_client_spi.h"

void connectServer(char *netaddress,char* port)
{
    
}

int
main(void) {

    //连接后端server
    connectServer("127.0.0.1","8800");

    Zpquant::CZpquantTradeApi  *pZpquantTradeApi = new Zpquant::CZpquantTradeApi();
    Zpquant::CZpquantTradeSpi  *pZpquantTradeSpi = new demoTraderClientSpi();

    if (!pOesApi || !pOesSpi) {
        fprintf(stderr, "内存不足!\n");
        return ENOMEM;
    }

    /* 打印API版本信息 */
    fprintf(stdout, "OesClientApi 版本: %s\n",
            Zpquant::CZpquantTradeApi::GetVersion());

    /* 注册spi回调接口 */
    pOesApi->RegisterSpi(pOesSpi);

    /* 加载配置文件 */
    if (! pOesApi->LoadCfg("oes_client_sample.conf")) {
        fprintf(stderr, "加载配置文件失败!\n");
        return EINVAL;
    }

    /*
     * 设置登录OES时使用的用户名和密码
     * @note 如通过API接口设置，则可以不在配置文件中配置;
     *          支持通过前缀指定密码类型, 如 md5:PASSWORD, txt:PASSWORD
     */
    // pOesApi->SetThreadUsername("customer1");
    // pOesApi->SetThreadPassword("txt:123456");
    // pOesApi->SetThreadPassword("md5:e10adc3949ba59abbe56e057f20f883e");

    /*
     * 设置客户端本地的设备序列号
     * @note 为满足监管需求，需要设置客户端本机的硬盘序列号
     */
    pOesApi->SetCustomizedDriverId("C02TL13QGVC8");

    /* 启动 */
    if (! pOesApi->Start()) {
        fprintf(stderr, "启动API失败!\n");
        return EINVAL;
    }

    /* 打印当前交易日 */
    fprintf(stdout, "服务端交易日: %08d\n", pOesApi->GetTradingDay());

    /* 查询样例 */
    {
        /* 查询 深交所 现货集中竞价平台 市场状态 */
        _OesClientMain_QueryMarketStatus(pOesApi, OES_EXCH_SZSE,
                OES_PLATFORM_CASH_AUCTION);

        /* 查询 客户端总览信息 */
        _OesClientMain_QueryClientOverview(pOesApi);

        /* 查询 所有关联资金账户的资金信息 */
        _OesClientMain_QueryCashAsset(pOesApi, NULL);

        /* 查询 指定资金账户的资金信息 */
        /* _OesClientMain_QueryCashAsset(pOesApi, "XXXXX"); */

        /* 查询 指定上证 600000 的产品信息 */
        _OesClientMain_QueryStock(pOesApi, "600000",
                OES_MKT_ID_UNDEFINE, OES_SECURITY_TYPE_UNDEFINE,
                OES_SUB_SECURITY_TYPE_UNDEFINE);

        /* 查询 上海A股市场 的产品信息 */
        /* _OesClientMain_QueryStock(pOesApi, NULL,
                OES_MKT_ID_SH_A, OES_SECURITY_TYPE_UNDEFINE,
                OES_SUB_SECURITY_TYPE_UNDEFINE); */

        /* 查询 上证 600000 股票的持仓 */
        _OesClientMain_QueryStkHolding(pOesApi, OES_MKT_ID_SH_A, "600000");

        /* 查询 上证 所有股票持仓 */
        _OesClientMain_QueryStkHolding(pOesApi, OES_MKT_ID_SH_A, NULL);

        /* 查询 沪深两市 所有股票持仓 */
        _OesClientMain_QueryStkHolding(pOesApi, OES_MKT_ID_UNDEFINE, NULL);
    }

    /* 委托样例 */
    {
        /* 以 12.67元 购买 浦发银行(600000) 100股 */
        _OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SH_A, "600000", NULL,
                OES_ORD_TYPE_LMT, OES_BS_TYPE_BUY, 100, 126700);

        /* 以 市价 卖出 平安银行(000001) 200股 */
        _OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SZ_A, "000001", NULL,
                OES_ORD_TYPE_SZ_MTL_BEST, OES_BS_TYPE_SELL, 200, 0);

        /*
         * 以 1.235 的报价做 10万元 GC001(204001)逆回购
         * - 逆回购每张标准券100元，委托份数填 (10万元 除以 100元/张 =) 1000张
         *   上证逆回购报价单位是0.005，份数单位是1000张
         */
        _OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SH_A, "204001", NULL,
                OES_ORD_TYPE_LMT, OES_BS_TYPE_CREDIT_SELL, 1000, 12350);

        /*
         * 以 4.321 的报价做 1千元 R-001(131810)逆回购
         * - 逆回购每张标准券100元，委托份数填 (1千元 除以 100元/张 =) 10张
         *   深证逆回购报价单位是0.001，份数单位是10张
         */
        _OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SZ_A, "131810", NULL,
                OES_ORD_TYPE_LMT, OES_BS_TYPE_CREDIT_SELL, 10, 43210);

        /* 以 11.16元 认购 宏达电子(300726) 500股 */
        _OesClientMain_SendOrder(pOesApi, OES_MKT_ID_SZ_A, "300726", NULL,
                OES_ORD_TYPE_LMT, OES_BS_TYPE_SUBSCRIPTION, 500, 111600);

        /*
         * 新股申购的申购额度的查询方式
         * - 新股申购额度通过 查询股东账户信息(OesApi_QueryInvAcct)接口 返回信息中的
         *   OesInvAcctItemT.subscriptionQuota 来获取
         * - 查询股东账户信息(OesApi_QueryInvAcct)接口 在不指定过滤条件的情况下可以依次返回
         *   沪深两市的股东账户信息，需要通过 OesInvAcctItemT.mktId 来区分不同市场的股东账户
         */
    }

    /* 撤单样例 */
    {
        /* 定义 origOrder 作为模拟的待撤委托 */
        OesOrdCnfmT         origOrder = {NULLOBJ_OES_ORD_CNFM};
        origOrder.mktId = OES_MKT_ID_SH_A;
        origOrder.clEnvId = 1;
        origOrder.clSeqNo = 11;
        origOrder.clOrdId = 111;        /* 真实场景中，待撤委托的clOrdId需要通过回报消息获取 */

        /* 通过待撤委托的 clOrdId 进行撤单 */
        _OesClientMain_CancelOrder(pOesApi, origOrder.mktId, NULL, NULL,
                0, 0, origOrder.clOrdId);

        /*
         * 通过待撤委托的 clSeqNo 进行撤单
         * - 如果撤单时 origClEnvId 填0，则默认会使用当前客户端实例的 clEnvId 作为
         *   待撤委托的 origClEnvId 进行撤单
         */
        _OesClientMain_CancelOrder(pOesApi, origOrder.mktId, NULL, NULL,
                origOrder.clSeqNo, origOrder.clEnvId, 0);
    }

    /* 等待回报消息接收完成 */
    SPK_SLEEP_MS(1000);

    /* 停止 */
    pOesApi->Stop();

    delete pOesApi;
    delete pOesSpi;

    return 0;
}