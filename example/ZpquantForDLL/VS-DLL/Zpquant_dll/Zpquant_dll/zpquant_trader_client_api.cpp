#include <iostream>
#include <string.h>
#include "ZpquantTraderApi.h"

void Communicate(const char * address, unsigned int port, const std::stringstream & in, std::stringstream & out);

namespace Zpquant {

	CZpquantTradeApi::CZpquantTradeApi() {
		pSpi = NULL;
		//memset(sendJsonDataStr, 0, sizeof(sendJsonDataStr)*4096);
	}


	CZpquantTradeApi::~CZpquantTradeApi() {
		/* Do nothing */
	}

	/**
	 * ע��spi�ص��ӿ�
	 *
	 * @param   pSpi        spi�ص��ӿ�
	 *
	 * @retval  TRUE: ���óɹ�
	 * @retval  FALSE: ����ʧ��
	 */
	void
		CZpquantTradeApi::RegisterSpi(CZpquantTradeSpi *pSpi) {
		this->pSpi = pSpi;
	}

	//��ʼ������Դ
	bool
		CZpquantTradeApi::InitTraderSource(ZpquantUserLoginField* userLogin) {
		stringstream in, out;
		boost::property_tree::ptree root, result;
		root.put("type", "reqaddtradesource");
		root.put("sourcetype", "kr_td_quant");
		root.put("username", userLogin->UserID);
		root.put("password", userLogin->UserPassword);
		try {
			boost::property_tree::write_json(in, root);
			Communicate(userLogin->strIP, userLogin->uPort, in, out);
			boost::property_tree::read_json(out, result);
		}
		catch (std::exception & err)
		{
			return false;
		}

		for (auto & node : result)
		{
			// auto MD = CString(CA2W(node.first.c_str()));
			// m_vecMDSource.push_back(MD);
			std::cout << "...InitTraderSource,result:" << node.first << endl;
		}

		return true;
	}

