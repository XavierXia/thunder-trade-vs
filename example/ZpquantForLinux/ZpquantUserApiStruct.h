/////////////////////////////////////////////////////////////////////////
///@system 
///@company 宙澎资产
///@file ZpquantUserApiStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
///2019.7.19	夏文星		创建该文件
/////////////////////////////////////////////////////////////////////////

#if !defined(ZPQUANT_USERAPISTRUCT_H)
#define ZPQUANT_USERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZpquantUserApiDataType.h"

/**
 * 通信消息的消息类型定义
 */
enum ZpquantMsgType {
    /*
     * 交易类消息
     */
    MSG_ORD_NEW_ORDER                        = 1,     /**< 0x01/01  委托申报消息 */
    MSG_ORD_CANCEL_REQUEST                   = 2,     /**< 0x02/02  撤单请求消息 */
    MSG_ORD_BATCH_ORDERS                     = 3,     /**< 0x03/03  批量委托消息 */
    /*
     * 执行报告类消息
     */
    MSG_RPT_MARKET_STATE                     = 4,     /**< 0x10/16  市场状态信息 */
    MSG_RPT_REPORT_SYNCHRONIZATION           = 5,     /**< 0x11/17  回报同步的应答消息 */

    MSG_RPT_BUSINESS_REJECT                  = 6,     /**< 0x12/18  OES业务拒绝 (因未通过风控检查等原因而被OES拒绝) */
    MSG_RPT_ORDER_INSERT                     = 7,     /**< 0x13/19  OES委托已生成 (已通过风控检查) */
    MSG_RPT_ORDER_REPORT                     = 8,     /**< 0x14/20  交易所委托回报 (包括交易所委托拒绝、委托确认和撤单完成通知) */
    MSG_RPT_TRADE_REPORT                     = 9,     /**< 0x15/21  交易所成交回报 */

    MSG_RPT_FUND_TRSF_REJECT                 = 10,     /**< 0x16/22  出入金委托拒绝 */
    MSG_RPT_FUND_TRSF_REPORT                 = 11,     /**< 0x17/23  出入金委托执行报告 */

    MSG_RPT_CASH_ASSET_VARIATION             = 12,     /**< 0x18/24  资金变动信息 */
    MSG_RPT_STOCK_HOLDING_VARIATION          = 13,     /**< 0x19/25  持仓变动信息 (股票) */
    MSG_RPT_OPTION_HOLDING_VARIATION         = 14,     /**< 0x1A/26  持仓变动信息 (期权) */

    /*
     * 非交易类消息
     */
    MSG_NONTRD_FUND_TRSF_REQ                 = 15,     /**< 0x21/33  出入金委托 */
    /*
     * 查询类消息
     */
    MSG_QRYMSG_CLIENT_OVERVIEW               = 16,     /**< 0x30/48  查询客户端总览信息 */
    MSG_QRYMSG_ORD                           = 17,     /**< 0x31/49  查询委托信息 */
    MSG_QRYMSG_TRD                           = 18,     /**< 0x32/50  查询成交信息 */
    MSG_QRYMSG_CASH_ASSET                    = 19,     /**< 0x33/51  查询客户资金信息 */
    MSG_QRYMSG_STK_HLD                       = 20,     /**< 0x34/52  查询股票持仓信息 */
    MSG_QRYMSG_OPT_HLD                       = 21,     /**< 0x35/53  查询期权持仓信息 */
    MSG_QRYMSG_CUST                          = 22,     /**< 0x36/54  查询客户信息 */
    MSG_QRYMSG_INV_ACCT                      = 23,     /**< 0x37/55  查询证券账户信息 */
    MSG_QRYMSG_COMMISSION_RATE               = 24,     /**< 0x38/56  查询客户佣金信息 */
    MSG_QRYMSG_FUND_TRSF                     = 25,     /**< 0x39/57  查询出入金信息 */
    MSG_QRYMSG_STOCK                         = 26,     /**< 0x3A/58  查询现货产品信息 */
    MSG_QRYMSG_ETF                           = 27,     /**< 0x3B/59  查询ETF申赎产品信息 */
    MSG_QRYMSG_ETF_COMPONENT                 = 28,     /**< 0x3C/60  查询ETF成分股信息 */
    MSG_QRYMSG_OPTION                        = 29,     /**< 0x3D/61  查询期权产品信息 */
    MSG_QRYMSG_ISSUE                         = 30,     /**< 0x3E/62  查询证券发行信息 */
    MSG_QRYMSG_LOT_WINNING                   = 31,     /**< 0x3F/63  查询新股配号、中签信息 */
    MSG_QRYMSG_TRADING_DAY                   = 32,     /**< 0x40/64  查询当前交易日 */
    MSG_QRYMSG_MARKET_STATE                  = 33,     /**< 0x41/65  查询市场状态 */
    MSG_QRYMSG_COUNTER_CASH                  = 34,     /**< 0x41/66  查询客户主柜资金信息 */
    /*
     * 公共的会话类消息
     */
    MSG_SESS_TRD_LOGIN                       = 35,     /**< 0xF1/241 交易客户端登录消息 */
    MSG_SESS_RPT_LOGIN                       = 36,     /**< 0xF3/243 执行报告登录消息 */
    MSG_SESS_QRY_LOGIN                       = 37,     /**< 0xF4/244 普通查询登录消息 */
    MSG_SESS_HEARTBEAT                       = 38,     /**< 0xFA/250 心跳消息 */
    MSG_SESS_TEST_REQUEST                    = 39,     /**< 0xFB/251 测试请求消息 */
};

