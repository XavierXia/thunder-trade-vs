/////////////////////////////////////////////////////////////////////////
///@system 
///@company 宙澎资产
///@file ZpquantUserApiDataType.h
///@brief 定义了客户端接口使用的业务数据类型
///@history 
///2019.7.19	夏文星		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef ZPQUANT_USERAPIDATATYPE_H
#define ZPQUANT_USERAPIDATATYPE_H

typedef signed char                 int8;
typedef unsigned char               uint8;

typedef short                       int16;
typedef unsigned short              uint16;

typedef int                         int32;
typedef unsigned int                uint32;

/** 密码最大长度 */
#define ZP_PWD_MAX_LEN                     (40)
/** 资金账户代码最大长度 */
#define ZP_CASH_ACCT_ID_MAX_LEN            (16)
/** 客户端名称最大长度 */
#define ZP_CLIENT_NAME_MAX_LEN             (32)
/** 客户端说明最大长度 */
#define ZP_CLIENT_DESC_MAX_LEN             (32)
#define ZP_MAX_CUST_PER_CLIENT			    (1)

//
typedef char CZpquantType[20];
//
typedef char CZpquantSourceType[20];
//用户名
typedef char CZpquantUserIDType[20];
//登录密码
typedef char CZpquantUserPassword[20];