	//�ɼ������ص�����
	bool
		CZpquantTradeApi::Start() {
		if (!subscriber.connect()) return false;
		if (!publisher.connect()) return false;

		//ִ�б�����Ϣ���н��պʹ���
		subscriber.subscribe("oes_resp", [this](const string& topic, const string& msg) {
			cout << "...client,oes_resp...subscribe,topic:" << topic << ",msg: " << msg << endl;

			ptree c_Config;
			std::stringstream jmsg(msg.c_str());
			try {
				boost::property_tree::read_json(jmsg, c_Config);
			}
			catch (std::exception & e) {
				fprintf(stdout, "cannot parse from string 'msg(client,oes_resp)' \n");
				return false;
			}


			int32 msgId = c_Config.get<int32>("msgId");
			cout << "...msgId:" << msgId << endl;
			switch (msgId)
			{
				//��ѯ�ֲ�
				/*
				{"msgId":100, "seqNo":1,"isEnd":"Y", "invAcctId":"A188800368","securityId":"600000","mktId":1,
				"originalHld":1000000, "totalBuyHld":0,"totalSellHld":0,"sellFrzHld":0, "totalTrsfInHld":0,
				"totalTrsfOutHld":0,"trsfOutFrzHld":0,"lockHld":0, "lockFrzHld":0,"unlockFrzHld":0,"coveredFrzHld":0,
				"coveredHld":0, "coveredAvlHld":0,"sumHld":1000000,"sellAvlHld":1000000,"trsfOutAvlHld":1000000,"lockAvlHld":1000000}
				  */
			case OESMSG_QRYMSG_STK_HLD:
			{
				ZpquantStkHoldingItem msgBody;
				ZpquantQryCursor pCursor;

				pCursor.seqNo = c_Config.get<int32>("seqNo");
				cout << "...seqNo:" << pCursor.seqNo << endl;

				char isEnd = c_Config.get<char>("isEnd");
				cout << "...isEnd:" << isEnd << endl;
				pCursor.isEnd = isEnd;
				string invAcctId = c_Config.get<string>("invAcctId");
				string securityId = c_Config.get<string>("securityId");
				//if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
				//if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

				msgBody.mktId = c_Config.get<uint8>("mktId");
				msgBody.originalHld = c_Config.get<int64>("originalHld");
				msgBody.totalBuyHld = c_Config.get<int64>("totalBuyHld");
				msgBody.totalSellHld = c_Config.get<int64>("totalSellHld");
				msgBody.sellFrzHld = c_Config.get<int64>("sellFrzHld");
				msgBody.totalTrsfInHld = c_Config.get<int64>("totalTrsfInHld");
				msgBody.totalTrsfOutHld = c_Config.get<int64>("totalTrsfOutHld");
				msgBody.trsfOutFrzHld = c_Config.get<int64>("trsfOutFrzHld");
				msgBody.lockHld = c_Config.get<int64>("lockHld");
				msgBody.lockFrzHld = c_Config.get<int64>("lockFrzHld");
				msgBody.unlockFrzHld = c_Config.get<int64>("unlockFrzHld");
				msgBody.coveredFrzHld = c_Config.get<int64>("coveredFrzHld");
				msgBody.coveredHld = c_Config.get<int64>("coveredHld");
				msgBody.coveredAvlHld = c_Config.get<int64>("coveredAvlHld");
				msgBody.sumHld = c_Config.get<int64>("sumHld");
				msgBody.sellAvlHld = c_Config.get<int64>("sellAvlHld");
				msgBody.trsfOutAvlHld = c_Config.get<int64>("trsfOutAvlHld");
				msgBody.lockAvlHld = c_Config.get<int64>("lockAvlHld");

				this->pSpi->OnQueryStkHolding(&msgBody, &pCursor, 0);
				break;
			}
			/*
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"7","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
			 securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":1,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":0,
			 ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":0, cumAmt":0,"cumInterest":0,"cumFee":0,"cumInterest":6600000, cumFee":0,
			 "frzAmt":50132,"frzInterest":0,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":0, }
			cannot parse from string 'msg(client,oes_resp)'
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"12","cashAcctId":"1888000368","custId":"1888000368","currType":0,
			"cashType":0, cashAcctStatus":0,"beginningBal":1316134912,"beginningAvailableBal":1316134912,"beginningDrawableBal":1316134912,
			disableBal":0,"totalDepositAmt":0,"totalWithdrawAmt":0,"withdrawFrzAmt":0, totalSellAmt":0,"totalBuyAmt":3300000,
			"buyFrzAmt":9900000,"totalFeeAmt":50066, feeFrzAmt":50198,"marginAmt":0,"marginFrzAmt":0,"currentTotalBal":1312784846,
			currentAvailableBal":1302834648,"currentDrawableBal":1302834648 ,}
			cannot parse from string 'msg(client,oes_resp)'
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"8","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
			 securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":3,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":145428627,
			 ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":100, cumAmt":3300000,"cumInterest":0,"cumFee":50066, cumFee":3300000,"frzAmt":0,
			 "frzInterest":66,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":1302834648, }
			cannot parse from string 'msg(client,oes_resp)'
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"9","exchTrdNum":1981,"clOrdId":1375,"clEnvId":0,"clSeqNo":2,
			"invAcctId":"A188800368", securityId":"601899","mktId":1,"trdSide":1, ordBuySellType":1,"trdDate":20190430,"trdTime":145428627,
			trdQty":100,"trdPrice":33000,"trdAmt":3300000,"cumQty":100, cumAmt":3300000,"cumInterest":0,cumFee":50066,"pbuId":88822, }
			cannot parse from string 'msg(client,oes_resp)'
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"12","cashAcctId":"1888000368","custId":"1888000368","currType":0,
			"cashType":0, cashAcctStatus":0,"beginningBal":1316134912,"beginningAvailableBal":1316134912,"beginningDrawableBal":1316134912,
			 disableBal":0,"totalDepositAmt":0,"totalWithdrawAmt":0,"withdrawFrzAmt":0, totalSellAmt":0,"totalBuyAmt":6600000,"buyFrzAmt":6600000,
			 "totalFeeAmt":100132, feeFrzAmt":132,"marginAmt":0,"marginFrzAmt":0,"currentTotalBal":1309434780, currentAvailableBal":1302834648,
			 "currentDrawableBal":1302834648 ,}
			cannot parse from string 'msg(client,oes_resp)'
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"13","invAcctId":"A188800368","securityId":"601899","mktId":1,
			"originalHld":0, totalBuyHld":200,"totalSellHld":0,"sellFrzHld":0,"manualFrzHld":0, totalTrsfInHld":0,"totalTrsfOutHld":0,
			"trsfOutFrzHld":0,"lockHld":0, lockFrzHld":0,"unlockFrzHld":0,"coveredFrzHld":0,"coveredHld":0, originalCostAmt":0,
			"totalBuyAmt":6600000,"totalSellAmt":0,"totalBuyFee":100132, totalSellFee":0,"costPrice":33501,"sumHld":200,"sellAvlHld":0,
			trsfOutAvlHld":200,"lockAvlHld":200,"coveredAvlHld":0, }
			cannot parse from string 'msg(client,oes_resp)'
			*/

			/* ί�����ջر� 114*/
			/*...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"7","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
			 securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":1,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":0,
			 ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":0, cumAmt":0,"cumInterest":0,"cumFee":0,"cumInterest":6600000, cumFee":0,
			 "frzAmt":50132,"frzInterest":0,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":0, }*/
			case OESMSG_RPT_ORDER_INSERT:
				/* ί��ȷ�ϻر� 115*/
				/*
				...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"8","clEnvId":0,"clSeqNo":2,"clOrdId":1375,"invAcctId":"A188800368",
				 securityId":"601899","mktId":1,"ordType":0,"bsType":1, ordStatus":3,"ordDate":20190430,"ordTime":145428627,"ordCnfmTime":145428627,
				 ordQty":200,"ordPrice":33000,"canceledQty":0,"cumQty":100, cumAmt":3300000,"cumInterest":0,"cumFee":50066, cumFee":3300000,"frzAmt":0,
				 "frzInterest":66,"frzFee":0, origClOrdId":0,"ordRejReason":0,"exchErrCode":1302834648, }
				*/
			case OESMSG_RPT_ORDER_REPORT:
			{
				ZpquantOrdCnfm msgBody;

				//if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
				//if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

				msgBody.clSeqNo = c_Config.get<int32>("clSeqNo");
				msgBody.mktId = c_Config.get<uint8>("mktId");
				msgBody.ordType = c_Config.get<uint8>("ordType");
				msgBody.bsType = c_Config.get<uint8>("bsType");
				string invAcctId = c_Config.get<string>("invAcctId");
				string securityId = c_Config.get<string>("securityId");

				msgBody.ordQty = c_Config.get<int32>("ordQty");
				msgBody.ordPrice = c_Config.get<int32>("ordPrice");
				msgBody.origClOrdId = c_Config.get<int64>("origClOrdId");
				msgBody.clOrdId = c_Config.get<int64>("clOrdId");

				msgBody.clEnvId = c_Config.get<int8>("clEnvId");
				//msgBody.origClEnvId = c_Config.get<int8>("origClEnvId");
				//msgBody.origClSeqNo = c_Config.get<int32>("origClSeqNo");
				msgBody.ordDate = c_Config.get<int32>("ordDate");
				msgBody.ordTime = c_Config.get<int32>("ordTime");
				msgBody.ordCnfmTime = c_Config.get<int32>("ordCnfmTime");

				msgBody.ordStatus = c_Config.get<uint8>("ordStatus");
				//msgBody.ordCnfmSts = c_Config.get<uint8>("ordCnfmSts");
				//msgBody.securityType = c_Config.get<uint8>("securityType");
				//string exchOrdId = c_Config.get<string>("exchOrdId");
				msgBody.frzAmt = c_Config.get<int64>("frzAmt");
				msgBody.frzInterest = c_Config.get<int64>("frzInterest");
				msgBody.frzFee = c_Config.get<int64>("frzFee");
				msgBody.cumAmt = c_Config.get<int64>("cumAmt");
				msgBody.cumInterest = c_Config.get<int64>("cumInterest");

				msgBody.cumFee = c_Config.get<int64>("cumFee");
				msgBody.cumQty = c_Config.get<int32>("cumQty");
				msgBody.canceledQty = c_Config.get<int32>("canceledQty");
				msgBody.ordRejReason = c_Config.get<int32>("ordRejReason");
				msgBody.exchErrCode = c_Config.get<int32>("exchErrCode");

				if (msgId == OESMSG_RPT_ORDER_INSERT)
				{
					this->pSpi->OnOrderInsert(&msgBody);
				}
				else /* ί��ȷ�ϻر� */
				{
					this->pSpi->OnOrderReport(0, &msgBody);
				}
				break;
			}
			/*
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"12","cashAcctId":"1888000368","custId":"1888000368","currType":0,
			"cashType":0, cashAcctStatus":0,"beginningBal":1316134912,"beginningAvailableBal":1316134912,"beginningDrawableBal":1316134912,
			disableBal":0,"totalDepositAmt":0,"totalWithdrawAmt":0,"withdrawFrzAmt":0, totalSellAmt":0,"totalBuyAmt":3300000,
			"buyFrzAmt":9900000,"totalFeeAmt":50066, feeFrzAmt":50198,"marginAmt":0,"marginFrzAmt":0,"currentTotalBal":1312784846,
			currentAvailableBal":1302834648,"currentDrawableBal":1302834648 ,}
			*/
			/* �ʽ�䶯�ر� 119*/
			case OESMSG_RPT_CASH_ASSET_VARIATION:
			{
				ZpquantCashAssetItem msgBody;

				string cashAcctId = c_Config.get<string>("cashAcctId");
				string custId = c_Config.get<string>("custId");
				//if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
				//if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

				msgBody.currType = c_Config.get<uint8>("currType");
				msgBody.cashType = c_Config.get<uint8>("cashType");
				msgBody.cashAcctStatus = c_Config.get<uint8>("cashAcctStatus");

				msgBody.beginningBal = c_Config.get<int64>("beginningBal");
				msgBody.beginningAvailableBal = c_Config.get<int64>("beginningAvailableBal");
				msgBody.beginningDrawableBal = c_Config.get<int64>("beginningDrawableBal");
				msgBody.disableBal = c_Config.get<int64>("disableBal");
				msgBody.totalDepositAmt = c_Config.get<int64>("totalDepositAmt");

				msgBody.totalWithdrawAmt = c_Config.get<int64>("totalWithdrawAmt");
				msgBody.withdrawFrzAmt = c_Config.get<int64>("withdrawFrzAmt");
				msgBody.totalBuyAmt = c_Config.get<int64>("totalBuyAmt");
				msgBody.totalSellAmt = c_Config.get<int64>("totalSellAmt");
				msgBody.buyFrzAmt = c_Config.get<int64>("buyFrzAmt");
				msgBody.totalFeeAmt = c_Config.get<int64>("totalFeeAmt");

				msgBody.feeFrzAmt = c_Config.get<int64>("feeFrzAmt");
				msgBody.marginAmt = c_Config.get<int64>("marginAmt");
				msgBody.marginFrzAmt = c_Config.get<int64>("marginFrzAmt");
				msgBody.currentTotalBal = c_Config.get<int64>("currentTotalBal");
				msgBody.currentAvailableBal = c_Config.get<int64>("currentAvailableBal");
				msgBody.currentDrawableBal = c_Config.get<int64>("currentDrawableBal");

				this->pSpi->OnCashAssetVariation(&msgBody);
				break;
			}
			/*
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":"9","exchTrdNum":1981,"clOrdId":1375,"clEnvId":0,"clSeqNo":2,
			"invAcctId":"A188800368", securityId":"601899","mktId":1,"trdSide":1, ordBuySellType":1,"trdDate":20190430,"trdTime":145428627,
			trdQty":100,"trdPrice":33000,"trdAmt":3300000,"cumQty":100, cumAmt":3300000,"cumInterest":0,cumFee":50066,"pbuId":88822, }
			cannot parse from string 'msg(client,oes_resp)'
			*/
			/* �ɽ��ر� 116*/
			case OESMSG_RPT_TRADE_REPORT:
			{
				ZpquantTrdCnfm msgBody;

				string invAcctId = c_Config.get<string>("invAcctId");
				string securityId = c_Config.get<string>("securityId");
				//if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
				//if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

				msgBody.exchTrdNum = c_Config.get<int64>("exchTrdNum");
				msgBody.clOrdId = c_Config.get<int64>("clOrdId");
				msgBody.clEnvId = c_Config.get<int8>("clEnvId");
				msgBody.clSeqNo = c_Config.get<int32>("clSeqNo");

				msgBody.mktId = c_Config.get<uint8>("mktId");
				msgBody.trdSide = c_Config.get<uint8>("trdSide");
				msgBody.ordBuySellType = c_Config.get<int64>("ordBuySellType");
				msgBody.trdDate = c_Config.get<int32>("trdDate");

				msgBody.trdTime = c_Config.get<int32>("trdTime");
				msgBody.trdQty = c_Config.get<int32>("trdQty");
				msgBody.trdPrice = c_Config.get<int32>("trdPrice");
				msgBody.trdAmt = c_Config.get<int64>("trdAmt");
				msgBody.cumAmt = c_Config.get<int64>("cumAmt");
				msgBody.cumInterest = c_Config.get<int64>("cumInterest");

				msgBody.cumFee = c_Config.get<int64>("cumFee");
				msgBody.pbuId = c_Config.get<int64>("pbuId");

				this->pSpi->OnTradeReport(&msgBody);
				break;
			}
			/* �ֱֲ䶯�ر� */
			/*
			...client,oes_resp...subscribe,topic:oes_resp,msg: {"msgId":120,"invAcctId":"A188800368","securityId":"601899",
			"mktId":1,"originalHld":0, "totalBuyHld":1400,"totalSellHld":0,"sellFrzHld":0,"manualFrzHld":0,
			"totalTrsfInHld":0,"totalTrsfOutHld":0,"trsfOutFrzHld":0,"lockHld":0, "lockFrzHld":0,"unlockFrzHld":0,
			"coveredFrzHld":0,"coveredHld":0, "originalCostAmt":0,"totalBuyAmt":45830000,"totalSellAmt":0,
			"totalBuyFee":350915, "totalSellFee":0,"costPrice":32986,"sumHld":1400,"sellAvlHld":0, "trsfOutAvlHld":1400,
			"lockAvlHld":1400,"coveredAvlHld":0}
			*/
			case OESMSG_RPT_STOCK_HOLDING_VARIATION:
			{
				ZpquantStkHoldingItem msgBody;

				string invAcctId = c_Config.get<string>("invAcctId");
				string securityId = c_Config.get<string>("securityId");
				//if (invAcctId != NULL) strncpy(msgBody.invAcctId, invAcctId.c_str(),sizeof(msgBody.invAcctId) - 1);
				//if (securityId != NULL) strncpy(msgBody.securityId, securityId.c_str(),sizeof(msgBody.securityId) - 1);

				msgBody.mktId = c_Config.get<uint8>("mktId");
				msgBody.originalHld = c_Config.get<int64>("originalHld");
				msgBody.totalBuyHld = c_Config.get<int64>("totalBuyHld");
				msgBody.totalSellHld = c_Config.get<int64>("totalSellHld");
				msgBody.sellFrzHld = c_Config.get<int64>("sellFrzHld");
				msgBody.totalTrsfInHld = c_Config.get<int64>("totalTrsfInHld");
				msgBody.totalTrsfOutHld = c_Config.get<int64>("totalTrsfOutHld");
				msgBody.trsfOutFrzHld = c_Config.get<int64>("trsfOutFrzHld");
				msgBody.lockHld = c_Config.get<int64>("lockHld");
				msgBody.lockFrzHld = c_Config.get<int64>("lockFrzHld");
				msgBody.unlockFrzHld = c_Config.get<int64>("unlockFrzHld");
				msgBody.coveredFrzHld = c_Config.get<int64>("coveredFrzHld");
				msgBody.coveredHld = c_Config.get<int64>("coveredHld");
				msgBody.coveredAvlHld = c_Config.get<int64>("coveredAvlHld");
				msgBody.sumHld = c_Config.get<int64>("sumHld");
				msgBody.sellAvlHld = c_Config.get<int64>("sellAvlHld");
				msgBody.trsfOutAvlHld = c_Config.get<int64>("trsfOutAvlHld");
				msgBody.lockAvlHld = c_Config.get<int64>("lockAvlHld");

				this->pSpi->OnStockHoldingVariation(&msgBody);
				break;
			}

			default:
				break;
			}

		});
		return true;
	}

