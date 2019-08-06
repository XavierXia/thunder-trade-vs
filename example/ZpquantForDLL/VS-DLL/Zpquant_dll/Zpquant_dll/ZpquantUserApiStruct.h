/////////////////////////////////////////////////////////////////////////
///@system 
///@company �����ʲ�
///@file ZpquantUserApiStruct.h
///@brief �����˿ͻ��˽ӿ�ʹ�õ�ҵ�����ݽṹ
///@history 
///2019.7.19	������		�������ļ�
/////////////////////////////////////////////////////////////////////////

#if !defined(ZPQUANT_USERAPISTRUCT_H)
#define ZPQUANT_USERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZpquantUserApiDataType.h"

/**
 ******************************************************
 * ͨ����Ϣ����Ϣ���Ͷ��� -- ������
 ******************************************************
**/

/**
 * ����ģʽ (SubMode) ����
 * - 0: (Set) ���¶���, ����Ϊ�����б��еĹ�Ʊ
 * - 1: (Append) ׷�Ӷ���, ���Ӷ����б��еĹ�Ʊ
 * - 2: (Delete) ɾ������, ɾ�������б��еĹ�Ʊ
 */
typedef enum _zpquantMdsSubscribeMode {
	/** ���¶���, ����Ϊ�����б��еĹ�Ʊ */
	MDS_SUB_MODE_SET = 0,

	/** ׷�Ӷ���, ���Ӷ����б��еĹ�Ʊ */
	MDS_SUB_MODE_APPEND = 1,

	/** ɾ������, ɾ�������б��еĹ�Ʊ */
	MDS_SUB_MODE_DELETE = 2,

	__MAX_MDS_SUB_MODE
} ZpquantMdsSubscribeMode;

/**
 * �ɶ��ĵ��������� (DataType) ����
 * - 0:      Ĭ���������� (����)
 * - 0x0001: L1����/ָ��/��Ȩ
 * - 0x0002: L2����
 * - 0x0004: L2ί�ж���
 * - 0x0008: L2��ʳɽ�
 * - 0x0010: L2���ί�� (����)
 * - 0x0020: L2���⼯�Ͼ��� (�Ϻ�)
 * - 0x0040: L2�г����� (�Ϻ�)
 * - 0x0100: �г�״̬ (�Ϻ�)
 * - 0x0200: ֤ȯʵʱ״̬ (����)
 * - 0x0400: ָ������ (��0x0001����������, 0x0400���Ե�������ָ������)
 * - 0x0800: ��Ȩ���� (��0x0001����������, 0x0800���Ե���������Ȩ����)
 * - 0xFFFF: ������������
 */
typedef enum _zpquantMdsSubscribeDataType {
	/** Ĭ���������� (��������) */
	MDS_SUB_DATA_TYPE_DEFAULT = 0,

	/** L1����/ָ��/��Ȩ (L1�������� + ָ������ + ��Ȩ����) */
	MDS_SUB_DATA_TYPE_L1_SNAPSHOT = 0x0001,

	/** L2���� */
	MDS_SUB_DATA_TYPE_L2_SNAPSHOT = 0x0002,

	/** L2ί�ж��� */
	MDS_SUB_DATA_TYPE_L2_BEST_ORDERS = 0x0004,

	/** L2��ʳɽ� */
	MDS_SUB_DATA_TYPE_L2_TRADE = 0x0008,

	/** L2���ί�� (*����, 0x10/16) */
	MDS_SUB_DATA_TYPE_L2_ORDER = 0x0010,

	/** L2���⼯�Ͼ��� (*�Ϻ�, 0x20/32) */
	MDS_SUB_DATA_TYPE_L2_VIRTUAL_AUCTION_PRICE = 0x0020,

	/** L2�г����� (*�Ϻ�, 0x40/64) */
	MDS_SUB_DATA_TYPE_L2_MARKET_OVERVIEW = 0x0040,

	/** �г�״̬ (*�Ϻ�, 0x100/256) */
	MDS_SUB_DATA_TYPE_TRADING_SESSION_STATUS = 0x0100,

	/** ֤ȯʵʱ״̬ (*����, 0x200/512) */
	MDS_SUB_DATA_TYPE_SECURITY_STATUS = 0x0200,

	/** ָ������ (��L1 _SNAPSHOT����������, INDEX_SNAPSHOT���Ե�������ָ������) */
	MDS_SUB_DATA_TYPE_INDEX_SNAPSHOT = 0x400,

	/** ��Ȩ���� (��L1_SNAPSHOT����������, OPTION_SNAPSHOT���Ե���������Ȩ����) */
	MDS_SUB_DATA_TYPE_OPTION_SNAPSHOT = 0x800,

	/** ���������� (�����ڲ�ָ���κ�������������) */
	MDS_SUB_DATA_TYPE_NONE = 0x8000,

	/** ������������ */
	MDS_SUB_DATA_TYPE_ALL = 0xFFFF,

	__MAX_MDS_SUB_DATA_TYPE = 0x7FFFFFFF
} ZpquantMdsSubscribeDataType;

typedef enum _ZpquantMdsMsgType {
	/*
	 * �Ự����Ϣ
	 */
	 /** ������Ϣ (1/0x01) */
	MDS_MSGTYPE_HEARTBEAT = 1,
	/** ����������Ϣ (2/0x02) */
	MDS_MSGTYPE_TEST_REQUEST = 2,
	/** ��¼��Ϣ (3/0x03) */
	MDS_MSGTYPE_LOGON = 3,
	/** ע����Ϣ (4/0x04) */
	MDS_MSGTYPE_LOGOUT = 4,
	/** ֤ȯ���鶩����Ϣ (5/0x05) */
	MDS_MSGTYPE_MARKET_DATA_REQUEST = 5,
	/** ѹ�������ݰ� (6/0x06, �ڲ�ʹ��) */
	MDS_MSGTYPE_COMPRESSED_PACKETS = 6,
	/** ���ĻỰ��Ϣ���� */
	__MDS_MSGTYPE_SESSION_MAX,

	/*
	 * Level1 ������Ϣ
	 */
	 /** Level1 �г�������� (10/0x0A) */
	 MDS_MSGTYPE_MARKET_DATA_SNAPSHOT_FULL_REFRESH = 10,
	 /** Level1/Level2 ָ��������� (11/0x0B) */
	 MDS_MSGTYPE_INDEX_SNAPSHOT_FULL_REFRESH = 11,
	 /** Level1/Level2 ��Ȩ������� (12/0x0C) */
	 MDS_MSGTYPE_OPTION_SNAPSHOT_FULL_REFRESH = 12,

	 /** �г�״̬��Ϣ (13/0x0D, ���Ϻ�) */
	 MDS_MSGTYPE_TRADING_SESSION_STATUS = 13,
	 /** ֤ȯ״̬��Ϣ (14/0x0E, ������) */
	 MDS_MSGTYPE_SECURITY_STATUS = 14,
	 /** ����Level-1������Ϣ���� */
	 __MDS_MSGTYPE_L1_MAX,

	 /*
	  * Level2 ������Ϣ
	  */
	  /** Level2 �г�������� (20/0x14) */
	  MDS_MSGTYPE_L2_MARKET_DATA_SNAPSHOT = 20,
	  /** Level2 ί�ж��п��� (��һ/��һǰ��ʮ��) (21/0x15) */
	  MDS_MSGTYPE_L2_BEST_ORDERS_SNAPSHOT = 21,

	  /** Level2 ��ʳɽ����� (22/0x16) */
	  MDS_MSGTYPE_L2_TRADE = 22,
	  /** Level2 ���ί������ (23/0x17, ������) */
	  MDS_MSGTYPE_L2_ORDER = 23,

	  /** Level2 �������������������Ϣ (24/0x18, ���Ϻ�) */
	  MDS_MSGTYPE_L2_MARKET_DATA_INCREMENTAL = 24,
	  /** Level2 ί�ж��п��յ�����������Ϣ (25/0x19, ���Ϻ�) */
	  MDS_MSGTYPE_L2_BEST_ORDERS_INCREMENTAL = 25,

	  /** Level2 �г�������Ϣ (26/0x1A, ���Ϻ�) */
	  MDS_MSGTYPE_L2_MARKET_OVERVIEW = 26,
	  /** Level2 ���⼯�Ͼ�����Ϣ (27/0x1B, ���Ϻ�) */
	  MDS_MSGTYPE_L2_VIRTUAL_AUCTION_PRICE = 27,
	  /** ����Level-2������Ϣ���� */
	  __MDS_MSGTYPE_L2_MAX,

	  /*
	   * ��ѯ����Ϣ
	   */
	   /** ��ѯ֤ȯ���� (80/0x50) */
	   MDS_MSGTYPE_QRY_MARKET_DATA_SNAPSHOT = 80,
	   /** ��ѯ(����)֤ȯ״̬ (81/0x51) */
	   MDS_MSGTYPE_QRY_SECURITY_STATUS = 81,
	   /** ��ѯ(��֤)�г�״̬ (82/0x52) */
	   MDS_MSGTYPE_QRY_TRADING_SESSION_STATUS = 82,
	   /** ���Ĳ�ѯ��Ϣ���� */
	   __MDS_MSGTYPE_QRY_MAX

} ZpquantMdsMsgTypeT;

/**
 * ����ͨ����Ϣ����Ϣ���Ͷ���
 */