///Fens用户信息
struct ZpquantUserLoginField
{
	///初始化类别
	CZpquantType type;
	///初始化资源类别
	CZpquantSourceType sourceType;
	///用户代码
	CZpquantUserIDType UserID;
	///登录模式
	CZpquantUserPassword UserPassword;
};

/**
 * 发送委托请求
 *
 * @param   mktId           市场代码 @see eOesMarketIdT
 * @param   pSecurityId     股票代码 (char[6]/char[8])
 * @param   pInvAcctId      股东账户代码 (char[10])，可 NULL
 * @param   ordType         委托类型 @see eOesOrdTypeT, eOesOrdTypeShT, eOesOrdTypeSzT
 * @param   bsType          买卖类型 @sse eOesBuySellTypeT
 * @param   ordQty          委托数量 (单位为股/张)
 * @param   ordPrice        委托价格 (单位精确到元后四位，即1元 = 10000)
 *
 * @return  大于等于0，成功；小于0，失败（错误号）
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
 * 发送撤单请求
 * @param   mktId           被撤委托的市场代码 @see eOesMarketIdT
 * @param   pSecurityId     被撤委托股票代码 (char[6]/char[8]), 可空
 * @param   pInvAcctId      被撤委托股东账户代码 (char[10])，可空
 * @param   origClSeqNo     被撤委托的流水号 (若使用 origClOrdId, 则不必填充该字段)
 * @param   origClEnvId     被撤委托的客户端环境号 (小于等于0, 则使用当前会话的 clEnvId)
 * @param   origClOrdId     被撤委托的客户订单编号 (若使用 origClSeqNo, 则不必填充该字段)
 */
struct ZpquantOrdCancelReqT
{
	uint8 mktId;
	char code[10];
	char pInvAcctId[10];
	int32 origClSeqNo;
	int8 origClEnvId;
	int64 origClOrdId;
};

/**
 * 出入金请求定义
 */
struct ZpquantFundTrsfReqT
{
    /** 客户委托流水号 (由客户端维护的递增流水) */
    int32 clSeqNo;
    /** 划转方向 @see eOesFundTrsfDirectT */
    uint8 direct;
    /** 出入金转账类型 @see eOesFundTrsfTypeT*/
    uint8 isAllotOnly;
    /** 资金账户代码 */ 
    char cashAcctId[ZP_CASH_ACCT_ID_MAX_LEN]; 
    /** 交易密码 */ 
    char trdPasswd[ZP_PWD_MAX_LEN]; 
    /**
     * 转账密码(转账方向为转入(银行转证券), 此密码为银行密码.
     * 转账方向为转出(证券转银行), 此密码为资金密码
     */
    char trsfPasswd[ZP_PWD_MAX_LEN];
    /** 发生金额 (都是正数), 单位精确到元后四位, 即1元 = 10000 */ 
    int64 occurAmt; 
};