	void
		CZpquantTradeApi::Stop() {
		subscriber.disconnect();
		publisher.disconnect();
	}

	/**
	 * ��ִ�б�����Ϣ���д���Ļص�����
	 *
	 * @param   pRptChannel     �ر�ͨ���ĻỰ��Ϣ
	 * @param   pMsgHead        ��Ϣͷ
	 * @param   pMsgBody        ��Ϣ������
	 * @param   pCallbackParams �ⲿ����Ĳ���
	 *
	 * @return  ���ڵ���0���ɹ���С��0��ʧ�ܣ�����ţ�
	 */
	 // int32
	 // CZpquantTradeApi::HandleReportMsg(OesApiSessionInfoT *pRptChannel,
	 //         SMsgHeadT *pMsgHead, void *pMsgBody, void *pCallbackParams) {
	 //     OesClientSpi        *pSpi = (OesClientSpi *) pCallbackParams;
	 //     OesRspMsgBodyT      *pRspMsg = (OesRspMsgBodyT *) pMsgBody;
	 //     OesRptMsgT          *pRptMsg = &pRspMsg->rptMsg;

	 //     assert(pRptChannel && pMsgHead && pRspMsg);

	 //     switch (pMsgHead->msgId) {
	 //     case OESMSG_RPT_ORDER_INSERT:               /* OESί�������� (��ͨ����ؼ��) */
	 //         pSpi->OnOrderInsert(&pRptMsg->rptBody.ordInsertRsp);
	 //         break;