typedef enum _ZpquantTdResponMsgType {
	/*
	 * ��������Ϣ
	 */
	OESMSG_ORD_NEW_ORDER = 100,     /**< 0x01/01  ί���걨��Ϣ */
	OESMSG_ORD_CANCEL_REQUEST = 101,     /**< 0x02/02  ����������Ϣ */
	OESMSG_ORD_BATCH_ORDERS = 102,     /**< 0x03/03  ����ί����Ϣ */
															/**< ����ί����Ϣ���� */
	/*
	 * ִ�б�������Ϣ
	 */
	__OESMSG_RPT_MIN = 110,     /**< 0x0F/15  ��С��ִ�б�����Ϣ���� */
	OESMSG_RPT_MARKET_STATE = 111,     /**< 0x10/16  �г�״̬��Ϣ */
	OESMSG_RPT_REPORT_SYNCHRONIZATION = 112,     /**< 0x11/17  �ر�ͬ����Ӧ����Ϣ */

	OESMSG_RPT_BUSINESS_REJECT = 113,     /**< 0x12/18  OESҵ��ܾ� (��δͨ����ؼ���ԭ�����OES�ܾ�) */
	OESMSG_RPT_ORDER_INSERT = 114,     /**< 0x13/19  OESί�������� (��ͨ����ؼ��) */
	OESMSG_RPT_ORDER_REPORT = 115,     /**< 0x14/20  ������ί�лر� (����������ί�оܾ���ί��ȷ�Ϻͳ������֪ͨ) */
	OESMSG_RPT_TRADE_REPORT = 116,     /**< 0x15/21  �������ɽ��ر� */

	OESMSG_RPT_FUND_TRSF_REJECT = 117,     /**< 0x16/22  �����ί�оܾ� */
	OESMSG_RPT_FUND_TRSF_REPORT = 118,     /**< 0x17/23  �����ί��ִ�б��� */

	OESMSG_RPT_CASH_ASSET_VARIATION = 119,     /**< 0x18/24  �ʽ�䶯��Ϣ */
	OESMSG_RPT_STOCK_HOLDING_VARIATION = 120,     /**< 0x19/25  �ֱֲ䶯��Ϣ (��Ʊ) */
	OESMSG_RPT_OPTION_HOLDING_VARIATION = 121,     /**< 0x1A/26  �ֱֲ䶯��Ϣ (��Ȩ) */                                                         /**< ���Ļر���Ϣ���� */

	/*
	 * �ǽ�������Ϣ
	 */
	OESMSG_NONTRD_FUND_TRSF_REQ = 130,     /**< 0x21/33  �����ί�� */
	OESMSG_NONTRD_CHANGE_PASSWORD = 131,     /**< 0x22/34  �޸Ŀͻ��˵�¼���� */

	/*
	 * ��ѯ����Ϣ
	 */
	OESMSG_QRYMSG_CLIENT_OVERVIEW = 136,     /**< 0x30/48  ��ѯ�ͻ���������Ϣ */
	OESMSG_QRYMSG_ORD = 137,     /**< 0x31/49  ��ѯί����Ϣ */
	OESMSG_QRYMSG_TRD = 138,     /**< 0x32/50  ��ѯ�ɽ���Ϣ */
	OESMSG_QRYMSG_CASH_ASSET = 139,     /**< 0x33/51  ��ѯ�ͻ��ʽ���Ϣ */
	OESMSG_QRYMSG_STK_HLD = 140,     /**< 0x34/52  ��ѯ��Ʊ�ֲ���Ϣ */
	OESMSG_QRYMSG_OPT_HLD = 141,     /**< 0x35/53  ��ѯ��Ȩ�ֲ���Ϣ */
	OESMSG_QRYMSG_CUST = 142,     /**< 0x36/54  ��ѯ�ͻ���Ϣ */
	OESMSG_QRYMSG_INV_ACCT = 143,     /**< 0x37/55  ��ѯ֤ȯ�˻���Ϣ */
	OESMSG_QRYMSG_COMMISSION_RATE = 144,     /**< 0x38/56  ��ѯ�ͻ�Ӷ����Ϣ */
	OESMSG_QRYMSG_FUND_TRSF = 145,     /**< 0x39/57  ��ѯ�������Ϣ */
	OESMSG_QRYMSG_STOCK = 146,     /**< 0x3A/58  ��ѯ�ֻ���Ʒ��Ϣ */
	OESMSG_QRYMSG_ETF = 147,     /**< 0x3B/59  ��ѯETF�����Ʒ��Ϣ */
	OESMSG_QRYMSG_ETF_COMPONENT = 148,     /**< 0x3C/60  ��ѯETF�ɷֹ���Ϣ */
	OESMSG_QRYMSG_OPTION = 149,     /**< 0x3D/61  ��ѯ��Ȩ��Ʒ��Ϣ */
	OESMSG_QRYMSG_ISSUE = 150,     /**< 0x3E/62  ��ѯ֤ȯ������Ϣ */
	OESMSG_QRYMSG_LOT_WINNING = 151,     /**< 0x3F/63  ��ѯ�¹���š���ǩ��Ϣ */
	OESMSG_QRYMSG_TRADING_DAY = 152,     /**< 0x40/64  ��ѯ��ǰ������ */
	OESMSG_QRYMSG_MARKET_STATE = 153,     /**< 0x41/65  ��ѯ�г�״̬ */
	OESMSG_QRYMSG_COUNTER_CASH = 154,     /**< 0x41/66  ��ѯ�ͻ������ʽ���Ϣ */
	__OESMSG_QRYMSG_MAX,                                    /**< ���Ĳ�ѯ��Ϣ���� */

	/*
	 * �����ĻỰ����Ϣ
	 */
	OESMSG_SESS_TRD_LOGIN = 160,     /**< 0xF1/241 ���׿ͻ��˵�¼��Ϣ */
	OESMSG_SESS_RPT_LOGIN = 161,     /**< 0xF3/243 ִ�б����¼��Ϣ */
	OESMSG_SESS_QRY_LOGIN = 162,     /**< 0xF4/244 ��ͨ��ѯ��¼��Ϣ */
	OESMSG_SESS_HEARTBEAT = 163,     /**< 0xFA/250 ������Ϣ */
	OESMSG_SESS_TEST_REQUEST = 164,     /**< 0xFB/251 ����������Ϣ */
	OESMSG_SESS_LOGOUT = 165,     /**< 0xFE/254 �ǳ���Ϣ */

} ZpquantTdResponMsgTypeT;

/**
 * ���鶩�������Ӧ����
 */
typedef struct _MdsMktDataRequestRsp {
	/**
	 * ����ģʽ
	 * - 0: (Set) ���¶���, ����Ϊ�����б��еĹ�Ʊ
	 * - 1: (Append) ׷�Ӷ���, ���Ӷ����б��еĹ�Ʊ
	 * - 2: (Delete) ɾ������, ɾ�������б��еĹ�Ʊ
	 *
	 * @see eMdsSubscribeModeT
	 */
	uint8               subMode;

	/**
	 * ����ģʽ, �������µ�������ջ�������ʱ�������
	 * -  0: (LatestSimplified) ֻ�������µ������������, �����Ժ������Ѿ���ʱ������
	 *       - ��ģʽ���͵���������С, û���ظ�����, Ҳ�����ظ��������¿���
	 *       - ��ģʽ��ʱ�Ӻʹ����涼�������, ���û����������, �Ƽ�ʹ�ø�ģʽ����
	 * -  1: (LatestTimely) ֻ�������µ������������, ������������������
	 *       - ֻҪ����������¼�, ���������͸ò�Ʒ����������, ��Ҳ������ظ����Ͷ����ͬ����������
	 *       - ���ĳЩ��Ʒ�Ľ��׺ܻ�Ծ, ���ͻ��˴����ֱȽϺ�ʱ�Ļ�, ��ô��ģʽ���ܻ����ʱ�Ļ�ȡ��
	 *         ��Щ��Ʒ����������
	 *       - ����, ��Ϊ�� AllIncrements ģʽ�µ�����һһ��Ӧ, ���Է���������������Ϣ���бȶԲ���
	 *       - ͨ�������, �Ƽ�ʹ�� LatestSimplified ģʽ����
	 * -  2: (AllIncrements) ��������ʱ�������������� (����Level2����������Ϣ)
	 *       - ��ģʽ����������ʱ�����������, ����Level2������յ�����������Ϣ
	 *       - �����Ҫ��ȡȫ����������ϸ, ������Ҫֱ��ʹ��Level2������������Ϣ, ����ʹ�ø�ģʽ
	 *
	 * @see eMdsSubscribedTickTypeT
	 */
	uint8               tickType;

	/**
	 * ������ʵʱ��������֮ǰ, �Ƿ���Ҫ�����Ѷ��Ĳ�Ʒ�ĳ�ʼ���������
	 * �ò���ֻ�ڳ�ʼ����ʱ��Ч, �Ự�����е�ʵʱ���Ľ���֧�ָò���, Ҳ�����ٴ����ͳ�ʼ�������
	 */
	uint8               isRequireInitialMktData;

	/** ���ĵ��ڲ�Ƶ���� (���ڲ�ʹ��, ��δ���⿪��) */
	uint8               __channelNos;

	/**
	 * ������ݵĹ���ʱ������
	 * -  0: ������
	 * -  1: �������� (1��, ������ڿ���1������Ϊ����)
	 * -  2: ��ʱ���� (3��)
	 * -  3: ��ʱ���� (30��)
	 *
	 * @see     eMdsSubscribedTickExpireTypeT
	 * @note    ��Ϊ���ڲ��ɿص���������, �����������ٷְٵľ�ȷ����, ��������ݵ�
	 *          ʱЧ���о�ȷҪ��, ����Ҫ��ǰ�˶������ٽ���һ�ι���
	 */
	uint8               tickExpireType;

	/** ��64λ���������� */
	uint8               __filler[3];

	/**
	 * ���ĵ���������
	 * - 0:      Ĭ���������� (����)
	 * - 0x0001: L1����/ָ��/��Ȩ
	 * - 0x0002: L2����
	 * - 0x0004: L2ί�ж���
	 * - 0x0008: L2��ʳɽ�
	 * - 0x0010: L2���ί�� (����)
	 * - 0x0020: L2���⼯�Ͼ��� (�Ϻ�)
	 * - 0x0040: L2�г����� (�Ϻ�)
	 * - 0x0100: �г�״̬ (�Ϻ�)
	 * - 0x0200: ֤ȯʵʱ״̬ (����)
	 * - 0x0400: ָ������ (��0x0001����������, 0x0400���Ե�������ָ������)
	 * - 0x0800: ��Ȩ���� (��0x0001����������, 0x0800���Ե���������Ȩ����)
	 * - 0xFFFF: ��������
	 *
	 * @see eMdsSubscribeDataTypeT
	 */
	int32               dataTypes;

	/**
	 * �����ĵ��������ݵ���ʼʱ�� (��ʽΪ: HHMMSS �� HHMMSSsss)
	 * - -1: ��ͷ��ʼ��ȡ
	 * -  0: ������λ�ÿ�ʼ��ȡʵʱ����
	 * - >0: ��ָ������ʼʱ�俪ʼ��ȡ (HHMMSS / HHMMSSsss)
	 * - ����Ӧ������, ��Ϊ 0 ���ʾ��ǰû�б���ʼʱ������µ���������
	 */
	int32               beginTime;

	/**
	 * ��֤��Ʊ(ծȯ/����)��Ʒ�Ķ��Ľ�� (ʵ���Ѷ��ĵĲ�Ʒ����)
	 * - -1: ���������в�Ʒ;
	 * -  0: δ���Ļ��ѽ���;
	 * - >0: �Ѷ��ĵĲ�Ʒ����(���ն����б�ɹ����ĵĲ�Ʒ����)
	 */
	int32               sseStockSubscribed;

	/**
	 * ��ָ֤����Ʒ�Ķ��Ľ�� (ʵ���Ѷ��ĵĲ�Ʒ����)
	 * - -1: ���������в�Ʒ;
	 * -  0: δ���Ļ��ѽ���;
	 * - >0: �Ѷ��ĵĲ�Ʒ����(���ն����б�ɹ����ĵĲ�Ʒ����)
	 */
	int32               sseIndexSubscribed;

	/**
	 * ��֤��Ȩ��Ʒ�Ķ��Ľ�� (ʵ���Ѷ��ĵĲ�Ʒ����)
	 * - -1: ���������в�Ʒ;
	 * -  0: δ���Ļ��ѽ���;
	 * - >0: �Ѷ��ĵĲ�Ʒ����(���ն����б�ɹ����ĵĲ�Ʒ����)
	 */
	int32               sseOptionSubscribed;

	/**
	 * ���ڹ�Ʊ(ծȯ/����)��Ʒ�Ķ��Ľ�� (ʵ���Ѷ��ĵĲ�Ʒ����)
	 * - -1: ���������в�Ʒ;
	 * -  0: δ���Ļ��ѽ���;
	 * - >0: �Ѷ��ĵĲ�Ʒ����(���ն����б�ɹ����ĵĲ�Ʒ����)
	 */
	int32               szseStockSubscribed;

	/**
	 * ����ָ����Ʒ�Ķ��Ľ�� (ʵ���Ѷ��ĵĲ�Ʒ����)
	 * - -1: ���������в�Ʒ;
	 * -  0: δ���Ļ��ѽ���;
	 * - >0: �Ѷ��ĵĲ�Ʒ����(���ն����б�ɹ����ĵĲ�Ʒ����)
	 */
	int32               szseIndexSubscribed;

	/**
	 * ������Ȩ��Ʒ�Ķ��Ľ�� (ʵ���Ѷ��ĵĲ�Ʒ����)
	 * - -1: ���������в�Ʒ;
	 * -  0: δ���Ļ��ѽ���;
	 * - >0: �Ѷ��ĵĲ�Ʒ����(���ն����б�ɹ����ĵĲ�Ʒ����)
	 */
	int32               szseOptionSubscribed;

} MdsMktDataRequestRspT;

