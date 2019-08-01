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
 ******************************************************
 * 通信消息的消息类型定义 -- 行情类
 ******************************************************
**/

/**
 * 订阅模式 (SubMode) 定义
 * - 0: (Set) 重新订阅, 设置为订阅列表中的股票
 * - 1: (Append) 追加订阅, 增加订阅列表中的股票
 * - 2: (Delete) 删除订阅, 删除订阅列表中的股票
 */
typedef enum _zpquantMdsSubscribeMode {
    /** 重新订阅, 设置为订阅列表中的股票 */
    MDS_SUB_MODE_SET                            = 0,

    /** 追加订阅, 增加订阅列表中的股票 */
    MDS_SUB_MODE_APPEND                         = 1,

    /** 删除订阅, 删除订阅列表中的股票 */
    MDS_SUB_MODE_DELETE                         = 2,

    __MAX_MDS_SUB_MODE
} ZpquantMdsSubscribeMode;

/**
 * 可订阅的数据种类 (DataType) 定义
 * - 0:      默认数据种类 (所有)
 * - 0x0001: L1快照/指数/期权
 * - 0x0002: L2快照
 * - 0x0004: L2委托队列
 * - 0x0008: L2逐笔成交
 * - 0x0010: L2逐笔委托 (深圳)
 * - 0x0020: L2虚拟集合竞价 (上海)
 * - 0x0040: L2市场总览 (上海)
 * - 0x0100: 市场状态 (上海)
 * - 0x0200: 证券实时状态 (深圳)
 * - 0x0400: 指数行情 (与0x0001的区别在于, 0x0400可以单独订阅指数行情)
 * - 0x0800: 期权行情 (与0x0001的区别在于, 0x0800可以单独订阅期权行情)
 * - 0xFFFF: 所有数据种类
 */
typedef enum _zpquantMdsSubscribeDataType {
    /** 默认数据种类 (所有种类) */
    MDS_SUB_DATA_TYPE_DEFAULT                   = 0,

    /** L1快照/指数/期权 (L1快照行情 + 指数行情 + 期权行情) */
    MDS_SUB_DATA_TYPE_L1_SNAPSHOT               = 0x0001,

    /** L2快照 */
    MDS_SUB_DATA_TYPE_L2_SNAPSHOT               = 0x0002,

    /** L2委托队列 */
    MDS_SUB_DATA_TYPE_L2_BEST_ORDERS            = 0x0004,

    /** L2逐笔成交 */
    MDS_SUB_DATA_TYPE_L2_TRADE                  = 0x0008,

    /** L2逐笔委托 (*深圳, 0x10/16) */
    MDS_SUB_DATA_TYPE_L2_ORDER                  = 0x0010,

    /** L2虚拟集合竞价 (*上海, 0x20/32) */
    MDS_SUB_DATA_TYPE_L2_VIRTUAL_AUCTION_PRICE  = 0x0020,

    /** L2市场总览 (*上海, 0x40/64) */
    MDS_SUB_DATA_TYPE_L2_MARKET_OVERVIEW        = 0x0040,

    /** 市场状态 (*上海, 0x100/256) */
    MDS_SUB_DATA_TYPE_TRADING_SESSION_STATUS    = 0x0100,

    /** 证券实时状态 (*深圳, 0x200/512) */
    MDS_SUB_DATA_TYPE_SECURITY_STATUS           = 0x0200,

    /** 指数行情 (与L1_SNAPSHOT的区别在于, INDEX_SNAPSHOT可以单独订阅指数行情) */
    MDS_SUB_DATA_TYPE_INDEX_SNAPSHOT            = 0x400,

    /** 期权行情 (与L1_SNAPSHOT的区别在于, OPTION_SNAPSHOT可以单独订阅期权行情) */
    MDS_SUB_DATA_TYPE_OPTION_SNAPSHOT           = 0x800,

    /** 空数据种类 (可用于不指定任何数量种类的情况) */
    MDS_SUB_DATA_TYPE_NONE                      = 0x8000,

    /** 所有数据种类 */
    MDS_SUB_DATA_TYPE_ALL                       = 0xFFFF,

    __MAX_MDS_SUB_DATA_TYPE                     = 0x7FFFFFFF
} ZpquantMdsSubscribeDataType;