	 //     case OESMSG_RPT_BUSINESS_REJECT:            /* OESҵ��ܾ� (δͨ����ؼ���) */
	 //         pSpi->OnBusinessReject(pRptMsg->rptHead.ordRejReason,
	 //                 &pRptMsg->rptBody.ordRejectRsp);
	 //         break;

	 //     case OESMSG_RPT_ORDER_REPORT:               /* ������ί�лر� (����������ί�оܾ���ί��ȷ�Ϻͳ������֪ͨ) */
	 //         pSpi->OnOrderReport(pRptMsg->rptHead.ordRejReason,
	 //                 &pRptMsg->rptBody.ordCnfm);
	 //         break;

	 //     case OESMSG_RPT_TRADE_REPORT:               /* �������ɽ��ر� */
	 //         pSpi->OnTradeReport(&pRptMsg->rptBody.trdCnfm);
	 //         break;

	 //     case OESMSG_RPT_CASH_ASSET_VARIATION:       /* �ʽ�䶯��Ϣ */
	 //         pSpi->OnCashAssetVariation(&pRptMsg->rptBody.cashAssetRpt);
	 //         break;

	 //     case OESMSG_RPT_STOCK_HOLDING_VARIATION:    /* �ֱֲ䶯��Ϣ (��Ʊ) */
	 //         pSpi->OnStockHoldingVariation(&pRptMsg->rptBody.stkHoldingRpt);
	 //         break;