//查询 客户端总览信息
struct ZpquantQryTrd {
	char code[10];
	uint8 sclb;
};

/**
 * 委托拒绝(OES业务拒绝)的结构体定义
 */
struct ZpquantOrdReject {
     /** 客户委托流水号 (由客户端维护的递增流水, 用于识别重复的委托申报) */ 
    int32           clSeqNo; 
    /** 市场代码 @see eOesMarketIdT */ 
    uint8           mktId; 
    /** 订单类型 @see eOesOrdTypeShT eOesOrdTypeSzT */ 
    uint8           ordType; 
    /** 买卖类型 @see eOesBuySellTypeT */ 
    uint8           bsType; 
    /** 证券账户 */ 
    char            invAcctId[16]; 
    /** 证券代码 */ 
    char            securityId[16]; 
    /** 委托数量 */ 
    int32           ordQty; 
    /** 委托价格, 单位精确到元后四位, 即1元 = 10000 */ 
    int32           ordPrice; 
    /** 原始订单(待撤销的订单)的客户订单编号 */ 
    int64           origClOrdId; 
    /** 原始订单(待撤销的订单)的客户委托流水号 (仅适用于撤单请求) */
    int32               origClSeqNo;
    /** 原始订单(待撤销的订单)的客户端环境号 (仅适用于撤单请求) */
    int8                origClEnvId;
    /** 客户端环境号 */
    int8                clEnvId;
    /** 客户端编号 */
    int16               clientId;
    /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */
    int32               ordDate;
    /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               ordTime;
    /** 订单拒绝原因 */
    int32               ordRejReason;
    /** 按64位对齐的填充域 */
    int32               __filler;
};

struct ZpquantOrdCnfm
{
	    /** 客户委托流水号 (由客户端维护的递增流水, 用于识别重复的委托申报) */ 
        int32           clSeqNo; 
        /** 市场代码 @see eOesMarketIdT */ 
        uint8           mktId; 
        /** 订单类型 @see eOesOrdTypeShT eOesOrdTypeSzT */ 
        uint8           ordType; 
        /** 买卖类型 @see eOesBuySellTypeT */ 
        uint8           bsType; 
        /** 证券账户 */ 
        char            invAcctId[16]; 
        /** 证券代码 */ 
        char            securityId[16]; 
        /** 委托数量 */ 
        int32           ordQty; 
        /** 委托价格, 单位精确到元后四位, 即1元 = 10000 */ 
        int32           ordPrice; 
        /** 原始订单(待撤销的订单)的客户订单编号 */ 
        int64           origClOrdId; 
                /** 客户订单编号 (在OES内具有唯一性的内部委托编号) */ 
        int64           clOrdId; 
        /** 客户端编号 */ 
        int16           clientId; 
        /** 客户端环境号 */ 
        int8            clEnvId; 
        /** 原始订单(待撤销的订单)的客户端环境号 (仅适用于撤单委托) */ 
        int8            origClEnvId; 
        /** 原始订单(待撤销的订单)的客户委托流水号 (仅适用于撤单委托) */ 
        int32           origClSeqNo; 
        
        /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */ 
        int32           ordDate; 
        /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */ 
        int32           ordTime; 
        /** 委托确认时间 (格式为 HHMMSSsss, 形如 141206000) */ 
        int32           ordCnfmTime; 
        