/**
 * ���������Ӧ����
 */
typedef struct _MdsTestRequestRsp {
	/** ���������ʶ�� */
	char                testReqId[32];

	/** ���������ԭʼ����ʱ�� (timeval�ṹ������'YYYYMMDD-HH:mm:SS.sss'���ַ���) */
	char                origSendTime[22];

	/** ��64λ���������� */
	char                __filler1[2];

	/** ��������Ӧ��ķ���ʱ�� (timeval�ṹ������'YYYYMMDD-HH:mm:SS.sss'���ַ���) */
	char                respTime[22];

	/** ��64λ���������� */
	char                __filler2[2];

} MdsTestRequestRspT;

/**
 * ��¼�����Ӧ����
 */
typedef struct _MdsLogonRsp {
	/** ���ܷ��� */
	int32               encryptMethod;
	/** �������, ��λΪ�� */
	int32               heartBtInt;

	/** �û��� */
	char                username[40];
	/** �������˲��õ�Э��汾�� */
	char                applVerId[32];
	/** �������˼��ݵ����Э��汾�� */
	char                minVerId[32];

	uint8               hostNum;                /**< ������������ */
	uint8               isLeader;               /**< �Ƿ���'���ڵ�' */
	uint8               leaderHostNum;          /**< '���ڵ�'��������� */
	uint8               __filler[5];            /**< ��64λ���������� */
} MdsLogonRspT;

/**
 * Level2 ��ʳɽ����鶨��
 */
typedef struct _MdsL2Trade {
	uint8               exchId;                 /**< ����������(��/��) @see eMdsExchangeIdT */
	uint8               securityType;           /**< ֤ȯ����(��Ʊ/��Ȩ) @see eMdsSecurityTypeT */
	int8                __isRepeated;           /**< �Ƿ����ظ������� (�ڲ�ʹ��, С��0��ʾ���ݵ���) */
	uint8               __filler1;              /**< ��64λ���������� */

	int32               tradeDate;              /**< �������� (YYYYMMDD, �ǹٷ�����) */
	int32               TransactTime;           /**< �ɽ�ʱ�� (HHMMSSsss) */

	int32               instrId;                /**< ��Ʒ���� */
	int32               ChannelNo;              /**< �ɽ�ͨ��/Ƶ������ [0..9999] */
	int32               ApplSeqNum;             /**< �ɽ����/��Ϣ��¼�� (��1��ʼ, ��Ƶ������) */

	/** ��Ʒ���� C6 / C8 (��: '600000' ��) */
	char                SecurityID[9];

	/**
	 * �ɽ���� (�����������, '4'=����, 'F'=�ɽ�)
	 * ������֤, ���̶�����Ϊ 'F'(�ɽ�)
	 * @see eMdsL2TradeExecTypeT
	 */
	char                ExecType;

	/**
	 * �����̱�־ (����������֤, 'B'=����,������, 'S'=����,������, 'N'=δ֪)
	 * �������, ���̶�����Ϊ 'N'(δ֪)
	 * @see eMdsL2TradeBSFlagT
	 */
	char                TradeBSFlag;

	uint8               __filler3[4];           /**< ��64λ���������� */
	uint8               __channelNo;            /**< �ڲ�Ƶ���� (���ڲ�ʹ��, ȡֵ��Χ{1,2,4,8}) */
	uint64              __origTickSeq;          /**< ��Ӧ��ԭʼ��������к� (�ڲ�ʹ��) */

	int32               TradePrice;             /**< �ɽ��۸� (�۸�λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */
	int32               TradeQty;               /**< �ɽ����� (�Ϻ�ծȯ��������λΪ: ��) */
	int64               TradeMoney;             /**< �ɽ���� (��λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */

	int64               BidApplSeqNum;          /**< �򷽶����� (�� 1 ��ʼ����, 0 ��ʾ�޶�Ӧί��) */
	int64               OfferApplSeqNum;        /**< ���������� (�� 1 ��ʼ����, 0 ��ʾ�޶�Ӧί��) */

} MdsL2TradeT;

/**
 * Level2 ���ί�����鶨�� (�����������)
 */
typedef struct _MdsL2Order {
	uint8               exchId;                 /**< ����������(��/��) @see eMdsExchangeIdT */
	uint8               securityType;           /**< ֤ȯ����(��Ʊ/��Ȩ) @see eMdsSecurityTypeT */
	int8                __isRepeated;           /**< �Ƿ����ظ������� (�ڲ�ʹ��, С��0��ʾ���ݵ���) */
	uint8               __filler1;              /**< ��64λ���������� */

	int32               tradeDate;              /**< �������� YYYYMMDD (��Ȼ��) */
	int32               TransactTime;           /**< ί��ʱ�� HHMMSSsss */

	int32               instrId;                /**< ��Ʒ���� */
	int32               ChannelNo;              /**< Ƶ������ [0..9999] */
	int32               ApplSeqNum;             /**< ί����� (��1��ʼ, ��Ƶ������) */

	/** ��Ʒ���� C6 / C8 (��: '000001' ��) */
	char                SecurityID[9];

	/** �������� ('1'=�� '2'=�� 'G'=���� 'F'=����) */
	char                Side;

	/** �������� ('1'=�м� '2'=�޼� 'U'=��������) */
	char                OrderType;

	uint8               __filler3[4];           /**< ��64λ���������� */
	uint8               __channelNo;            /**< �ڲ�Ƶ���� (���ڲ�ʹ��, ȡֵ��Χ{1,2,4,8}) */
	uint64              __origTickSeq;          /**< ��Ӧ��ԭʼ��������к� (�ڲ�ʹ��) */

	int32               Price;                  /**< ί�м۸� (�۸�λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */
	int32               OrderQty;               /**< ί������ */
} MdsL2OrderT;

/**
 * �г�״̬��Ϣ(MsgType=h)���� (���������Ϻ��г�, �����г�û�и�����)
 */
typedef struct _MdsTradingSessionStatusMsg {
	uint8               exchId;                 /**< ����������(��/��) @see eMdsExchangeIdT */
	uint8               securityType;           /**< ֤ȯ����(��Ʊ/��Ȩ) @see eMdsSecurityTypeT */
	int8                __isRepeated;           /**< �Ƿ����ظ������� (���ڲ�ʹ��, С��0 ��ʾ���ݵ���) */
	uint8               __filler1;              /**< ��64λ���������� */

	int32               tradeDate;              /**< �������� (YYYYMMDD, ͨ�����SendingTime�õ�, ���ǹٷ�����) */
	int32               updateTime;             /**< ����ʱ�� (HHMMSSsss, ������ʱ��, ͨ�����SendingTime�õ�, ���ǹٷ�����) */
	int32               __exchSendingTime;      /**< ����������ʱ�� (HHMMSSsss) */
	int32               __mdsRecvTime;          /**< MDS���յ�ʱ�� (HHMMSSsss) */

	int32               TotNoRelatedSym;        /**< ����Ʒ��Ŀ (����ָ��) */

	/**
	 * ȫ�г�����״̬ (*C8)
	 * ���ֶ�Ϊ 8 λ�ַ���,����ÿλ��ʾ�ض��ĺ���,�޶�������ո�
	 * �� 1 λ: ��S����ʾȫ�г������ڼ�(����ǰ), ��T����ʾȫ�г����ڽ����ڼ� (���м�����), ��E����ʾȫ�г����ڱ����ڼ䡣
	 * �� 2 λ: ��1����ʾ���̼��Ͼ��۽�����־, δ����ȡ��0����
	 * �� 3 λ: ��1����ʾ�г�������б�־, δ����ȡ��0����
	 */
	char                TradingSessionID[9];

	uint8               __filler3[3];           /**< ��64λ���������� */
	uint32              __dataVersion;          /**< �������ݵĸ��°汾�� (��__isRepeated!=0ʱ, ��ֵ����Ϊ�ο�ֵ) */
	uint64              __origTickSeq;          /**< ��Ӧ��ԭʼ��������к�(���ڲ�ʹ��) */

} MdsTradingSessionStatusMsgT;