typedef enum _ZpquantMdsMsgType {
    /*
     * 会话类消息
     */
    /** 心跳消息 (1/0x01) */
    MDS_MSGTYPE_HEARTBEAT                           = 1,
    /** 测试请求消息 (2/0x02) */
    MDS_MSGTYPE_TEST_REQUEST                        = 2,
    /** 登录消息 (3/0x03) */
    MDS_MSGTYPE_LOGON                               = 3,
    /** 注销消息 (4/0x04) */
    MDS_MSGTYPE_LOGOUT                              = 4,
    /** 证券行情订阅消息 (5/0x05) */
    MDS_MSGTYPE_MARKET_DATA_REQUEST                 = 5,
    /** 压缩的数据包 (6/0x06, 内部使用) */
    MDS_MSGTYPE_COMPRESSED_PACKETS                  = 6,
    /** 最大的会话消息类型 */
    __MDS_MSGTYPE_SESSION_MAX,

    /*
     * Level1 行情消息
     */
    /** Level1 市场行情快照 (10/0x0A) */
    MDS_MSGTYPE_MARKET_DATA_SNAPSHOT_FULL_REFRESH   = 10,
    /** Level1/Level2 指数行情快照 (11/0x0B) */
    MDS_MSGTYPE_INDEX_SNAPSHOT_FULL_REFRESH         = 11,
    /** Level1/Level2 期权行情快照 (12/0x0C) */
    MDS_MSGTYPE_OPTION_SNAPSHOT_FULL_REFRESH        = 12,

    /** 市场状态消息 (13/0x0D, 仅上海) */
    MDS_MSGTYPE_TRADING_SESSION_STATUS              = 13,
    /** 证券状态消息 (14/0x0E, 仅深圳) */
    MDS_MSGTYPE_SECURITY_STATUS                     = 14,
    /** 最大的Level-1行情消息类型 */
    __MDS_MSGTYPE_L1_MAX,

    /*
     * Level2 行情消息
     */
    /** Level2 市场行情快照 (20/0x14) */
    MDS_MSGTYPE_L2_MARKET_DATA_SNAPSHOT             = 20,
    /** Level2 委托队列快照 (买一/卖一前五十笔) (21/0x15) */
    MDS_MSGTYPE_L2_BEST_ORDERS_SNAPSHOT             = 21,

    /** Level2 逐笔成交行情 (22/0x16) */
    MDS_MSGTYPE_L2_TRADE                            = 22,
    /** Level2 逐笔委托行情 (23/0x17, 仅深圳) */
    MDS_MSGTYPE_L2_ORDER                            = 23,

    /** Level2 快照行情的增量更新消息 (24/0x18, 仅上海) */
    MDS_MSGTYPE_L2_MARKET_DATA_INCREMENTAL          = 24,
    /** Level2 委托队列快照的增量更新消息 (25/0x19, 仅上海) */
    MDS_MSGTYPE_L2_BEST_ORDERS_INCREMENTAL          = 25,

    /** Level2 市场总览消息 (26/0x1A, 仅上海) */
    MDS_MSGTYPE_L2_MARKET_OVERVIEW                  = 26,
    /** Level2 虚拟集合竞价消息 (27/0x1B, 仅上海) */
    MDS_MSGTYPE_L2_VIRTUAL_AUCTION_PRICE            = 27,
    /** 最大的Level-2行情消息类型 */
    __MDS_MSGTYPE_L2_MAX,

    /*
     * 查询类消息
     */
    /** 查询证券行情 (80/0x50) */
    MDS_MSGTYPE_QRY_MARKET_DATA_SNAPSHOT            = 80,
    /** 查询(深圳)证券状态 (81/0x51) */
    MDS_MSGTYPE_QRY_SECURITY_STATUS                 = 81,
    /** 查询(上证)市场状态 (82/0x52) */
    MDS_MSGTYPE_QRY_TRADING_SESSION_STATUS          = 82,
    /** 最大的查询消息类型 */
    __MDS_MSGTYPE_QRY_MAX

} ZpquantMdsMsgTypeT;

typedef enum _ZpquantTdResponMsgType {
 
    /* 查询到持仓信息回调 */
    TD_MSGTYPE_STKHOLDING_RESPON                    = 100,


} ZpquantTdResponMsgTypeT;

/**
 * 行情订阅请求的应答报文
 */