        /** 订单当前状态 @see eOesOrdStatusT */ 
        uint8           ordStatus; 
        /** 委托确认状态 (交易所返回的回报状态，仅供参考)  @see eOesOrdStatusT */ 
        uint8           ordCnfmSts; 
        /** 证券类型 @see eOesSecurityTypeT */ 
        uint8           securityType; 
        /** 证券子类型 @see eOesSubSecurityTypeT */ 
        uint8           subSecurityType; 
        /** 交易所订单编号 (深交所的订单编号是16位的非数字字符串) */ 
        char            exchOrdId[20]; 
        /** 委托当前冻结的交易金额 */ 
        int64           frzAmt; 
        /** 委托当前冻结的利息 */ 
        int64           frzInterest; 
        /** 委托当前冻结的交易费用 */ 
        int64           frzFee; 
        /** 委托累计已发生的交易金额 */ 
        int64           cumAmt; 
        /** 委托累计已发生的利息 */ 
        int64           cumInterest; 
        /** 委托累计已发生的交易费用 */ 
        int64           cumFee; 
        
        /** 累计执行数量 (累计成交数量) */ 
        int32           cumQty; 
        /** 已撤单数量 */ 
        int32           canceledQty; 
  
        /** 订单/撤单拒绝原因 */
        int32           ordRejReason;
        /** 交易所错误码 */
        int32           exchErrCode;
        /** PBU代码 (席位号) */
        int32           pbuId;
        /** 营业部代码 */
        int32           branchId;
};

/**
 * 成交回报结构体定义
 */
struct ZpquantTrdCnfm {
        /** 交易所成交编号 (以下的6个字段是成交信息的联合索引字段) */ 
        int64           exchTrdNum; 
        /** 市场代码 @see eOesMarketIdT */ 
        uint8           mktId; 
        /** 买卖类型 (取值范围: 买/卖, 申购/赎回(仅深圳)) @see eOesBuySellTypeT */ 
        uint8           trdSide; 
        /** 股东账户代码 */ 
        char            invAcctId[16]; 
        /** 产品代码 */ 
        char            securityId[16]; 

        /** 成交日期 (格式为 YYYYMMDD, 形如 20160830) */ 
        int32           trdDate; 
        /** 成交时间 (格式为 HHMMSSsss, 形如 141205000) */ 
        int32           trdTime; 
        /** 成交数量 */ 
        int32           trdQty; 
        /** 成交价格 (单位精确到元后四位, 即: 1元=10000) */ 
        int32           trdPrice; 
        /** 成交金额 (单位精确到元后四位, 即: 1元=10000) */ 
        int64           trdAmt; 
        
        /** 客户订单编号 */ 
        int64           clOrdId; 
        /** 累计执行数量 */ 
        int32           cumQty; 

        /** 原始委托数量 */ 
        int32           origOrdQty; 
        /** PBU代码 (席位号) */ 
        int32           pbuId; 
        /** 营业部代码 */ 
        int32           branchId
        /** 客户委托流水号 */ 
        int32           clSeqNo; 
        /** 客户端编号 */ 
        int16           clientId; 
        /** 客户端环境号 */ 
        int8            clEnvId; 
        /** 证券子类别 (为保持兼容而位置凌乱, 后续会做调整) @see eOesSubSecurityTypeT */ 
        uint8           subSecurityType; 
        
        /** 订单当前状态 @see eOesOrdStatusT */ 
        uint8           ordStatus; 
        /** 订单类型 @see eOesOrdTypeShT eOesOrdTypeSzT */ 
        uint8           ordType; 
        /** 买卖类型 @see eOesBuySellTypeT */ 
        uint8           ordBuySellType; 
        /** 证券类型 @see eOesSecurityTypeT */ 
        uint8           securityType; 
        /** 原始委托价格, 单位精确到元后四位, 即1元 = 10000 */ 
        int32           origOrdPrice; 
        
        /** 累计成交金额 */ 
        int64           cumAmt; 
        /** 累计成交利息 */ 
        int64           cumInterest; 
        /** 累计交易费用 */ 
        int64           cumFee; 
};


/**
 * 客户资金信息内容
 */
struct ZpquantCashAssetItem {
    /** 资金账户代码 */ 
    char                cashAcctId[16];
    /** 客户代码 */
    char                custId[16];
   