/**
 * ֤ȯʵʱ״̬���� (�������������г�, �Ϻ��г�û�и�����)
 */
typedef struct _MdsSecurityStatusMsg {
	uint8               exchId;                 /**< ����������(��/��) @see eMdsExchangeIdT */
	uint8               securityType;           /**< ֤ȯ����(��Ʊ/��Ȩ) @see eMdsSecurityTypeT */
	int8                __isRepeated;           /**< �Ƿ����ظ������� (���ڲ�ʹ��, С��0 ��ʾ���ݵ���) */
	uint8               __filler1;              /**< ��64λ���������� */

	int32               tradeDate;              /**< �������� (YYYYMMDD, ͨ�������������ʱ��OrigTime�õ�) */
	int32               updateTime;             /**< ����ʱ�� (HHMMSSsss, ������ʱ��, ͨ�������������ʱ��OrigTime�õ�) */
	int32               __exchSendingTime;      /**< ����������ʱ�� (HHMMSSsss, Ŀǰ��ȡ��������ķ���ʱ��, �̶�Ϊ 0) */
	int32               __mdsRecvTime;          /**< MDS���յ�ʱ�� (HHMMSSsss) */

	int32               instrId;                /**< ��Ʒ���� */

	/** ��Ʒ���� C6 / C8 (��: '000001' ��) */
	char                SecurityID[9];

	/**
	 * ֤ȯ״̬ (C8)
	 * A=���й�˾�����¶��ʾ
	 * B=���й�˾�����¶��ʾ
	 */
	char                FinancialStatus[9];

	uint8               __filler2;              /**< ��64λ���������� */
	uint8               __channelNo;            /**< �ڲ�Ƶ���� (���ڲ�ʹ��, ȡֵ��Χ{1,2,4,8}) */
	uint32              __dataVersion;          /**< �������ݵĸ��°汾�� (��__isRepeated!=0ʱ, ��ֵ����Ϊ�ο�ֵ) */
	uint64              __origTickSeq;          /**< ��Ӧ��ԭʼ��������к�(���ڲ�ʹ��) */

	int32               NoSwitch;               /**< ���ظ��� */
	int32               __filler4;              /**< ��64λ���������� */

	/**
	 * ֤ȯҵ�񿪹��б�
	 * ҵ�񿪹��б�Ϊ��������, ������±�λ�ö�Ӧ�ڸ���ҵ�񿪹�, ҵ�񿪹�˵������:
	 *  -  1: ��������, ���������ʱ��֤ȯ
	 *  -  2: ��ȯ����, ��������ȯ���֤ȯ
	 *  -  3: �깺, ������ ETF/LOF �ȿ���ʽ����, ���ڻƽ� ETF ��ָ�ֽ��깺
	 *  -  4: ���, ������ ETF/LOF �ȿ���ʽ����, ���ڻƽ� ETF ��ָ�ֽ���ؿ���
	 *  -  5: �Ϲ�, ���������Ϸ����Ϲ�����
	 *  -  6: ת��, �����ڴ���ת�ɻ����ڵĿ�תծ
	 *  -  7: ����, �����ڴ���ת�ɻ����ڵĿ�תծ
	 *  -  8: ��Ȩ, �����ڴ�����Ȩ�ڵ�Ȩ֤����Ȩ
	 *  - 10: �򿪲�, ��������Ȩ������Ʒ
	 *  - 11: ������, ��������Ȩ������Ʒ
	 *  - 12: �ƽ�ETFʵ���깺, �����ڻƽ� ETF
	 *  - 13: �ƽ�ETFʵ�����, �����ڻƽ� ETF
	 *  - 14: Ԥ��ҪԼ, �����ڴ���ҪԼ�չ��ڵĹ�Ʊ
	 *  - 15: ���ҪԼ, �����ڴ���ҪԼ�չ��ڵĹ�Ʊ
	 *  - 18: ת�ɳ���, �����ڴ���ת�ɻ����ڵĿ�תծ
	 *  - 19: ���۳���, �����ڴ���ת�ɻ����ڵĿ�תծ
	 *  - 20: ��Ѻ, ��������Ѻʽ�ع�����Ѻ���֤ȯ
	 *  - 21: ��Ѻ, ��������Ѻʽ�ع�����Ѻ���֤ȯ
	 *  - 22: ���Ȩ, ���������ȹ�
	 *  - 23: ��Ʊ��Ѻʽ�ع�, �����ڿɿ�չ��Ʊ��Ѻʽ�ع�ҵ���֤ȯ
	 *  - 24: ʵʱ�ֲ�, �����ڷּ�����
	 *  - 25: ʵʱ�ϲ�, �����ڷּ�����
	 *  - 26: ���ҿ���, ��������Ȩ������Ʒ
	 *  - 27: �����̱���, ��������Ȩ��֧�������̱��۵�֤ȯ
	 */
	struct {
		/** ҵ�񿪹ص�ʹ�ܱ�־ (0 δ����, 1 ����) */
		uint8           switchFlag;

		/** ����״̬ (0 �ر�, 1 ����) */
		uint8           switchStatus;
	} switches[32];

} MdsSecurityStatusMsgT;

/**
 * Level1/Level2 ��������(֤ȯ����ȫ����Ϣ)����Ϣͷ����
 */
typedef struct _MdsMktDataSnapshotHead {
	uint8               exchId;                 /**< ����������(��/��) @see eMdsExchangeIdT */
	uint8               securityType;           /**< ֤ȯ����(��Ʊ/��Ȩ) @see eMdsSecurityTypeT */
	int8                __isRepeated;           /**< �Ƿ����ظ������� (�ڲ�ʹ��, С��0��ʾ���ݵ���) */
	uint8               __origMdSource;         /**< ԭʼ����������Դ @see eMdsMsgSourceT */

	int32               tradeDate;              /**< �������� (YYYYMMDD, 8λ������ֵ) */
	int32               updateTime;             /**< ����ʱ�� (HHMMSSsss, ������ʱ��, ֻ���Ϻ�L1���ܻ�ͨ�����SendingTime�õ�) */

	int32               instrId;                /**< ��Ʒ���� */
	int16               bodyLength;             /**< ʵ�����ݳ��� */
	uint8               mdStreamType;           /**< ������������ @see eMdsMdStreamTypeT */
	uint8               __channelNo;            /**< �ڲ�Ƶ���� (���ڲ�ʹ��, ȡֵ��Χ{1,2,4,8}) */
	uint32              __dataVersion;          /**< �������ݵĸ��°汾�� */
	uint64              __origTickSeq;          /**< ��Ӧ��ԭʼ��������к�(���ڲ�ʹ��) */

} MdsMktDataSnapshotHeadT;

/**
 * Level2 ί�ж�����Ϣ (��һ����һǰ��ʮ��ί����ϸ)
 */
typedef struct _MdsL2BestOrdersSnapshotBody {
	/** ��Ʒ���� C6 / C8 (��: '600000' ��) */
	char                SecurityID[9];
	uint8               __filler[5];            /**< ��64λ���������� */
	uint8               NoBidOrders;            /**< ��һ��λ�Ľ�ʾί�б��� */
	uint8               NoOfferOrders;          /**< ��һ��λ�Ľ�ʾί�б��� */

	uint64              TotalVolumeTraded;      /**< �ɽ����� (���Կ�������������ֶ�) */
	int32               BestBidPrice;           /**< ��������� */
	int32               BestOfferPrice;         /**< ���������� */

	/** ��һ��λ��ί����ϸ(ǰ50��) */
	int32               BidOrderQty[50];

	/** ��һ��λ��ί����ϸ(ǰ50��) */
	int32               OfferOrderQty[50];

} MdsL2BestOrdersSnapshotBodyT;

/**
 * Level2 �г�������Ϣ����
 */
typedef struct _MdsL2MarketOverview {
	int32               OrigDate;               /**< �г����� (YYYYMMDD) */
	int32               OrigTime;               /**< �г�ʱ�� (HHMMSSss0, ʵ�ʾ���Ϊ�ٷ�֮һ��(HHMMSSss)) */

	int32               __exchSendingTime;      /**< ����������ʱ�� (HHMMSS000, ʵ�ʾ���Ϊ��(HHMMSS)) */
	int32               __mdsRecvTime;          /**< MDS���յ�ʱ�� (HHMMSSsss) */
} MdsL2MarketOverviewT;

/**
 * ��λ��Ϣ����
 */
typedef struct _MdsPriceLevelEntry {
	int32               Price;                  /**< ί�м۸� */
	int32               NumberOfOrders;         /**< ��λ��ί�б��� (Level1����ʾ��ֵ, �̶�Ϊ0) */
	int64               OrderQty;               /**< ί������ */
} MdsPriceLevelEntryT;

/**
 * Level2 �������鶨��
 * ��Ʊ(A��B��)��ծȯ������
 *
 * ���ڼ��Ͼ����ڼ�����⼯�Ͼ������� (�Ϻ�L2������L2):
 * - ����L2���Ͼ����ڼ������ɽ���ͨ�������̵�λ��ʾ, ������һ����һ����ʾ����ɽ��۸�ͳɽ�����,
 *   �����������ʾ����ɽ���λ�ϵ���ʣ��������ʣ����
 * - �Ϻ�L2�����⼯�Ͼ�������ͨ���������⼯�Ͼ��ۿ�����Ϣ(MdsL2VirtualAuctionPriceT)����
 */