typedef struct _MdsMktDataRequestRsp {
    /**
     * 订阅模式
     * - 0: (Set) 重新订阅, 设置为订阅列表中的股票
     * - 1: (Append) 追加订阅, 增加订阅列表中的股票
     * - 2: (Delete) 删除订阅, 删除订阅列表中的股票
     *
     * @see eMdsSubscribeModeT
     */
    uint8               subMode;

    /**
     * 数据模式, 订阅最新的行情快照还是所有时点的数据
     * -  0: (LatestSimplified) 只订阅最新的行情快照数据, 并忽略和跳过已经过时的数据
     *       - 该模式推送的数据量最小, 没有重复数据, 也不会重复发送最新快照
     *       - 该模式在时延和带宽方面都相对优秀, 如果没有特殊需求, 推荐使用该模式即可
     * -  1: (LatestTimely) 只订阅最新的行情快照数据, 并立即发送最新数据
     *       - 只要有行情更新事件, 便立即推送该产品的最新行情, 但也会因此重复发送多次相同的最新行情
     *       - 如果某些产品的交易很活跃, 而客户端处理又比较耗时的话, 那么该模式可能会更及时的获取到
     *         这些产品的最新行情
     *       - 此外, 因为与 AllIncrements 模式下的数据一一对应, 可以方便与增量更新消息进行比对测试
     *       - 通常情况下, 推荐使用 LatestSimplified 模式即可
     * -  2: (AllIncrements) 订阅所有时点的行情快照数据 (包括Level2增量更新消息)
     *       - 该模式会推送所有时点的行情数据, 包括Level2行情快照的增量更新消息
     *       - 如果需要获取全量的行情明细, 或者需要直接使用Level2的增量更新消息, 可以使用该模式
     *
     * @see eMdsSubscribedTickTypeT
     */
    uint8               tickType;

    /**
     * 在推送实时行情数据之前, 是否需要推送已订阅产品的初始的行情快照
     * 该参数只在初始订阅时有效, 会话过程中的实时订阅将不支持该参数, 也不会再次推送初始行情快照
     */
    uint8               isRequireInitialMktData;

    /** 订阅的内部频道号 (供内部使用, 尚未对外开放) */
    uint8               __channelNos;

    /**
     * 逐笔数据的过期时间类型
     * -  0: 不过期
     * -  1: 立即过期 (1秒, 若落后于快照1秒则视为过期)
     * -  2: 及时过期 (3秒)
     * -  3: 超时过期 (30秒)
     *
     * @see     eMdsSubscribedTickExpireTypeT
     * @note    因为存在不可控的网络因素, 所以做不到百分百的精确过滤, 如果对数据的
     *          时效性有精确要求, 就需要在前端对数据再进行一次过滤
     */
    uint8               tickExpireType;

    /** 按64位对齐的填充域 */
    uint8               __filler[3];

    /**
     * 订阅的数据种类
     * - 0:      默认数据种类 (所有)
     * - 0x0001: L1快照/指数/期权
     * - 0x0002: L2快照
     * - 0x0004: L2委托队列
     * - 0x0008: L2逐笔成交
     * - 0x0010: L2逐笔委托 (深圳)
     * - 0x0020: L2虚拟集合竞价 (上海)
     * - 0x0040: L2市场总览 (上海)
     * - 0x0100: 市场状态 (上海)
     * - 0x0200: 证券实时状态 (深圳)
     * - 0x0400: 指数行情 (与0x0001的区别在于, 0x0400可以单独订阅指数行情)
     * - 0x0800: 期权行情 (与0x0001的区别在于, 0x0800可以单独订阅期权行情)
     * - 0xFFFF: 所有数据
     *
     * @see eMdsSubscribeDataTypeT
     */
    int32               dataTypes;

    /**
     * 请求订阅的行情数据的起始时间 (格式为: HHMMSS 或 HHMMSSsss)
     * - -1: 从头开始获取
     * -  0: 从最新位置开始获取实时行情
     * - >0: 从指定的起始时间开始获取 (HHMMSS / HHMMSSsss)
     * - 对于应答数据, 若为 0 则表示当前没有比起始时间更加新的行情数据
     */
    int32               beginTime;

    /**
     * 上证股票(债券/基金)产品的订阅结果 (实际已订阅的产品数量)
     * - -1: 订阅了所有产品;
     * -  0: 未订阅或已禁用;
     * - >0: 已订阅的产品数量(按照订阅列表成功订阅的产品数量)
     */
    int32               sseStockSubscribed;

    /**
     * 上证指数产品的订阅结果 (实际已订阅的产品数量)
     * - -1: 订阅了所有产品;
     * -  0: 未订阅或已禁用;
     * - >0: 已订阅的产品数量(按照订阅列表成功订阅的产品数量)
     */
    int32               sseIndexSubscribed;

    /**
     * 上证期权产品的订阅结果 (实际已订阅的产品数量)
     * - -1: 订阅了所有产品;
     * -  0: 未订阅或已禁用;
     * - >0: 已订阅的产品数量(按照订阅列表成功订阅的产品数量)
     */
    int32               sseOptionSubscribed;

    /**
     * 深圳股票(债券/基金)产品的订阅结果 (实际已订阅的产品数量)
     * - -1: 订阅了所有产品;
     * -  0: 未订阅或已禁用;
     * - >0: 已订阅的产品数量(按照订阅列表成功订阅的产品数量)
     */
    int32               szseStockSubscribed;

    /**
     * 深圳指数产品的订阅结果 (实际已订阅的产品数量)
     * - -1: 订阅了所有产品;
     * -  0: 未订阅或已禁用;
     * - >0: 已订阅的产品数量(按照订阅列表成功订阅的产品数量)
     */
    int32               szseIndexSubscribed;

    /**
     * 深圳期权产品的订阅结果 (实际已订阅的产品数量)
     * - -1: 订阅了所有产品;
     * -  0: 未订阅或已禁用;
     * - >0: 已订阅的产品数量(按照订阅列表成功订阅的产品数量)
     */
    int32               szseOptionSubscribed;

} MdsMktDataRequestRspT;

