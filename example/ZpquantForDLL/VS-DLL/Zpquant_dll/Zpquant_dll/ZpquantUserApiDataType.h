/////////////////////////////////////////////////////////////////////////
///@system 
///@company �����ʲ�
///@file ZpquantUserApiDataType.h
///@brief �����˿ͻ��˽ӿ�ʹ�õ�ҵ����������
///@history 
///2019.7.19	������		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef ZPQUANT_USERAPIDATATYPE_H
#define ZPQUANT_USERAPIDATATYPE_H

typedef signed char                 int8;
typedef unsigned char               uint8;

typedef short                       int16;
typedef unsigned short              uint16;

typedef int                         int32;
typedef unsigned int                uint32;

typedef long           				int64;
typedef unsigned long   			uint64;

/** ������󳤶� */
#define ZP_PWD_MAX_LEN                     (40)
/** �ʽ��˻�������󳤶� */
#define ZP_CASH_ACCT_ID_MAX_LEN            (16)
/** �ͻ���������󳤶� */
#define ZP_CLIENT_NAME_MAX_LEN             (32)
/** �ͻ���˵����󳤶� */
#define ZP_CLIENT_DESC_MAX_LEN             (32)
#define ZP_MAX_CUST_PER_CLIENT			    (1)

//
typedef char CZpquantType[20];
//
typedef char CZpquantSourceType[20];
//�û���
typedef char CZpquantUserIDType[20];
//��¼����
typedef char CZpquantUserPassword[20];

#endif

#pragma once