typedef struct _MdsL2StockSnapshotBody {
	/** ��Ʒ���� C6 / C8 (��: '600000' ��) */
	char                SecurityID[9];

	/**
	 * ��Ʒʵʱ�׶μ���־ C8 / C4
	 *
	 * �Ͻ�����Ʊ (C8):
	 *  -# �� 1 λ:
	 *      - ��S����ʾ���� (����ǰ) ʱ��, ��C����ʾ���Ͼ���ʱ��, ��T����ʾ��������ʱ��
	 *      - ��B����ʾ����ʱ��, ��E����ʾ����ʱ��, ��P����ʾ��Ʒͣ��
	 *      - ��M����ʾ�ɻָ����׵��۶�ʱ�� (���м��Ͼ���), ��N����ʾ���ɻָ����׵��۶�ʱ�� (��ͣ����������)
	 *      - ��D����ʾ���̼��Ͼ��۽׶ν������������۽׶ο�ʼ֮ǰ��ʱ�� (����) ��
	 *  -# �� 2 λ:
	 *      - ��0����ʾ�˲�Ʒ������������,
	 *      - ��1����ʾ�˲�Ʒ����������,
	 *      - ��������ո�
	 *  -# �� 3 λ:
	 *      - ��0����ʾδ����, ��1����ʾ�����С�
	 *  -# �� 4 λ:
	 *      - ��0����ʾ�˲�Ʒ�ڵ�ǰʱ�β����ܽ����¶����걨,
	 *      - ��1�� ��ʾ�˲�Ʒ�ڵ�ǰʱ�οɽ��ܽ����¶����걨��
	 *      - ��������ո�
	 *
	 * �Ͻ�����Ȩ (C4):
	 *  -# �� 1 λ:
	 *      - ��S����ʾ����(����ǰ)ʱ��, ��C����ʾ���Ͼ���ʱ��, ��T����ʾ��������ʱ��,
	 *      - ��B����ʾ����ʱ��, ��E����ʾ����ʱ��, ��V����ʾ�������ж�, ��P�� ��ʾ��ʱͣ��, ��U�����̼��Ͼ��ۡ�
	 *      - ��M����ʾ�ɻָ� ���׵��۶�(���м��Ͼ���), ��N����ʾ���ɻָ����׵��۶�(��ͣ����������)
	 *  -# �� 2 λ:
	 *      - ��0����ʾδ����ͣ��, ��1����ʾ����ͣ�ơ�(Ԥ��,����ո�)
	 *  -# �� 3 λ:
	 *      - ��0����ʾ�����ƿ���, ��1����ʾ���Ʊ��ҿ���, ��2����ʾ��������, ��3����ʾ�����������֡����ҿ���,
	 *      - ��4����ʾ�������뿪��, ��5����ʾ�������뿪 �֡����ҿ���,��6����ʾ�������뿪�֡���������,
	 *      - ��7����ʾ�������뿪�֡��������֡����ҿ���
	 *  -# �� 4 λ:
	 *      - ��0����ʾ�˲�Ʒ�ڵ�ǰʱ�β����ܽ����¶����걨, ��1�� ��ʾ�˲�Ʒ�ڵ�ǰʱ�οɽ��ܽ����¶����걨��
	 *
	 * ��� (C8):
	 *  -# �� 0 λ:
	 *      - S=����(����ǰ) O=���̼��Ͼ��� T=��������
	 *      - B=���� C=���̼��Ͼ��� E=�ѱ��� H=��ʱͣ��
	 *      - A=�̺��� V=�������ж�
	 *  -# �� 1 λ:
	 *      - 0=����״̬
	 *      - 1=ȫ��ͣ��
	 */
	char                TradingPhaseCode[9];
	char                __filler[6];            /**< ��64λ���������� */

	uint64              NumTrades;              /**< �ɽ����� */
	uint64              TotalVolumeTraded;      /**< �ɽ����� */
	int64               TotalValueTraded;       /**< �ɽ��ܽ�� (��λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */

	int32               PrevClosePx;            /**< �������̼�/��Ȩ��Լ���ս���� (�۸�λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */
	int32               OpenPx;                 /**< ���̼� (�۸�λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */
	int32               HighPx;                 /**< ��߼� */
	int32               LowPx;                  /**< ��ͼ� */
	int32               TradePx;                /**< �ɽ��� */
	int32               ClosePx;                /**< �����̼�/��Ȩ���̼� (���Ϻ�, ��������û�е��������̼�) */

	int32               IOPV;                   /**< ����ݶ�ο���ֵ/ETF����ĵ�λ�ο���ֵ (�����ڻ���) */
	int32               NAV;                    /**< ���� T-1 �վ�ֵ (�����ڻ���) */
	uint64              TotalLongPosition;      /**< ��Լ�ֲܳ��� (��������Ȩ) */

	int64               TotalBidQty;            /**< ί���������� */
	int64               TotalOfferQty;          /**< ί���������� */
	int32               WeightedAvgBidPx;       /**< ��Ȩƽ��ί��۸� */
	int32               WeightedAvgOfferPx;     /**< ��Ȩƽ��ί���۸� */
	int32               BidPriceLevel;          /**< ��ί�м�λ�� (ʵ�ʵ�ί�м�λ����, ���Ϻ�) */
	int32               OfferPriceLevel;        /**< ����ί�м�λ�� (ʵ�ʵ�ί�м�λ����, ���Ϻ�) */

	/** ʮ�����̼�λ��Ϣ */
	MdsPriceLevelEntryT BidLevels[10];

	/** ʮ�����̼�λ��Ϣ */
	MdsPriceLevelEntryT OfferLevels[10];

} MdsL2StockSnapshotBodyT;

/**
 * ������ Level1/Level2 ֤ȯ����ȫ����Ϣ����
 */
typedef struct _MdsMktDataSnapshot {
	/** �������ݵ���Ϣͷ */
	MdsMktDataSnapshotHeadT                 head;

	union {
		/** Level2 ��������(��Ʊ��ծȯ������) */
		MdsL2StockSnapshotBodyT             l2Stock;
		/** Level2 ί�ж���(��һ����һǰ��ʮ��ί����ϸ) */
		//MdsL2BestOrdersSnapshotBodyT        l2BestOrders;
		/** Level2 �г����� (����֤) */
		//MdsL2MarketOverviewT                l2MarketOverview;
	};
} MdsMktDataSnapshotT;


/**
 * ���ܵ�Ӧ����Ϣ����Ϣ�嶨��
 */
typedef union _MdsMktRspMsgBody {
	/** ֤ȯ����ȫ����Ϣ */
	MdsMktDataSnapshotT             mktDataSnapshot;
	/** Level2 ��ʳɽ����� */
	MdsL2TradeT                     trade;
	/** Level2 ���ί������ */
	MdsL2OrderT                     order;

	/** �г�״̬��Ϣ */
	//MdsTradingSessionStatusMsgT     trdSessionStatus;
	/** ֤ȯʵʱ״̬��Ϣ */
	//MdsSecurityStatusMsgT           securityStatus;
} MdsMktRspMsgBodyT;


/**
 ******************************************************
 * ͨ����Ϣ����Ϣ���Ͷ��� -- ������
 ******************************************************
 */
enum ZpquantMsgType {
	/*
	 * ��������Ϣ
	 */
	MSG_ORD_NEW_ORDER = 1,     /**< 0x01/01  ί���걨��Ϣ */
	MSG_ORD_CANCEL_REQUEST = 2,     /**< 0x02/02  ����������Ϣ */
	MSG_ORD_BATCH_ORDERS = 3,     /**< 0x03/03  ����ί����Ϣ */
	/*
	 * ִ�б�������Ϣ
	 */
	MSG_RPT_MARKET_STATE = 4,     /**< 0x10/16  �г�״̬��Ϣ */
	MSG_RPT_REPORT_SYNCHRONIZATION = 5,     /**< 0x11/17  �ر�ͬ����Ӧ����Ϣ */

	MSG_RPT_BUSINESS_REJECT = 6,     /**< 0x12/18  OESҵ��ܾ� (��δͨ����ؼ���ԭ�����OES�ܾ�) */
	MSG_RPT_ORDER_INSERT = 7,     /**< 0x13/19  OESί�������� (��ͨ����ؼ��) */
	MSG_RPT_ORDER_REPORT = 8,     /**< 0x14/20  ������ί�лر� (����������ί�оܾ���ί��ȷ�Ϻͳ������֪ͨ) */
	MSG_RPT_TRADE_REPORT = 9,     /**< 0x15/21  �������ɽ��ر� */

	MSG_RPT_FUND_TRSF_REJECT = 10,     /**< 0x16/22  �����ί�оܾ� */
	MSG_RPT_FUND_TRSF_REPORT = 11,     /**< 0x17/23  �����ί��ִ�б��� */

	MSG_RPT_CASH_ASSET_VARIATION = 12,     /**< 0x18/24  �ʽ�䶯��Ϣ */
	MSG_RPT_STOCK_HOLDING_VARIATION = 13,     /**< 0x19/25  �ֱֲ䶯��Ϣ (��Ʊ) */
	MSG_RPT_OPTION_HOLDING_VARIATION = 14,     /**< 0x1A/26  �ֱֲ䶯��Ϣ (��Ȩ) */

	/*
	 * �ǽ�������Ϣ
	 */
	MSG_NONTRD_FUND_TRSF_REQ = 15,     /**< 0x21/33  �����ί�� */
	/*
	 * ��ѯ����Ϣ
	 */
	MSG_QRYMSG_CLIENT_OVERVIEW = 16,     /**< 0x30/48  ��ѯ�ͻ���������Ϣ */
	MSG_QRYMSG_ORD = 17,     /**< 0x31/49  ��ѯί����Ϣ */
	MSG_QRYMSG_TRD = 18,     /**< 0x32/50  ��ѯ�ɽ���Ϣ */
	MSG_QRYMSG_CASH_ASSET = 19,     /**< 0x33/51  ��ѯ�ͻ��ʽ���Ϣ */
	MSG_QRYMSG_STK_HLD = 20,     /**< 0x34/52  ��ѯ��Ʊ�ֲ���Ϣ */
	MSG_QRYMSG_OPT_HLD = 21,     /**< 0x35/53  ��ѯ��Ȩ�ֲ���Ϣ */
	MSG_QRYMSG_CUST = 22,     /**< 0x36/54  ��ѯ�ͻ���Ϣ */
	MSG_QRYMSG_INV_ACCT = 23,     /**< 0x37/55  ��ѯ֤ȯ�˻���Ϣ */
	MSG_QRYMSG_COMMISSION_RATE = 24,     /**< 0x38/56  ��ѯ�ͻ�Ӷ����Ϣ */
	MSG_QRYMSG_FUND_TRSF = 25,     /**< 0x39/57  ��ѯ�������Ϣ */
	MSG_QRYMSG_STOCK = 26,     /**< 0x3A/58  ��ѯ�ֻ���Ʒ��Ϣ */
	MSG_QRYMSG_ETF = 27,     /**< 0x3B/59  ��ѯETF�����Ʒ��Ϣ */
	MSG_QRYMSG_ETF_COMPONENT = 28,     /**< 0x3C/60  ��ѯETF�ɷֹ���Ϣ */
	MSG_QRYMSG_OPTION = 29,     /**< 0x3D/61  ��ѯ��Ȩ��Ʒ��Ϣ */
	MSG_QRYMSG_ISSUE = 30,     /**< 0x3E/62  ��ѯ֤ȯ������Ϣ */
	MSG_QRYMSG_LOT_WINNING = 31,     /**< 0x3F/63  ��ѯ�¹���š���ǩ��Ϣ */
	MSG_QRYMSG_TRADING_DAY = 32,     /**< 0x40/64  ��ѯ��ǰ������ */
	MSG_QRYMSG_MARKET_STATE = 33,     /**< 0x41/65  ��ѯ�г�״̬ */
	MSG_QRYMSG_COUNTER_CASH = 34,     /**< 0x41/66  ��ѯ�ͻ������ʽ���Ϣ */
	/*
	 * �����ĻỰ����Ϣ
	 */
	MSG_SESS_TRD_LOGIN = 35,     /**< 0xF1/241 ���׿ͻ��˵�¼��Ϣ */
	MSG_SESS_RPT_LOGIN = 36,     /**< 0xF3/243 ִ�б����¼��Ϣ */
	MSG_SESS_QRY_LOGIN = 37,     /**< 0xF4/244 ��ͨ��ѯ��¼��Ϣ */
	MSG_SESS_HEARTBEAT = 38,     /**< 0xFA/250 ������Ϣ */
	MSG_SESS_TEST_REQUEST = 39,     /**< 0xFB/251 ����������Ϣ */
};