/**
 * 测试请求的应答报文
 */
typedef struct _MdsTestRequestRsp {
    /** 测试请求标识符 */
    char                testReqId[32];

    /** 测试请求的原始发送时间 (timeval结构或形如'YYYYMMDD-HH:mm:SS.sss'的字符串) */
    char                origSendTime[22];

    /** 按64位对齐的填充域 */
    char                __filler1[2];

    /** 测试请求应答的发送时间 (timeval结构或形如'YYYYMMDD-HH:mm:SS.sss'的字符串) */
    char                respTime[22];

    /** 按64位对齐的填充域 */
    char                __filler2[2];

} MdsTestRequestRspT;

/**
 * 登录请求的应答报文
 */
typedef struct _MdsLogonRsp {
    /** 加密方法 */
    int32               encryptMethod;
    /** 心跳间隔, 单位为秒 */
    int32               heartBtInt;

    /** 用户名 */
    char                username[40];
    /** 服务器端采用的协议版本号 */
    char                applVerId[32];
    /** 服务器端兼容的最低协议版本号 */
    char                minVerId[32];

    uint8               hostNum;                /**< 服务端主机编号 */
    uint8               isLeader;               /**< 是否是'主节点' */
    uint8               leaderHostNum;          /**< '主节点'的主机编号 */
    uint8               __filler[5];            /**< 按64位对齐的填充域 */
} MdsLogonRspT;

/**
 * Level2 逐笔成交行情定义
 */
typedef struct _MdsL2Trade {
    uint8               exchId;                 /**< 交易所代码(沪/深) @see eMdsExchangeIdT */
    uint8               securityType;           /**< 证券类型(股票/期权) @see eMdsSecurityTypeT */
    int8                __isRepeated;           /**< 是否是重复的行情 (内部使用, 小于0表示数据倒流) */
    uint8               __filler1;              /**< 按64位对齐的填充域 */

    int32               tradeDate;              /**< 交易日期 (YYYYMMDD, 非官方数据) */
    int32               TransactTime;           /**< 成交时间 (HHMMSSsss) */

    int32               instrId;                /**< 产品代码 */
    int32               ChannelNo;              /**< 成交通道/频道代码 [0..9999] */
    int32               ApplSeqNum;             /**< 成交序号/消息记录号 (从1开始, 按频道连续) */

    /** 产品代码 C6 / C8 (如: '600000' 等) */
    char                SecurityID[9];

    /**
     * 成交类别 (仅适用于深交所, '4'=撤销, 'F'=成交)
     * 对于上证, 将固定设置为 'F'(成交)
     * @see eMdsL2TradeExecTypeT
     */
    char                ExecType;

    /**
     * 内外盘标志 (仅适用于上证, 'B'=外盘,主动买, 'S'=内盘,主动卖, 'N'=未知)
     * 对于深交所, 将固定设置为 'N'(未知)
     * @see eMdsL2TradeBSFlagT
     */
    char                TradeBSFlag;

    uint8               __filler3[4];           /**< 按64位对齐的填充域 */
    uint8               __channelNo;            /**< 内部频道号 (供内部使用, 取值范围{1,2,4,8}) */
    uint64              __origTickSeq;          /**< 对应的原始行情的序列号 (内部使用) */

    int32               TradePrice;             /**< 成交价格 (价格单位精确到元后四位, 即: 1元=10000) */
    int32               TradeQty;               /**< 成交数量 (上海债券的数量单位为: 手) */
    int64               TradeMoney;             /**< 成交金额 (金额单位精确到元后四位, 即: 1元=10000) */

    int64               BidApplSeqNum;          /**< 买方订单号 (从 1 开始计数, 0 表示无对应委托) */
    int64               OfferApplSeqNum;        /**< 卖方订单号 (从 1 开始计数, 0 表示无对应委托) */

} MdsL2TradeT;

/**
 * Level2 逐笔委托行情定义 (仅适用于深交所)
 */
