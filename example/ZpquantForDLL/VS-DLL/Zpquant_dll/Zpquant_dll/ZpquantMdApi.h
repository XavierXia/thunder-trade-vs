/////////////////////////////////////////////////////////////////////////
///@system 
///@company �����ʲ�
///@file ZpquantMdApi.h
///@brief �����˿ͻ��˽ӿ�
///@history 
///2019.7.19	������		�������ļ�
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
#include "redox.hpp"


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
		///���tick����֪ͨ
		virtual void OnTickRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData) = 0;

		///���trade����֪ͨ
		virtual void OnTradeRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData) = 0;

		///���order����֪ͨ
		virtual void OnOrderRtnDepthMarketData(MdsMktRspMsgBodyT *pDepthMarketData) = 0;
	};

	class MD_API_EXPORT CZpquantMdApi
	{
	public:
		static const char * GetVersion(void);
		redox::Subscriber subscriber;
		redox::Redox publisher;

	public:
		CZpquantMdApi();

		virtual ~CZpquantMdApi();

		///��ȡAPI�İ汾��Ϣ
		///@retrun ��ȡ���İ汾��
		const char *GetApiVersion();
		/* ������ͨ����ͨ�� */
		bool                Start();
		/* ֹͣ��ͨ����ͨ�� */
		void                Stop();
		//��ʼ������Դ
		bool InitMdSource(ZpquantUserLoginField* userLogin);

		///ע��ص��ӿ�
		///@param pSpi �����Իص��ӿ����ʵ��
		void RegisterSpi(CZpquantMdSpi *pSpi);

		///��������
		///@param ppInstrumentID ��ԼID,������ʽ����600000, 600001, 000001��  
		///@param mdsSubMode ����ģʽ (���¶���/׷�Ӷ���/ɾ������)
		///@remark 
		int SubscribeMarketData(char *ppInstrumentIDStr, ZpquantMdsSubscribeMode mdsSubMode);

	private:
		/* ��ֹ�������캯�� */
		CZpquantMdApi(const CZpquantMdApi&);
		/* ��ֹ��ֵ���� */
		CZpquantMdApi& operator=(const CZpquantMdApi&);


	private:
		CZpquantMdSpi        *pSpi;
	};

}

#endif
#pragma once