///Fens�û���Ϣ
struct ZpquantUserLoginField
{
	///��ʼ�����
	//CZpquantType type;
	///��ʼ����Դ���
	//CZpquantSourceType sourceType;
	///�û�����
	CZpquantUserIDType UserID;
	///��¼ģʽ
	CZpquantUserPassword UserPassword;

	char strIP[18]; //IP��ַ
	unsigned int uPort; //�˿ں�
};

typedef enum _zpquantInvestorClass {
	INVESTOR_CLASS_NORMAL = 0,        /**< ��ͨͶ���� */
	INVESTOR_CLASS_PROFESSIONAL_A = 1,        /**< A��רҵͶ���� */
	INVESTOR_CLASS_PROFESSIONAL_B = 2,        /**< B��רҵͶ���� */
	INVESTOR_CLASS_PROFESSIONAL_C = 3         /**< C��רҵͶ���� */
} ZpquantInvestorClassT;

/**
 * ���������
 */
typedef enum _zpquantFundTrsfDirect {
	FUND_TRSF_DIRECT_IN = 0,        /**< ת�� (���) */
	FUND_TRSF_DIRECT_OUT = 1         /**< ת�� (����) */
} ZpquantFundTrsfDirectT;

/**
 * OES ����ʱ�ζ���
 */
typedef enum _zpquantTrdSessType {
	TRD_SESS_TYPE_O = 0,        /**< ���̼��Ͼ���ʱ�� */
	TRD_SESS_TYPE_T = 1,        /**< ��������ʱ�� */
	TRD_SESS_TYPE_C = 2,        /**< ���̼��Ͼ��� */
	__TRD_SESS_TYPE_MAX                             /**< ʱ���������ֵ (ʱ����������) */
} pquantTrdSessTypeT;

/**
 * ����ί������
 *
 * @param   mktId           �г����� @see eOesMarketIdT
 * @param   pSecurityId     ��Ʊ���� (char[6]/char[8])
 * @param   pInvAcctId      �ɶ��˻����� (char[10])���� NULL
 * @param   ordType         ί������ @see eOesOrdTypeT, eOesOrdTypeShT, eOesOrdTypeSzT
 * @param   bsType          �������� @sse eOesBuySellTypeT
 * @param   ordQty          ί������ (��λΪ��/��)
 * @param   ordPrice        ί�м۸� (��λ��ȷ��Ԫ����λ����1Ԫ = 10000)
 *
 * @return  ���ڵ���0���ɹ���С��0��ʧ�ܣ�����ţ�
 */
struct  ZpquantOrdReqT
{
	char pSecurityId[10];
	uint8 mktId;
	uint8 ordType;
	uint8 bsType;
	int32 ordQty;
	int32 ordPrice;
	char username[20];
	char pInvAcctId[30];
};

/**
 * ���ͳ�������
 * @param   mktId           ����ί�е��г����� @see eOesMarketIdT
 * @param   pSecurityId     ����ί�й�Ʊ���� (char[6]/char[8]), �ɿ�
 * @param   pInvAcctId      ����ί�йɶ��˻����� (char[10])���ɿ�
 * @param   origClSeqNo     ����ί�е���ˮ�� (��ʹ�� origClOrdId, �򲻱������ֶ�)
 * @param   origClEnvId     ����ί�еĿͻ��˻����� (С�ڵ���0, ��ʹ�õ�ǰ�Ự�� clEnvId)
 * @param   origClOrdId     ����ί�еĿͻ�������� (��ʹ�� origClSeqNo, �򲻱������ֶ�)
 */
struct ZpquantOrdCancelReqT
{
	uint8 mktId;
	int32 origClSeqNo;
	int8 origClEnvId;
	int64 origClOrdId;
	char code[10];
	char pInvAcctId[10];
};

/**
 * �����������
 */
struct ZpquantFundTrsfReqT
{
	/** �ͻ�ί����ˮ�� (�ɿͻ���ά���ĵ�����ˮ) */
	int32 clSeqNo;
	/** ��ת���� @see eOesFundTrsfDirectT */
	uint8 direct;
	/** �����ת������ @see eOesFundTrsfTypeT*/
	uint8 isAllotOnly;
	/** �ʽ��˻����� */
	char cashAcctId[ZP_CASH_ACCT_ID_MAX_LEN];
	/** �������� */
	char trdPasswd[ZP_PWD_MAX_LEN];
	/**
	 * ת������(ת�˷���Ϊת��(����ת֤ȯ), ������Ϊ��������.
	 * ת�˷���Ϊת��(֤ȯת����), ������Ϊ�ʽ�����
	 */
	char trsfPasswd[ZP_PWD_MAX_LEN];
	/** ������� (��������), ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64 occurAmt;
};

//��ѯ �ͻ���������Ϣ
struct ZpquantQryTrd {
	char code[7];
	uint8 sclb;
};

/**
 * ί�оܾ�(OESҵ��ܾ�)�Ľṹ�嶨��
 */
struct ZpquantOrdReject {
	/** �ͻ�ί����ˮ�� (�ɿͻ���ά���ĵ�����ˮ, ����ʶ���ظ���ί���걨) */
	int32           clSeqNo;
	/** �г����� @see eOesMarketIdT */
	uint8           mktId;
	/** �������� @see eOesOrdTypeShT eOesOrdTypeSzT */
	uint8           ordType;
	/** �������� @see eOesBuySellTypeT */
	uint8           bsType;
	/** ֤ȯ�˻� */
	char            invAcctId[16];
	/** ֤ȯ���� */
	char            securityId[16];
	/** ί������ */
	int32           ordQty;
	/** ί�м۸�, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int32           ordPrice;
	/** ԭʼ����(�������Ķ���)�Ŀͻ�������� */
	int64           origClOrdId;
	/** ԭʼ����(�������Ķ���)�Ŀͻ�ί����ˮ�� (�������ڳ�������) */
	int32               origClSeqNo;
	/** ԭʼ����(�������Ķ���)�Ŀͻ��˻����� (�������ڳ�������) */
	int8                origClEnvId;
	/** �ͻ��˻����� */
	int8                clEnvId;
	/** �ͻ��˱�� */
	int16               clientId;
	/** ί������ (��ʽΪ YYYYMMDD, ���� 20160830) */
	int32               ordDate;
	/** ί��ʱ�� (��ʽΪ HHMMSSsss, ���� 141205000) */
	int32               ordTime;
	/** �����ܾ�ԭ�� */
	int32               ordRejReason;
	/** ��64λ���������� */
	int32               __filler;
};

struct ZpquantOrdCnfm
{
	/** �ͻ�ί����ˮ�� (�ɿͻ���ά���ĵ�����ˮ, ����ʶ���ظ���ί���걨) */
	int32           clSeqNo;
	/** �г����� @see eOesMarketIdT */
	uint8           mktId;
	/** �������� @see eOesOrdTypeShT eOesOrdTypeSzT */
	uint8           ordType;
	/** �������� @see eOesBuySellTypeT */
	uint8           bsType;
	/** ֤ȯ�˻� */
	char            invAcctId[16];
	/** ֤ȯ���� */
	char            securityId[16];
	/** ί������ */
	int32           ordQty;
	/** ί�м۸�, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int32           ordPrice;
	/** ԭʼ����(�������Ķ���)�Ŀͻ�������� */
	int64           origClOrdId;
	/** �ͻ�������� (��OES�ھ���Ψһ�Ե��ڲ�ί�б��) */
	int64           clOrdId;
	/** �ͻ��˱�� */
	int16           clientId;
	/** �ͻ��˻����� */
	int8            clEnvId;
	/** ԭʼ����(�������Ķ���)�Ŀͻ��˻����� (�������ڳ���ί��) */
	int8            origClEnvId;
	/** ԭʼ����(�������Ķ���)�Ŀͻ�ί����ˮ�� (�������ڳ���ί��) */
	int32           origClSeqNo;

	/** ί������ (��ʽΪ YYYYMMDD, ���� 20160830) */
	int32           ordDate;
	/** ί��ʱ�� (��ʽΪ HHMMSSsss, ���� 141205000) */
	int32           ordTime;
	/** ί��ȷ��ʱ�� (��ʽΪ HHMMSSsss, ���� 141206000) */
	int32           ordCnfmTime;

	/** ������ǰ״̬ @see eOesOrdStatusT */
	uint8           ordStatus;
	/** ί��ȷ��״̬ (���������صĻر�״̬�������ο�)  @see eOesOrdStatusT */
	uint8           ordCnfmSts;
	/** ֤ȯ���� @see eOesSecurityTypeT */
	uint8           securityType;
	/** ֤ȯ������ @see eOesSubSecurityTypeT */
	uint8           subSecurityType;
	/** ������������� (����Ķ��������16λ�ķ������ַ���) */
	char            exchOrdId[20];
	/** ί�е�ǰ����Ľ��׽�� */
	int64           frzAmt;
	/** ί�е�ǰ�������Ϣ */
	int64           frzInterest;
	/** ί�е�ǰ����Ľ��׷��� */
	int64           frzFee;
	/** ί���ۼ��ѷ����Ľ��׽�� */
	int64           cumAmt;
	/** ί���ۼ��ѷ�������Ϣ */
	int64           cumInterest;
	/** ί���ۼ��ѷ����Ľ��׷��� */
	int64           cumFee;