typedef struct _MdsL2Order {
    uint8               exchId;                 /**< 交易所代码(沪/深) @see eMdsExchangeIdT */
    uint8               securityType;           /**< 证券类型(股票/期权) @see eMdsSecurityTypeT */
    int8                __isRepeated;           /**< 是否是重复的行情 (内部使用, 小于0表示数据倒流) */
    uint8               __filler1;              /**< 按64位对齐的填充域 */

    int32               tradeDate;              /**< 交易日期 YYYYMMDD (自然日) */
    int32               TransactTime;           /**< 委托时间 HHMMSSsss */

    int32               instrId;                /**< 产品代码 */
    int32               ChannelNo;              /**< 频道代码 [0..9999] */
    int32               ApplSeqNum;             /**< 委托序号 (从1开始, 按频道连续) */

    /** 产品代码 C6 / C8 (如: '000001' 等) */
    char                SecurityID[9];

    /** 买卖方向 ('1'=买 '2'=卖 'G'=借入 'F'=出借) */
    char                Side;

    /** 订单类型 ('1'=市价 '2'=限价 'U'=本方最优) */
    char                OrderType;

    uint8               __filler3[4];           /**< 按64位对齐的填充域 */
    uint8               __channelNo;            /**< 内部频道号 (供内部使用, 取值范围{1,2,4,8}) */
    uint64              __origTickSeq;          /**< 对应的原始行情的序列号 (内部使用) */

    int32               Price;                  /**< 委托价格 (价格单位精确到元后四位, 即: 1元=10000) */
    int32               OrderQty;               /**< 委托数量 */
} MdsL2OrderT;

/**
 * 市场状态消息(MsgType=h)定义 (仅适用于上海市场, 深圳市场没有该行情)
 */
typedef struct _MdsTradingSessionStatusMsg {
    uint8               exchId;                 /**< 交易所代码(沪/深) @see eMdsExchangeIdT */
    uint8               securityType;           /**< 证券类型(股票/期权) @see eMdsSecurityTypeT */
    int8                __isRepeated;           /**< 是否是重复的行情 (供内部使用, 小于0 表示数据倒流) */
    uint8               __filler1;              /**< 按64位对齐的填充域 */

    int32               tradeDate;              /**< 交易日期 (YYYYMMDD, 通过拆解SendingTime得到, 并非官方数据) */
    int32               updateTime;             /**< 行情时间 (HHMMSSsss, 交易所时间, 通过拆解SendingTime得到, 并非官方数据) */
    int32               __exchSendingTime;      /**< 交易所发送时间 (HHMMSSsss) */
    int32               __mdsRecvTime;          /**< MDS接收到时间 (HHMMSSsss) */

    int32               TotNoRelatedSym;        /**< 最大产品数目 (包括指数) */

    /**
     * 全市场行情状态 (*C8)
     * 该字段为 8 位字符串,左起每位表示特定的含义,无定义则填空格。
     * 第 1 位: ‘S’表示全市场启动期间(开市前), ‘T’表示全市场处于交易期间 (含中间休市), ‘E’表示全市场处于闭市期间。
     * 第 2 位: ‘1’表示开盘集合竞价结束标志, 未结束取‘0’。
     * 第 3 位: ‘1’表示市场行情闭市标志, 未闭市取‘0’。
     */
    char                TradingSessionID[9];

    uint8               __filler3[3];           /**< 按64位对齐的填充域 */
    uint32              __dataVersion;          /**< 行情数据的更新版本号 (当__isRepeated!=0时, 该值仅作为参考值) */
    uint64              __origTickSeq;          /**< 对应的原始行情的序列号(供内部使用) */

} MdsTradingSessionStatusMsgT;

/**
 * 证券实时状态定义 (仅适用于深圳市场, 上海市场没有该行情)
 */
