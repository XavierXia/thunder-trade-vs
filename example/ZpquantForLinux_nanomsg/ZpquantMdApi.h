/////////////////////////////////////////////////////////////////////////
///@system 
///@company 宙澎资产
///@file ZpquantMdApi.h
///@brief 定义了客户端接口
///@history 
///2019.7.19	夏文星		创建该文件
/////////////////////////////////////////////////////////////////////////

#if !defined(ZPQUANT_MDAPI_H)
#define ZPQUANT_MDAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp> 
using namespace boost::property_tree;
using namespace std;

#include "ZpquantUserApiStruct.h"
#include "nn.hpp"


#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_MD_API_EXPORT
#define MD_API_EXPORT __declspec(dllexport)
#else
#define MD_API_EXPORT __declspec(dllimport)
#endif
#else
#define MD_API_EXPORT 
#endif

namespace   Zpquant {

class CZpquantMdSpi
{
public:
    ///深度tick行情通知
    virtual void OnTickRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData) = 0;

    ///深度trade行情通知
    virtual void OnTradeRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData) = 0;

    ///深度order行情通知
    virtual void OnOrderRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData) = 0;
};

class MD_API_EXPORT CZpquantMdApi
{
public:
    static const char * GetVersion(void);

public:
	CZpquantMdApi();

    virtual ~CZpquantMdApi();

	///获取API的版本信息
	///@retrun 获取到的版本号
	const char *GetApiVersion();
	    /* 启动连通数据通道 */
    bool                Start();
    /* 停止连通数据通道 */
    void                Stop();
	//初始化行情源
    bool InitMdSource(ZpquantUserLoginField* userLogin);
	
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	void RegisterSpi(CZpquantMdSpi *pSpi);
	
	///订阅行情
	///@param ppInstrumentID 合约ID,数据形式：“600000, 600001, 000001”  
	///@param mdsSubMode 订阅模式 (重新订阅/追加订阅/删除订阅)
	///@remark 
	int SubscribeMarketData(char *ppInstrumentIDStr,ZpquantMdsSubscribeMode mdsSubMode);

private:
    /* 禁止拷贝构造函数 */
    CZpquantMdApi(const CZpquantMdApi&);
    /* 禁止赋值函数 */
    CZpquantMdApi& operator=(const CZpquantMdApi&);
private:
    /* 内部的回调处理函数，可以考虑不定义在类中 */
    static void *MdThreadMain(void *pParams);
    
private:
    CZpquantMdSpi        *pSpi;
};

}

#endif