    /** 币种 @see eOesCurrTypeT */ 
    uint8               currType; 
    /** 资金帐户类别(冗余自资金账户) @see eOesCashTypeT */ 
    uint8               cashType; 
    /** 资金帐户状态(冗余自资金账户) @deprecated @see eOesAcctStatusT */ 
    uint8               cashAcctStatus; 
    /** 是否禁止出入金 (仅供API查询使用) */ 
    uint8               isFundTrsfDisabled; 
    /** 期初余额, 单位精确到元后四位, 即1元 = 10000 */
    int64               beginningBal;
    /** 期初可用余额, 单位精确到元后四位, 即1元 = 10000 */
    int64               beginningAvailableBal;
    /** 期初可取余额, 单位精确到元后四位, 即1元 = 10000 */
    int64               beginningDrawableBal;
    /** 不可用资金余额(既不可交易又不可提取), 单位精确到元后四位, 即1元 = 10000 */
    int64               disableBal;
    /** 当前冲正金额(红冲蓝补的资金净额), 取值可以为负数(表示资金调出), 单位精确到元后四位(即1元 = 10000) */
    int64               reversalAmt;
    /** 手动冻结资金, 取值在0和当前资产之间, 单位精确到元后四位(即1元 = 10000) */
    int64               manualFrzAmt;
   
    /** 日中累计存入资金金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               totalDepositAmt; 
    /** 日中累计提取资金金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               totalWithdrawAmt; 
    /** 当前提取冻结资金金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               withdrawFrzAmt; 
    
    /** 日中累计卖获得资金金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               totalSellAmt; 
    /** 日中累计 买/申购/逆回购 使用资金金额，单位精确到元后四位，即1元 = 10000 */ 
    int64               totalBuyAmt; 
    /** 当前交易冻结金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               buyFrzAmt; 
    
    /** 日中累计交易费用金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               totalFeeAmt; 
    /** 当前冻结交易费用金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               feeFrzAmt; 
    
    /** 当前维持的保证金金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               marginAmt; 
    /** 当前冻结的保证金金额, 单位精确到元后四位, 即1元 = 10000 */ 
    int64               marginFrzAmt;

    /**
     * 当前余额, 包括当前可用余额和在途冻结资金在內的汇总值
     * 可用余额请参考“当前可用余额(currentAvailableBal)”字段
     */
    int64               currentTotalBal;
    /** 当前可用余额 */
    int64               currentAvailableBal;
    /** 当前可取余额 */
    int64               currentDrawableBal;
};

/**
 * 股票持仓基础信息的内容定义
 */
#define __ZPQUANT_STK_HOLDING_BASE_INFO_PKT                 \
        /** 账户代码 */ \
        char                invAcctId[16]; \
        /** 产品代码 */ \
        char                securityId[16]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 按64位对齐的填充域 */ \
        uint8               __HOLD_BASE_filler[5]; \
        \
        /** 日初持仓 */ \
        int64               originalHld; \
        /** 日初总持仓成本 (日初持仓成本价 = 日初总持仓成本 / 日初持仓) */ \
        int64               originalCostAmt; \
        \
        /** 日中累计买入持仓 */ \
        int64               totalBuyHld; \
        /** 日中累计卖出持仓 */ \
        int64               totalSellHld; \
        /** 当前卖出冻结持仓 */ \
        int64               sellFrzHld; \
        /** 手动冻结持仓 */ \
        int64               manualFrzHld; \
        \
        /** 日中累计买入金额 */ \
        int64               totalBuyAmt; \
        /** 日中累计卖出金额 */ \
        int64               totalSellAmt; \
        /** 日中累计买入费用 */ \
        int64               totalBuyFee; \
        /** 日中累计卖出费用 */ \
        int64               totalSellFee; \
        \
        /**
         * 日中累计转换获得持仓, ETF申赎业务使用
         * - 成分股持仓场景, 转换获得指赎回时获得的成分股持仓;
         * - ETF证券持仓场景, 转换获得指申购时获得的ETF证券股持仓;
         */ \
        int64               totalTrsfInHld; \
        /**
         * 日中累计转换付出持仓, ETF申赎业务使用
         * - 成分股持仓场景, 转换付出指申购时使用的成分股持仓;
         * - ETF证券持仓场景, 转换付出指赎回时使用的ETF证券股持仓;
         */ \
        int64               totalTrsfOutHld; \
        /** 当前转换付出冻结持仓 */ \
        int64               trsfOutFrzHld; \
        \
        /** 当前已锁定持仓 */ \
        int64               lockHld; \
        /** 当前锁定冻结持仓数量 */ \
        int64               lockFrzHld; \
        /** 当前解锁定冻结持仓数量 */ \
        int64               unlockFrzHld; \
        \
        /** 当前备兑冻结的现货持仓数量 */ \
        int64               coveredFrzHld; \
        /** 当前已备兑使用的现货持仓数量 */ \
        int64               coveredHld