typedef struct _MdsSecurityStatusMsg {
    uint8               exchId;                 /**< 交易所代码(沪/深) @see eMdsExchangeIdT */
    uint8               securityType;           /**< 证券类型(股票/期权) @see eMdsSecurityTypeT */
    int8                __isRepeated;           /**< 是否是重复的行情 (供内部使用, 小于0 表示数据倒流) */
    uint8               __filler1;              /**< 按64位对齐的填充域 */

    int32               tradeDate;              /**< 交易日期 (YYYYMMDD, 通过拆解数据生成时间OrigTime得到) */
    int32               updateTime;             /**< 行情时间 (HHMMSSsss, 交易所时间, 通过拆解数据生成时间OrigTime得到) */
    int32               __exchSendingTime;      /**< 交易所发送时间 (HHMMSSsss, 目前获取不到深交所的发送时间, 固定为 0) */
    int32               __mdsRecvTime;          /**< MDS接收到时间 (HHMMSSsss) */

    int32               instrId;                /**< 产品代码 */

    /** 产品代码 C6 / C8 (如: '000001' 等) */
    char                SecurityID[9];

    /**
     * 证券状态 (C8)
     * A=上市公司早间披露提示
     * B=上市公司午间披露提示
     */
    char                FinancialStatus[9];

    uint8               __filler2;              /**< 按64位对齐的填充域 */
    uint8               __channelNo;            /**< 内部频道号 (供内部使用, 取值范围{1,2,4,8}) */
    uint32              __dataVersion;          /**< 行情数据的更新版本号 (当__isRepeated!=0时, 该值仅作为参考值) */
    uint64              __origTickSeq;          /**< 对应的原始行情的序列号(供内部使用) */

    int32               NoSwitch;               /**< 开关个数 */
    int32               __filler4;              /**< 按64位对齐的填充域 */

    /**
     * 证券业务开关列表
     * 业务开关列表为定长数组, 数组的下标位置对应于各个业务开关, 业务开关说明如下:
     *  -  1: 融资买入, 适用于融资标的证券
     *  -  2: 融券卖出, 适用于融券标的证券
     *  -  3: 申购, 适用于 ETF/LOF 等开放式基金, 对于黄金 ETF 是指现金申购
     *  -  4: 赎回, 适用于 ETF/LOF 等开放式基金, 对于黄金 ETF 是指现金赎回开关
     *  -  5: 认购, 适用于网上发行认购代码
     *  -  6: 转股, 适用于处于转股回售期的可转债
     *  -  7: 回售, 适用于处于转股回售期的可转债
     *  -  8: 行权, 适用于处于行权期的权证或期权
     *  - 10: 买开仓, 适用于期权等衍生品
     *  - 11: 卖开仓, 适用于期权等衍生品
     *  - 12: 黄金ETF实物申购, 适用于黄金 ETF
     *  - 13: 黄金ETF实物赎回, 适用于黄金 ETF
     *  - 14: 预受要约, 适用于处于要约收购期的股票
     *  - 15: 解除要约, 适用于处于要约收购期的股票
     *  - 18: 转股撤单, 适用于处于转股回售期的可转债
     *  - 19: 回售撤单, 适用于处于转股回售期的可转债
     *  - 20: 质押, 适用于质押式回购可质押入库证券
     *  - 21: 解押, 适用于质押式回购可质押入库证券
     *  - 22: 表决权, 适用于优先股
     *  - 23: 股票质押式回购, 适用于可开展股票质押式回购业务的证券
     *  - 24: 实时分拆, 适用于分级基金
     *  - 25: 实时合并, 适用于分级基金
     *  - 26: 备兑开仓, 适用于期权等衍生品
     *  - 27: 做市商报价, 适用于期权等支持做市商报价的证券
     */
    struct {
        /** 业务开关的使能标志 (0 未启用, 1 启用) */
        uint8           switchFlag;

        /** 开关状态 (0 关闭, 1 开启) */
        uint8           switchStatus;
    } switches[32];

} MdsSecurityStatusMsgT;

/**
 * Level1/Level2 快照行情(证券行情全幅消息)的消息头定义
 */
typedef struct _MdsMktDataSnapshotHead {
    uint8               exchId;                 /**< 交易所代码(沪/深) @see eMdsExchangeIdT */
    uint8               securityType;           /**< 证券类型(股票/期权) @see eMdsSecurityTypeT */
    int8                __isRepeated;           /**< 是否是重复的行情 (内部使用, 小于0表示数据倒流) */
    uint8               __origMdSource;         /**< 原始行情数据来源 @see eMdsMsgSourceT */

    int32               tradeDate;              /**< 交易日期 (YYYYMMDD, 8位整型数值) */
    int32               updateTime;             /**< 行情时间 (HHMMSSsss, 交易所时间, 只有上海L1可能会通过拆解SendingTime得到) */

    int32               instrId;                /**< 产品代码 */
    int16               bodyLength;             /**< 实际数据长度 */
    uint8               mdStreamType;           /**< 行情数据类型 @see eMdsMdStreamTypeT */
    uint8               __channelNo;            /**< 内部频道号 (供内部使用, 取值范围{1,2,4,8}) */
    uint32              __dataVersion;          /**< 行情数据的更新版本号 */
    uint64              __origTickSeq;          /**< 对应的原始行情的序列号(供内部使用) */

} MdsMktDataSnapshotHeadT;

/**
 * Level2 委托队列信息 (买一／卖一前五十笔委托明细)
 */
typedef struct _MdsL2BestOrdersSnapshotBody {
    /** 产品代码 C6 / C8 (如: '600000' 等) */
    char                SecurityID[9];
    uint8               __filler[5];            /**< 按64位对齐的填充域 */
    uint8               NoBidOrders;            /**< 买一价位的揭示委托笔数 */
    uint8               NoOfferOrders;          /**< 卖一价位的揭示委托笔数 */

    uint64              TotalVolumeTraded;      /**< 成交总量 (来自快照行情的冗余字段) */
    int32               BestBidPrice;           /**< 最优申买价 */
    int32               BestOfferPrice;         /**< 最优申卖价 */

    /** 买一价位的委托明细(前50笔) */
    int32               BidOrderQty[50];

    /** 卖一价位的委托明细(前50笔) */
    int32               OfferOrderQty[50];

} MdsL2BestOrdersSnapshotBodyT;

/**
 * Level2 市场总览消息定义
 */