	 //     case OESMSG_RPT_FUND_TRSF_REJECT:           /* �����ί����Ӧ-ҵ��ܾ� */
	 //         pSpi->OnFundTrsfReject(pRptMsg->rptHead.ordRejReason,
	 //                 &pRptMsg->rptBody.fundTrsfRejectRsp);
	 //         break;

	 //     case OESMSG_RPT_FUND_TRSF_REPORT:           /* �����ί��ִ�б��� */
	 //         pSpi->OnFundTrsfReport(pRptMsg->rptHead.ordRejReason,
	 //                 &pRptMsg->rptBody.fundTrsfCnfm);
	 //         break;

	 //     case OESMSG_RPT_MARKET_STATE:               /* �г�״̬��Ϣ */
	 //         pSpi->OnMarketState(&pRspMsg->mktStateRpt);
	 //         break;

	 //     case OESMSG_RPT_REPORT_SYNCHRONIZATION: /* �ر�ͬ����Ӧ */
	 //         break;

	 //     case OESMSG_SESS_HEARTBEAT:
	 //         break;

	 //     default:
	 //         fprintf(stderr, "�յ�δ���崦��ʽ�Ļر���Ϣ! msgId[0x%02X]\n",
	 //                 pMsgHead->msgId);
	 //         break;
	 //     }