	/** �ۼ�ִ������ (�ۼƳɽ�����) */
	int32           cumQty;
	/** �ѳ������� */
	int32           canceledQty;

	/** ����/�����ܾ�ԭ�� */
	int32           ordRejReason;
	/** ������������ */
	int32           exchErrCode;
	/** PBU���� (ϯλ��) */
	int32           pbuId;
	/** Ӫҵ������ */
	int32           branchId;
};

/**
 * �ɽ��ر��ṹ�嶨��
 */
struct ZpquantTrdCnfm {
	/** �������ɽ���� (���µ�6���ֶ��ǳɽ���Ϣ�����������ֶ�) */
	int64           exchTrdNum;
	/** �г����� @see eOesMarketIdT */
	uint8           mktId;
	/** �������� (ȡֵ��Χ: ��/��, �깺/���(������)) @see eOesBuySellTypeT */
	uint8           trdSide;
	/** �ɶ��˻����� */
	char            invAcctId[16];
	/** ��Ʒ���� */
	char            securityId[16];

	/** �ɽ����� (��ʽΪ YYYYMMDD, ���� 20160830) */
	int32           trdDate;
	/** �ɽ�ʱ�� (��ʽΪ HHMMSSsss, ���� 141205000) */
	int32           trdTime;
	/** �ɽ����� */
	int32           trdQty;
	/** �ɽ��۸� (��λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */
	int32           trdPrice;
	/** �ɽ���� (��λ��ȷ��Ԫ����λ, ��: 1Ԫ=10000) */
	int64           trdAmt;

	/** �ͻ�������� */
	int64           clOrdId;
	/** �ۼ�ִ������ */
	int32           cumQty;

	/** ԭʼί������ */
	int32           origOrdQty;
	/** PBU���� (ϯλ��) */
	int32           pbuId;
	/** Ӫҵ������ */
	int32           branchId;
	/** �ͻ�ί����ˮ�� */
	int32           clSeqNo;
	/** �ͻ��˱�� */
	int16           clientId;
	/** �ͻ��˻����� */
	int8            clEnvId;
	/** ֤ȯ����� (Ϊ���ּ��ݶ�λ������, ������������) @see eOesSubSecurityTypeT */
	uint8           subSecurityType;

	/** ������ǰ״̬ @see eOesOrdStatusT */
	uint8           ordStatus;
	/** �������� @see eOesOrdTypeShT eOesOrdTypeSzT */
	uint8           ordType;
	/** �������� @see eOesBuySellTypeT */
	uint8           ordBuySellType;
	/** ֤ȯ���� @see eOesSecurityTypeT */
	uint8           securityType;
	/** ԭʼί�м۸�, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int32           origOrdPrice;

	/** �ۼƳɽ���� */
	int64           cumAmt;
	/** �ۼƳɽ���Ϣ */
	int64           cumInterest;
	/** �ۼƽ��׷��� */
	int64           cumFee;
};


/**
 * �ͻ��ʽ���Ϣ����
 */
struct ZpquantCashAssetItem {
	/** �ʽ��˻����� */
	char                cashAcctId[16];
	/** �ͻ����� */
	char                custId[16];

	/** ���� @see eOesCurrTypeT */
	uint8               currType;
	/** �ʽ��ʻ����(�������ʽ��˻�) @see eOesCashTypeT */
	uint8               cashType;
	/** �ʽ��ʻ�״̬(�������ʽ��˻�) @deprecated @see eOesAcctStatusT */
	uint8               cashAcctStatus;
	/** �Ƿ��ֹ����� (����API��ѯʹ��) */
	uint8               isFundTrsfDisabled;
	/** �ڳ����, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               beginningBal;
	/** �ڳ��������, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               beginningAvailableBal;
	/** �ڳ���ȡ���, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               beginningDrawableBal;
	/** �������ʽ����(�Ȳ��ɽ����ֲ�����ȡ), ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               disableBal;
	/** ��ǰ�������(����������ʽ𾻶�), ȡֵ����Ϊ����(��ʾ�ʽ����), ��λ��ȷ��Ԫ����λ(��1Ԫ = 10000) */
	int64               reversalAmt;
	/** �ֶ������ʽ�, ȡֵ��0�͵�ǰ�ʲ�֮��, ��λ��ȷ��Ԫ����λ(��1Ԫ = 10000) */
	int64               manualFrzAmt;

	/** �����ۼƴ����ʽ���, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               totalDepositAmt;
	/** �����ۼ���ȡ�ʽ���, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               totalWithdrawAmt;
	/** ��ǰ��ȡ�����ʽ���, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               withdrawFrzAmt;

	/** �����ۼ�������ʽ���, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               totalSellAmt;
	/** �����ۼ� ��/�깺/��ع� ʹ���ʽ����λ��ȷ��Ԫ����λ����1Ԫ = 10000 */
	int64               totalBuyAmt;
	/** ��ǰ���׶�����, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               buyFrzAmt;

	/** �����ۼƽ��׷��ý��, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               totalFeeAmt;
	/** ��ǰ���ύ�׷��ý��, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               feeFrzAmt;

	/** ��ǰά�ֵı�֤����, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               marginAmt;
	/** ��ǰ����ı�֤����, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               marginFrzAmt;

	/**
	 * ��ǰ���, ������ǰ����������;�����ʽ��ڃȵĻ���ֵ
	 * ���������ο�����ǰ�������(currentAvailableBal)���ֶ�
	 */
	int64               currentTotalBal;
	/** ��ǰ������� */
	int64               currentAvailableBal;
	/** ��ǰ��ȡ��� */
	int64               currentDrawableBal;
};

/**
 * ��Ʊ�ֲֻ�����Ϣ�����ݶ���
 */
#define __ZPQUANT_STK_HOLDING_BASE_INFO_PKT                 \
        /** �˻����� */ \
        char                invAcctId[16]; \
        /** ��Ʒ���� */ \
        char                securityId[16]; \
        /** �г����� @see eOesMarketIdT */ \
        uint8               mktId; \
        /** ֤ȯ���� @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** ֤ȯ������ @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** ��64λ���������� */ \
        uint8               __HOLD_BASE_filler[5]; \
        \
        /** �ճ��ֲ� */ \
        int64               originalHld; \
        /** �ճ��ֲֳܳɱ� (�ճ��ֲֳɱ��� = �ճ��ֲֳܳɱ� / �ճ��ֲ�) */ \
        int64               originalCostAmt; \
        \
        /** �����ۼ�����ֲ� */ \
        int64               totalBuyHld; \
        /** �����ۼ������ֲ� */ \
        int64               totalSellHld; \
        /** ��ǰ��������ֲ� */ \
        int64               sellFrzHld; \
        /** �ֶ�����ֲ� */ \
        int64               manualFrzHld; \
        \
        /** �����ۼ������� */ \
        int64               totalBuyAmt; \
        /** �����ۼ�������� */ \
        int64               totalSellAmt; \
        /** �����ۼ�������� */ \
        int64               totalBuyFee; \
        /** �����ۼ��������� */ \
        int64               totalSellFee; \
        \
        /**
		 * �����ۼ�ת����óֲ�, ETF����ҵ��ʹ��
		 * - �ɷֹɳֲֳ���, ת�����ָ���ʱ��õĳɷֹɳֲ�;
		 * - ETF֤ȯ�ֲֳ���, ת�����ָ�깺ʱ��õ�ETF֤ȯ�ɳֲ�;
		 */ \
	int64               totalTrsfInHld; \
	/**
	 * �����ۼ�ת�������ֲ�, ETF����ҵ��ʹ��
	 * - �ɷֹɳֲֳ���, ת������ָ�깺ʱʹ�õĳɷֹɳֲ�;
	 * - ETF֤ȯ�ֲֳ���, ת������ָ���ʱʹ�õ�ETF֤ȯ�ɳֲ�;
	 */ \
	int64               totalTrsfOutHld; \
	/** ��ǰת����������ֲ� */ \
	int64               trsfOutFrzHld; \
	\
	/** ��ǰ�������ֲ� */ \
	int64               lockHld; \
	/** ��ǰ��������ֲ����� */ \
	int64               lockFrzHld; \
	/** ��ǰ����������ֲ����� */ \
	int64               unlockFrzHld; \
	\
	/** ��ǰ���Ҷ�����ֻ��ֲ����� */ \
	int64               coveredFrzHld; \
	/** ��ǰ�ѱ���ʹ�õ��ֻ��ֲ����� */ \
	int64               coveredHld

	/**
	 * ��ѯ���Ĺ�Ʊ�ֲ���Ϣ����
	 */
	struct ZpquantStkHoldingItem {
	__ZPQUANT_STK_HOLDING_BASE_INFO_PKT;

	/** ��ǰ�����ֲ� */
	int64               sellAvlHld;
	/** ��ǰ��ת�������ֲ� */
	int64               trsfOutAvlHld;
	/** ��ǰ�������ֲ� */
	int64               lockAvlHld;
	/** ��ǰ�����ڱ��ҵ��ֻ��ֲ�, �ɽ����������ֲ����������ͬ */
	int64               coveredAvlHld;
	/**
	 * �ֲܳ�, ������ǰ���óֲ֡����ɽ��׳ֲֺ���;����ֲ��ڃȵĻ���ֵ
	 * �����ֲ���ο�����ǰ�����ֲ�(sellAvlHld)���ֶ�
	 */
	int64               sumHld;
	/** �ֲֳɱ��� */
	int64               costPrice;
};

/**
 * �����ί�л�����Ϣ�����ݶ���
 */
#define __ZPQUANT_FUND_TRSF_BASE_INFO_PKT                   \
        /** �ͻ�ί����ˮ�� (�ɿͻ���ά���ĵ�����ˮ) */ \
        int32           clSeqNo; \
        /** ��ת���� @see eOesFundTrsfDirectT */ \
        uint8           direct; \
        /** �����ת������ @see eOesFundTrsfTypeT*/ \
        uint8           isAllotOnly; \
        \
        /** �ʽ��˻����� */ \
        char            cashAcctId[16]; \
        /** �������� */ \
        char            trdPasswd[40]; \
        /**
		 * ת������(ת�˷���Ϊת��(����ת֤ȯ), ������Ϊ��������.
		 * ת�˷���Ϊת��(֤ȯת����), ������Ϊ�ʽ�����
		 */ \
	char            trsfPasswd[40]; \
	\
	/** ������� (��������), ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */ \
	int64           occurAmt