typedef struct _MdsL2MarketOverview {
    int32               OrigDate;               /**< 市场日期 (YYYYMMDD) */
    int32               OrigTime;               /**< 市场时间 (HHMMSSss0, 实际精度为百分之一秒(HHMMSSss)) */

    int32               __exchSendingTime;      /**< 交易所发送时间 (HHMMSS000, 实际精度为秒(HHMMSS)) */
    int32               __mdsRecvTime;          /**< MDS接收到时间 (HHMMSSsss) */
} MdsL2MarketOverviewT;

/**
 * 价位信息定义
 */
typedef struct _MdsPriceLevelEntry {
    int32               Price;                  /**< 委托价格 */
    int32               NumberOfOrders;         /**< 价位总委托笔数 (Level1不揭示该值, 固定为0) */
    int64               OrderQty;               /**< 委托数量 */
} MdsPriceLevelEntryT;

/**
 * Level2 快照行情定义
 * 股票(A、B股)、债券、基金
 *
 * 关于集合竞价期间的虚拟集合竞价行情 (上海L2、深圳L2):
 * - 深圳L2集合竞价期间的虚拟成交价通过买卖盘档位揭示, 其中买一和卖一都揭示虚拟成交价格和成交数量,
 *   买二或卖二揭示虚拟成交价位上的买剩余量或卖剩余量
 * - 上海L2的虚拟集合竞价行情通过单独虚拟集合竞价快照消息(MdsL2VirtualAuctionPriceT)发布
 */
typedef struct _MdsL2StockSnapshotBody {
    /** 产品代码 C6 / C8 (如: '600000' 等) */
    char                SecurityID[9];

    /**
     * 产品实时阶段及标志 C8 / C4
     *
     * 上交所股票 (C8):
     *  -# 第 1 位:
     *      - ‘S’表示启动 (开市前) 时段, ‘C’表示集合竞价时段, ‘T’表示连续交易时段
     *      - ‘B’表示休市时段, ‘E’表示闭市时段, ‘P’表示产品停牌
     *      - ‘M’表示可恢复交易的熔断时段 (盘中集合竞价), ‘N’表示不可恢复交易的熔断时段 (暂停交易至闭市)
     *      - ‘D’表示开盘集合竞价阶段结束到连续竞价阶段开始之前的时段 (如有) 。
     *  -# 第 2 位:
     *      - ‘0’表示此产品不可正常交易,
     *      - ‘1’表示此产品可正常交易,
     *      - 无意义填空格。
     *  -# 第 3 位:
     *      - ‘0’表示未上市, ‘1’表示已上市。
     *  -# 第 4 位:
     *      - ‘0’表示此产品在当前时段不接受进行新订单申报,
     *      - ‘1’ 表示此产品在当前时段可接受进行新订单申报。
     *      - 无意义填空格。
     *
     * 上交所期权 (C4):
     *  -# 第 1 位:
     *      - ‘S’表示启动(开市前)时段, ‘C’表示集合竞价时段, ‘T’表示连续交易时段,
     *      - ‘B’表示休市时段, ‘E’表示闭市时段, ‘V’表示波动性中断, ‘P’ 表示临时停牌, ‘U’收盘集合竞价。
     *      - ‘M’表示可恢复 交易的熔断(盘中集合竞价), ‘N’表示不可恢复交易的熔断(暂停交易至闭市)
     *  -# 第 2 位:
     *      - ‘0’表示未连续停牌, ‘1’表示连续停牌。(预留,暂填空格)
     *  -# 第 3 位:
     *      - ‘0’表示不限制开仓, ‘1’表示限制备兑开仓, ‘2’表示卖出开仓, ‘3’表示限制卖出开仓、备兑开仓,
     *      - ‘4’表示限制买入开仓, ‘5’表示限制买入开 仓、备兑开仓,‘6’表示限制买入开仓、卖出开仓,
     *      - ‘7’表示限制买入开仓、卖出开仓、备兑开仓
     *  -# 第 4 位:
     *      - ‘0’表示此产品在当前时段不接受进行新订单申报, ‘1’ 表示此产品在当前时段可接受进行新订单申报。
     *
     * 深交所 (C8):
     *  -# 第 0 位:
     *      - S=启动(开市前) O=开盘集合竞价 T=连续竞价
     *      - B=休市 C=收盘集合竞价 E=已闭市 H=临时停牌
     *      - A=盘后交易 V=波动性中断
     *  -# 第 1 位:
     *      - 0=正常状态
     *      - 1=全天停牌
     */
    char                TradingPhaseCode[9];
    char                __filler[6];            /**< 按64位对齐的填充域 */

    uint64              NumTrades;              /**< 成交笔数 */
    uint64              TotalVolumeTraded;      /**< 成交总量 */
    int64               TotalValueTraded;       /**< 成交总金额 (金额单位精确到元后四位, 即: 1元=10000) */

    int32               PrevClosePx;            /**< 昨日收盘价/期权合约昨日结算价 (价格单位精确到元后四位, 即: 1元=10000) */
    int32               OpenPx;                 /**< 今开盘价 (价格单位精确到元后四位, 即: 1元=10000) */
    int32               HighPx;                 /**< 最高价 */
    int32               LowPx;                  /**< 最低价 */
    int32               TradePx;                /**< 成交价 */
    int32               ClosePx;                /**< 今收盘价/期权收盘价 (仅上海, 深圳行情没有单独的收盘价) */

    int32               IOPV;                   /**< 基金份额参考净值/ETF申赎的单位参考净值 (适用于基金) */
    int32               NAV;                    /**< 基金 T-1 日净值 (适用于基金) */
    uint64              TotalLongPosition;      /**< 合约总持仓量 (适用于期权) */

    int64               TotalBidQty;            /**< 委托买入总量 */
    int64               TotalOfferQty;          /**< 委托卖出总量 */
    int32               WeightedAvgBidPx;       /**< 加权平均委买价格 */
    int32               WeightedAvgOfferPx;     /**< 加权平均委卖价格 */
    int32               BidPriceLevel;          /**< 买方委托价位数 (实际的委托价位总数, 仅上海) */
    int32               OfferPriceLevel;        /**< 卖方委托价位数 (实际的委托价位总数, 仅上海) */

    /** 十档买盘价位信息 */
    MdsPriceLevelEntryT BidLevels[10];

    /** 十档卖盘价位信息 */
    MdsPriceLevelEntryT OfferLevels[10];

} MdsL2StockSnapshotBodyT;