/**
 * 查询到的股票持仓信息内容
 */
struct ZpquantStkHoldingItem {
    __ZPQUANT_STK_HOLDING_BASE_INFO_PKT;

    /** 当前可卖持仓 */
    int64               sellAvlHld;
    /** 当前可转换付出持仓 */
    int64               trsfOutAvlHld;
    /** 当前可锁定持仓 */
    int64               lockAvlHld;
    /** 当前可用于备兑的现货持仓, 可解锁的锁定持仓数量与此相同 */
    int64               coveredAvlHld;
    /**
     * 总持仓, 包括当前可用持仓、不可交易持仓和在途冻结持仓在內的汇总值
     * 可卖持仓请参考“当前可卖持仓(sellAvlHld)”字段
     */
    int64               sumHld;
    /** 持仓成本价 */
    int64               costPrice;
};

/**
 * 出入金委托基础信息的内容定义
 */
#define __ZPQUANT_FUND_TRSF_BASE_INFO_PKT                   \
        /** 客户委托流水号 (由客户端维护的递增流水) */ \
        int32           clSeqNo; \
        /** 划转方向 @see eOesFundTrsfDirectT */ \
        uint8           direct; \
        /** 出入金转账类型 @see eOesFundTrsfTypeT*/ \
        uint8           isAllotOnly; \
        \
        /** 资金账户代码 */ \
        char            cashAcctId[16]; \
        /** 交易密码 */ \
        char            trdPasswd[40]; \
        /**
         * 转账密码(转账方向为转入(银行转证券), 此密码为银行密码.
         * 转账方向为转出(证券转银行), 此密码为资金密码
         */ \
        char            trsfPasswd[40]; \
        \
        /** 发生金额 (都是正数), 单位精确到元后四位, 即1元 = 10000 */ \
        int64           occurAmt

/**
 * 出入金拒绝的回报结构定义 (因风控检查未通过而被OES拒绝)
 */
struct ZpquantFundTrsfReject {
    __ZPQUANT_FUND_TRSF_BASE_INFO_PKT;

    /** 委托日期 (格式为 YYYYMMDD, 形如 20160830) */
    int32               ordDate;
    /** 委托时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               ordTime;

    /** 客户端编号 */
    int16               clientId;
    /** 客户端环境号 */
    int8                clEnvId;
    /** 64位对齐的填充域 */
    int8                __filler;
    /** 错误码 */
    int32               rejReason;

    /** 错误信息 */
    char                errorInfo[64];
};

/**
 * 出入金委托执行状态回报的结构体定义
 */
struct ZpquantFundTrsfReport {
    /** 客户委托流水号 (由客户端维护的递增流水) */
    int32               clSeqNo;
    /** 客户端编号 */
    int16               clientId;
    /** 客户端环境号 */
    int8                clEnvId;
    /** 划转方向 @see eOesFundTrsfDirectT */
    uint8               direct;
    /** 资金账户代码 */
    char                cashAcctId[16];

    /** 发生金额 (都是正数), 单位精确到元后四位, 即1元 = 10000 */
    int64               occurAmt;

    /** OES出入金委托编号 (在OES内具有唯一性的内部出入金委托编号) */
    int32               fundTrsfId;
    /** 柜台出入金委托编号 */
    int32               counterEntrustNo;

    /** 出入金委托日期 (格式为 YYYYMMDD, 形如 20160830) */
    int32               operDate;
    /** 出入金委托时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               operTime;
    /** 上报柜台时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               dclrTime;
    /** 柜台执行结果采集时间 (格式为 HHMMSSsss, 形如 141205000) */
    int32               doneTime;