	/**
	 * �����ܾ��Ļر��ṹ���� (���ؼ��δͨ������OES�ܾ�)
	 */
	struct ZpquantFundTrsfReject {
	__ZPQUANT_FUND_TRSF_BASE_INFO_PKT;

	/** ί������ (��ʽΪ YYYYMMDD, ���� 20160830) */
	int32               ordDate;
	/** ί��ʱ�� (��ʽΪ HHMMSSsss, ���� 141205000) */
	int32               ordTime;

	/** �ͻ��˱�� */
	int16               clientId;
	/** �ͻ��˻����� */
	int8                clEnvId;
	/** 64λ���������� */
	int8                __filler;
	/** ������ */
	int32               rejReason;

	/** ������Ϣ */
	char                errorInfo[64];
};

/**
 * �����ί��ִ��״̬�ر��Ľṹ�嶨��
 */
struct ZpquantFundTrsfReport {
	/** �ͻ�ί����ˮ�� (�ɿͻ���ά���ĵ�����ˮ) */
	int32               clSeqNo;
	/** �ͻ��˱�� */
	int16               clientId;
	/** �ͻ��˻����� */
	int8                clEnvId;
	/** ��ת���� @see eOesFundTrsfDirectT */
	uint8               direct;
	/** �ʽ��˻����� */
	char                cashAcctId[16];

	/** ������� (��������), ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int64               occurAmt;

	/** OES�����ί�б�� (��OES�ھ���Ψһ�Ե��ڲ������ί�б��) */
	int32               fundTrsfId;
	/** ��̨�����ί�б�� */
	int32               counterEntrustNo;

	/** �����ί������ (��ʽΪ YYYYMMDD, ���� 20160830) */
	int32               operDate;
	/** �����ί��ʱ�� (��ʽΪ HHMMSSsss, ���� 141205000) */
	int32               operTime;
	/** �ϱ���̨ʱ�� (��ʽΪ HHMMSSsss, ���� 141205000) */
	int32               dclrTime;
	/** ��ִ̨�н���ɼ�ʱ�� (��ʽΪ HHMMSSsss, ���� 141205000) */
	int32               doneTime;

	/** �����ת������ @see eOesFundTrsfTypeT*/
	uint8               isAllotOnly;
	/** �����ί��ִ��״̬ @see eOesFundTrsfStatusT */
	uint8               trsfStatus;
	/** �Ƿ���ת�˵����� */
	uint8               __hasCounterTransfered;
	/** ��64λ��������� */
	uint8               __filler;

	/** ����ԭ�� */
	int32               rejReason;
	/** ��������� */
	int32               counterErrCode;
	/** ��64λ��������� */
	uint32              __filler2;
	/** �ʽ������ˮ�� */
	char                allotSerialNo[64];
	/** ������Ϣ */
	char                errorInfo[64];
};

/**
 * �г�״̬��Ϣ�Ľṹ�嶨��
 */
struct ZpquantMarketStateInfo {
	uint8               exchId;             /**< ���������� @see eOesExchangeIdT */
	uint8               platformId;         /**< ����ƽ̨���� @see eOesPlatformIdT */
	uint8               mktId;              /**< �г����� @see eOesMarketIdT */
	uint8               mktState;           /**< �г�״̬ @see eOesMarketStatusT */
	uint8               __filler[4];        /**< ��64λ���������� */
};

/**
 * ��ѯ��λ���α�ṹ
 */
struct ZpquantQryCursor {
	/** ��ѯλ�� */
	int32               seqNo;

	/** �Ƿ��ǵ�ǰ���һ���� */
	int8                isEnd;
	/** ��64λ��������� */
	int8                __filler[3];

	/** �û�˽����Ϣ (�ɿͻ����Զ������, ����Ӧ��������ԭ������) */
	int64               userInfo;
};

/**
 * �ͻ�������Ϣ�����ݶ���
 */
#define __ZPQUANT_CUST_BASE_INFO_PKT                        \
        /** �ͻ����� */ \
        char                custId[16]; \
        \
        /** �ͻ����� */ \
        uint8               custType; \
        /** �ͻ�״̬ 0 ����, ��0 ������ */ \
        uint8               status; \
        /** OES���յȼ� @see eOesSecurityRiskLevelT */ \
        uint8               riskLevel; \
        /** �ͻ�ԭʼ���յȼ� */ \
        uint8               originRiskLevel; \
        /** ������־ (TRUE ����Ͷ����, FALSE ����Ͷ����) */ \
        uint8               institutionFlag; \
        /** Ͷ���߷��� @see eOesInvestorClassT */ \
        uint8               investorClass; \
        /** ��64λ��������� */ \
        uint8               __CUST_BASE_filler1[2]; \
        \
        /** Ӫҵ������ */ \
        int32               branchId; \
        /** ��64λ��������� */ \
        uint32              __CUST_BASE_filler2

 /**
  * �ͻ�������Ϣ�Ľṹ�嶨��
  */
struct ZpquantCustBaseInfo {
	__ZPQUANT_CUST_BASE_INFO_PKT;
};

/**
 * ֤ȯ�˻�������Ϣ�����ݶ���
 */
#define __ZPQUANT_INV_ACCT_BASE_INFO_PKT                   \
        /** �ɶ��˻����� */ \
        char                invAcctId[16]; \
        /** �г� @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** �˻����� @see eOesAcctTypeT */ \
        uint8               acctType; \
        /** �˻�״̬ @see eOesAcctStatusT */ \
        uint8               status; \
        /** �ɶ��˻������������� @see eOesOwnerTypeT */ \
        uint8               ownerType; \
        /** ��ȨͶ���߼��� @see eOesOptInvLevelT */ \
        uint8               optInvLevel; \
        /** �Ƿ��ֹ���� (����API��ѯʹ��) */ \
        uint8               isTradeDisabled; \
        /** ��64λ��������� */ \
        uint8               __INV_ACCT_BASE_filler[2]; \
        \
        /** ֤ȯ�˻�Ȩ������ @see eOesTradingLimitT */ \
        uint64              limits; \
        /** �ɶ�Ȩ��/�ͻ�Ȩ�� @see eOesTradingPermissionT */ \
        uint64              permissions; \
        \
        /** ϯλ�� */ \
        int32               pbuId; \
        /** �¹��Ϲ��޶� */ \
        int32               subscriptionQuota

 /**
  * ֤ȯ�˻�����
  */
struct ZpquantInvAcctItem {
	__ZPQUANT_INV_ACCT_BASE_INFO_PKT;

	/** �ͻ����� */
	char                custId[16];
};

/**
 * ���۵��޼۲�������
 */
typedef struct _ZpquantPriceLimit {
	/** �����޼�, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int32               ceilPrice;
	/** �µ��޼�, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */
	int32               floorPrice;
} ZpquantPriceLimitT;

/**
 * �ֻ���Ʒ������Ϣ�����ݶ���
 */
#define __ZPQUANT_STOCK_BASE_INFO_PKT                       \
        /** ��Ʒ���� */ \
        char                securityId[16]; \
        /** �г����� @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** ֤ȯ���� @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** ֤ȯ������ @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** ��Ʒ���� @see eOesSecurityLevelT */ \
        uint8               securityLevel; \
        /** ��Ʒ���յȼ� @see eOesSecurityRiskLevelT */ \
        uint8               securityRiskLevel; \
        /** ���� @see eOesCurrTypeT */ \
        uint8               currType; \
        /** Ͷ�����ʵ��Թ������ @see eOesQualificationClassT */ \
        uint8               qualificationClass; \
        /** �Ƿ�֧�ֵ��ջ�ת���� 0: ��֧��; ����: ֧�� */ \
        uint8               isDayTrading; \
        \
        /** ����ͣ�Ʊ�ʶ @see eOesSecuritySuspFlagT */ \
        uint8               suspFlag; \
        /** ��ʱͣ�Ʊ�ʶ (TRUE ��ͣ��, FALSE δͣ��) */ \
        uint8               temporarySuspFlag; \
        /** ����ַ�  ����64λ���� */ \
        uint8               __filter[6]; \
        \
        /** ���뵥λ */ \
        int32               buyQtyUnit; \
        /** ������λ */ \
        int32               sellQtyUnit; \
        /** ������ί�������� */ \
        int32               buyOrdMaxQty; \
        /** ������ί����С���� */ \
        int32               buyOrdMinQty; \
        /** ������ί�������� */ \
        int32               sellOrdMaxQty; \
        /** ������ί����С���� */ \
        int32               sellOrdMinQty; \
        \
        /** ��С�۸�䶯��λ, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */ \
        int32               priceUnit; \
        /** �������̼ۣ���λ��ȷ��Ԫ����λ����1Ԫ = 10000 */ \
        int32               prevClose; \
        /** ��ֵ, ��λ��ȷ��Ԫ����λ, ��1Ԫ = 10000 */ \
        int64               parPrice; \
        /** ծȯ��ÿ��ԪӦ����Ϣ��, ��λ��ȷ��Ԫ���λ */ \
        int64               bondInterest; \
        \
        /** ��ع����� */ \
        int32               repoExpirationDays; \
        /** ռ������ */ \
        int32               cashHoldDays; \
        \
        /** �����޼۲�����, �����±�Ϊ��ǰʱ�α�־ @see eOesTrdSessTypeT */ \
        ZpquantPriceLimitT      priceLimit[3]; \
        \
        /** ��Ʒ���� */ \
        char                securityName[24]; \
        /** ETF�깺����� */ \
        char                fundId[16]

 /**
  * �ֻ���Ʒ������Ϣ�Ľṹ�嶨��
  */
struct ZpquantStockBaseInfo {
	__ZPQUANT_STOCK_BASE_INFO_PKT;
};


/**
 * ��ѯ����ί����Ϣ����
 */
typedef ZpquantOrdCnfm     ZpquantOrdItem;
/**
 * ��ѯ���ĳɽ���Ϣ����
 */
typedef ZpquantTrdCnfm     ZpquantTrdItem;
/**
 * �ͻ���Ϣ����
 */
typedef ZpquantCustBaseInfo    ZpquantCustItemT;
/**
 * ��ѯ�������ˮ��ϢӦ��
 */
typedef ZpquantFundTrsfReport      ZpquantFundTransferSerialItemT;
/**
 * �ֻ���Ʒ��Ϣ����
 */
typedef ZpquantStockBaseInfo      OesStockItemT;

#endif
#pragma once