/**
 * 完整的 Level1/Level2 证券行情全幅消息定义
 */
typedef struct _MdsMktDataSnapshot {
    /** 行情数据的消息头 */
    MdsMktDataSnapshotHeadT                 head;

    union {
        /** Level2 快照行情(股票、债券、基金) */
        MdsL2StockSnapshotBodyT             l2Stock;
        /** Level2 委托队列(买一／卖一前五十笔委托明细) */
        //MdsL2BestOrdersSnapshotBodyT        l2BestOrders;
        /** Level2 市场总览 (仅上证) */
        //MdsL2MarketOverviewT                l2MarketOverview;
    };
} MdsMktDataSnapshotT;


/**
 * 汇总的应答消息的消息体定义
 */
typedef union _MdsMktRspMsgBody {
    /** 证券行情全幅消息 */
    MdsMktDataSnapshotT             mktDataSnapshot;
    /** Level2 逐笔成交行情 */
    MdsL2TradeT                     trade;
    /** Level2 逐笔委托行情 */
    MdsL2OrderT                     order;

    /** 市场状态消息 */
    //MdsTradingSessionStatusMsgT     trdSessionStatus;
    /** 证券实时状态消息 */
    //MdsSecurityStatusMsgT           securityStatus;
} MdsMktRspMsgBodyT;


/**
 ******************************************************
 * 通信消息的消息类型定义 -- 交易类
 ******************************************************
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
	//CZpquantType type;
	///初始化资源类别
	//CZpquantSourceType sourceType;
	///用户代码
	CZpquantUserIDType UserID;
	///登录模式
	CZpquantUserPassword UserPassword;

    char strIP[18]; //IP地址
    unsigned int uPort; //端口号
};

typedef enum _zpquantInvestorClass {
    INVESTOR_CLASS_NORMAL               = 0,        /**< 普通投资者 */
    INVESTOR_CLASS_PROFESSIONAL_A       = 1,        /**< A类专业投资者 */
    INVESTOR_CLASS_PROFESSIONAL_B       = 2,        /**< B类专业投资者 */
    INVESTOR_CLASS_PROFESSIONAL_C       = 3         /**< C类专业投资者 */
} ZpquantInvestorClassT;

/**
 * 出入金方向定义
 */
typedef enum _zpquantFundTrsfDirect {
    FUND_TRSF_DIRECT_IN                 = 0,        /**< 转入 (入金) */
    FUND_TRSF_DIRECT_OUT                = 1         /**< 转出 (出金) */
} ZpquantFundTrsfDirectT;

/**
 * OES 竞价时段定义
 */
typedef enum _zpquantTrdSessType {
    TRD_SESS_TYPE_O                     = 0,        /**< 开盘集合竞价时段 */
    TRD_SESS_TYPE_T                     = 1,        /**< 连续竞价时段 */
    TRD_SESS_TYPE_C                     = 2,        /**< 收盘集合竞价 */
    __TRD_SESS_TYPE_MAX                             /**< 时段类型最大值 (时段类型数量) */
} pquantTrdSessTypeT;

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
	char code[7];
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
        int32           branchId;
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
typedef ZpquantStockBaseInfo      OesStockItemT;

#endif
