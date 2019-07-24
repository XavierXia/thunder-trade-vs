#include    <iostream>
#include    "ZpquantTraderApi.h"
#include    "demo_trader_client_spi.h"

const char THE_CONFIG_FILE_NAME[100]="/root/thunder-trade-vs/third/Kr360Quant/conf/oes_client.conf";

void connectServer(char *netaddress,char* port)
{
    
}

int
main(void) {

    //连接后端server TOD
    //connectServer("127.0.0.1","8800");

    Zpquant::CZpquantTradeApi  *pZpquantTradeApi = new Zpquant::CZpquantTradeApi();
    Zpquant::CZpquantTradeSpi  *pZpquantTradeSpi = new demoTraderClientSpi();

    if (!pZpquantTradeApi || !pZpquantTradeSpi) {
        fprintf(stderr, "内存不足!\n");
        return ENOMEM;
    }

    /* 打印API版本信息 */
    fprintf(stdout, "OesClientApi 版本: %s\n",
            Zpquant::CZpquantTradeApi::GetVersion());

    /* 注册spi回调接口 */
    pZpquantTradeApi->RegisterSpi(pZpquantTradeSpi);

    /* 加载配置文件 */
    if (! pZpquantTradeApi->LoadCfg(THE_CONFIG_FILE_NAME)) {
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
    pZpquantTradeApi->SetCustomizedDriverId("C02TL13QGVC8");

    /* 启动 */
    if (! pZpquantTradeApi->Start()) {
        fprintf(stderr, "启动API失败!\n");
        return EINVAL;
    }

    /* 打印当前交易日 */
    fprintf(stdout, "服务端交易日: %08d\n", pZpquantTradeApi->GetTradingDay());


    //查询持仓
    ZpquantQryTrd zQryTrd;
    zQryTrd.code = "601881";
    zQryTrd.sclb = 1;
    pZpquantTradeApi->QueryStkHolding(zQryTrd);


    /* 等待回报消息接收完成 */
    sleep(10);

    /* 停止 */
    pZpquantTradeApi->Stop();

    delete pZpquantTradeApi;
    delete pZpquantTradeSpi;

    return 0;
}