	 //     return 0;
	 // }

	 /**
	  * ����ί���걨����
	  */
	int32
		CZpquantTradeApi::SendOrder(const ZpquantOrdReqT *pOrderReq)
	{
		string buyorsell;
		if (pOrderReq->bsType == 1) //����
		{
			buyorsell = "buy";
		}
		else if (pOrderReq->bsType == 2) { //����
			buyorsell = "sell";
		}
		else { //����
			buyorsell = "other";
		}

		sprintf(sendJsonDataStr,
			"{\"type\":\"%s\",\"code\":\"%s\",\"sclb\":\"%d\",\"wtfs\":\"%d\",\"amount\":\"%d\",\"price\":\"%d\"}",
			buyorsell.c_str(), pOrderReq->pSecurityId, pOrderReq->mktId, pOrderReq->ordType, pOrderReq->ordQty, pOrderReq->ordPrice);
		cout << "...SendOrder...sendJsonDataStr: " << sendJsonDataStr << endl;
		publisher.publish("order2server_td", sendJsonDataStr);

		return 0;
	}


	/**
	 * ���ͳ�������
	 */
	int32
		CZpquantTradeApi::SendCancelOrder(const ZpquantOrdCancelReqT *pCancelReq)
	{
		string cancelOrd("cancelOrder");
		sprintf(sendJsonDataStr,
			"{\"type\":\"%s\",\"mktId\":%d,\"origClSeqNo\":%d,\"origClEnvId\":%d,\"origClOrdId\":%d}",
			cancelOrd.c_str(), pCancelReq->mktId, pCancelReq->origClSeqNo, pCancelReq->origClEnvId, pCancelReq->origClOrdId);
		cout << "...SendCancelOrder...sendJsonDataStr: " << sendJsonDataStr << endl;
		publisher.publish("order2server_td", sendJsonDataStr);
		return 0;
	}