    /** 出入金转账类型 @see eOesFundTrsfTypeT*/
    uint8               isAllotOnly;
    /** 出入金委托执行状态 @see eOesFundTrsfStatusT */
    uint8               trsfStatus;
    /** 是否有转账到主柜 */
    uint8               __hasCounterTransfered;
    /** 按64位对齐填充域 */
    uint8               __filler;

    /** 错误原因 */
    int32               rejReason;
    /** 主柜错误码 */
    int32               counterErrCode;
    /** 按64位对齐填充域 */
    uint32              __filler2;
    /** 资金调拨流水号 */
    char                allotSerialNo[64];
    /** 错误信息 */
    char                errorInfo[64];
};

/**
 * 市场状态信息的结构体定义
 */
struct ZpquantMarketStateInfo {
    uint8               exchId;             /**< 交易所代码 @see eOesExchangeIdT */
    uint8               platformId;         /**< 交易平台类型 @see eOesPlatformIdT */
    uint8               mktId;              /**< 市场代码 @see eOesMarketIdT */
    uint8               mktState;           /**< 市场状态 @see eOesMarketStatusT */
    uint8               __filler[4];        /**< 按64位对齐的填充域 */
};

/**
 * 查询定位的游标结构
 */
struct ZpquantQryCursor {
    /** 查询位置 */
    int32               seqNo;

    /** 是否是当前最后一个包 */
    int8                isEnd;
    /** 按64位对齐填充域 */
    int8                __filler[3];

    /** 用户私有信息 (由客户端自定义填充, 并在应答数据中原样返回) */
    int64               userInfo;
};

/**
 * 客户基础信息的内容定义
 */
#define __ZPQUANT_CUST_BASE_INFO_PKT                        \
        /** 客户代码 */ \
        char                custId[16]; \
        \
        /** 客户类型 */ \
        uint8               custType; \
        /** 客户状态 0 正常, 非0 不正常 */ \
        uint8               status; \
        /** OES风险等级 @see eOesSecurityRiskLevelT */ \
        uint8               riskLevel; \
        /** 客户原始风险等级 */ \
        uint8               originRiskLevel; \
        /** 机构标志 (TRUE 机构投资者, FALSE 个人投资者) */ \
        uint8               institutionFlag; \
        /** 投资者分类 @see eOesInvestorClassT */ \
        uint8               investorClass; \
        /** 按64位对齐填充域 */ \
        uint8               __CUST_BASE_filler1[2]; \
        \
        /** 营业部代码 */ \
        int32               branchId; \
        /** 按64位对齐填充域 */ \
        uint32              __CUST_BASE_filler2

/**
 * 客户基础信息的结构体定义
 */
struct ZpquantCustBaseInfo {
    __ZPQUANT_CUST_BASE_INFO_PKT;
};

/**
 * 证券账户基础信息的内容定义
 */
#define __ZPQUANT_INV_ACCT_BASE_INFO_PKT                   \
        /** 股东账户代码 */ \
        char                invAcctId[16]; \
        /** 市场 @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** 账户类型 @see eOesAcctTypeT */ \
        uint8               acctType; \
        /** 账户状态 @see eOesAcctStatusT */ \
        uint8               status; \
        /** 股东账户的所有者类型 @see eOesOwnerTypeT */ \
        uint8               ownerType; \
        /** 期权投资者级别 @see eOesOptInvLevelT */ \
        uint8               optInvLevel; \
        /** 是否禁止交易 (仅供API查询使用) */ \
        uint8               isTradeDisabled; \
        /** 按64位对齐填充域 */ \
        uint8               __INV_ACCT_BASE_filler[2]; \
        \
        /** 证券账户权限限制 @see eOesTradingLimitT */ \
        uint64              limits; \
        /** 股东权限/客户权限 @see eOesTradingPermissionT */ \
        uint64              permissions; \
        \
        /** 席位号 */ \
        int32               pbuId; \
        /** 新股认购限额 */ \
        int32               subscriptionQuota

/**
 * 证券账户内容
 */