	/**
	 * ���ͳ����ί������
	 */
	int32
		CZpquantTradeApi::SendFundTrsf(const ZpquantOrdCancelReqT *pFundTrsfReq)
	{
		return 0;
	}

	/**
	 * ��ȡAPI�ķ��а汾��
	 *
	 * @return  API�ķ��а汾�� (��: "0.15.3")
	 */
	const char *
		CZpquantTradeApi::GetVersion(void) {
		string vStr = "0.0.1";
		return vStr.c_str();
	}


	/**
	 * ��ȡ��ǰ������
	 *
	 * @retval  >0          ��ǰ������ (��ʽ��YYYYMMDD)
	 * @retval  0           ��δ���Ӳ�ѯ����
	 * @retval  <0          ʧ�� (���Ĵ����)
	 */
	int32
		CZpquantTradeApi::GetTradingDay(void) {
		return 0;
	}

	/*
	 * ��ѯ�ͻ��ʽ���Ϣ
	 */
	int32
		CZpquantTradeApi::QueryCashAsset(int32 requestId)
	{
		string str = "{\"type\":\"query\",\"category\":\"cashAsset\",\"code\":\"\",\"sclb\":\"\"}";
		cout << "...QueryCashAsset...str: " << str << endl;
		publisher.publish("order2server_td", str);
		return 0;
	}

	//��ѯ ��֤������ ��Ʊ�ĳֲ�
	int32
		CZpquantTradeApi::QueryStkHolding(const ZpquantQryTrd *pQryFilter, int32 requestId)
	{
		sprintf(sendJsonDataStr,
			"{\"type\":\"query\",\"category\":\"stkHolding\",\"code\":\"%s\",\"sclb\":\"%d\"}", pQryFilter->code, pQryFilter->sclb);
		cout << "...stkHolding...sendJsonDataStr: " << sendJsonDataStr << endl;
		publisher.publish("order2server_td", sendJsonDataStr);
		return 0;
	}

	/**
	 * ��ѯ֤ȯ�˻���Ϣ
	 */
	int32
		CZpquantTradeApi::QueryInvAcct(const ZpquantQryTrd *pQryFilter, int32 requestId)
	{
		return 0;
	}

	/**
	 * ��ѯ�ֻ���Ʒ��Ϣ
	 */
	int32
		CZpquantTradeApi::QueryStock(const ZpquantQryTrd *pQryFilter, int32 requestId)
	{
		sprintf(sendJsonDataStr,
			"{\"type\":\"query\",\"category\":\"stkInfo\",\"code\":\"%s\",\"sclb\":\"%d\"}", pQryFilter->code, pQryFilter->sclb);
		cout << "...stkInfo...sendJsonDataStr: " << sendJsonDataStr << endl;
		publisher.publish("order2server_td", sendJsonDataStr);
		return 0;
	}

}