struct ZpquantInvAcctItem {
    __ZPQUANT_INV_ACCT_BASE_INFO_PKT;

    /** 客户代码 */
    char                custId[16];
};

/**
 * 竞价的限价参数配置
 */
typedef struct _ZpquantPriceLimit {
    /** 上涨限价, 单位精确到元后四位, 即1元 = 10000 */
    int32               ceilPrice;
    /** 下跌限价, 单位精确到元后四位, 即1元 = 10000 */
    int32               floorPrice;
} ZpquantPriceLimitT;

/**
 * 现货产品基础信息的内容定义
 */
#define __ZPQUANT_STOCK_BASE_INFO_PKT                       \
        /** 产品代码 */ \
        char                securityId[16]; \
        /** 市场代码 @see eOesMarketIdT */ \
        uint8               mktId; \
        \
        /** 证券类型 @see eOesSecurityTypeT */ \
        uint8               securityType; \
        /** 证券子类型 @see eOesSubSecurityTypeT */ \
        uint8               subSecurityType; \
        /** 产品级别 @see eOesSecurityLevelT */ \
        uint8               securityLevel; \
        /** 产品风险等级 @see eOesSecurityRiskLevelT */ \
        uint8               securityRiskLevel; \
        /** 币种 @see eOesCurrTypeT */ \
        uint8               currType; \
        /** 投资者适当性管理分类 @see eOesQualificationClassT */ \
        uint8               qualificationClass; \
        /** 是否支持当日回转交易 0: 不支持; 其他: 支持 */ \
        uint8               isDayTrading; \
        \
        /** 连续停牌标识 @see eOesSecuritySuspFlagT */ \
        uint8               suspFlag; \
        /** 临时停牌标识 (TRUE 已停牌, FALSE 未停牌) */ \
        uint8               temporarySuspFlag; \
        /** 填充字符  满足64位对齐 */ \
        uint8               __filter[6]; \
        \
        /** 买入单位 */ \
        int32               buyQtyUnit; \
        /** 卖出单位 */ \
        int32               sellQtyUnit; \
        /** 单笔买委托最大份数 */ \
        int32               buyOrdMaxQty; \
        /** 单笔买委托最小份数 */ \
        int32               buyOrdMinQty; \
        /** 单笔卖委托最大份数 */ \
        int32               sellOrdMaxQty; \
        /** 单笔卖委托最小份数 */ \
        int32               sellOrdMinQty; \
        \
        /** 最小价格变动单位, 单位精确到元后四位, 即1元 = 10000 */ \
        int32               priceUnit; \
        /** 昨日收盘价，单位精确到元后四位，即1元 = 10000 */ \
        int32               prevClose; \
        /** 面值, 单位精确到元后四位, 即1元 = 10000 */ \
        int64               parPrice; \
        /** 债券的每百元应计利息额, 单位精确到元后八位 */ \
        int64               bondInterest; \
        \
        /** 逆回购期限 */ \
        int32               repoExpirationDays; \
        /** 占款天数 */ \
        int32               cashHoldDays; \
        \
        /** 竞价限价参数表, 数组下标为当前时段标志 @see eOesTrdSessTypeT */ \
        ZpquantPriceLimitT      priceLimit[3]; \
        \
        /** 产品名称 */ \
        char                securityName[24]; \
        /** ETF申购赎代码 */ \
        char                fundId[16]

/**
 * 现货产品基础信息的结构体定义
 */
struct ZpquantStockBaseInfo {
    __ZPQUANT_STOCK_BASE_INFO_PKT;
};



/**
 * 查询到的委托信息内容
 */
typedef ZpquantOrdCnfm     ZpquantOrdItem;
/**
 * 查询到的成交信息内容
 */
typedef ZpquantTrdCnfm     ZpquantTrdItem;
/**
 * 客户信息内容
 */
typedef ZpquantCustBaseInfo    ZpquantCustItemT;
/**
 * 查询出入金流水信息应答
 */
typedef ZpquantFundTrsfReport      ZpquantFundTransferSerialItemT;
/**
 * 现货产品信息内容
 */
typedef OesStockBaseInfoT       OesStockItemT;


