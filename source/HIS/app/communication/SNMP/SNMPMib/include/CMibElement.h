/****************************************Copyright (c)*****************************************************************
 **                            Easy Broadband Technology Co., Ltd.
 **                                      
 **                                 http://www.ebbnetwork.com
 **
 **--------------File Info----------------------------------------------------------------------------------------------
 ** FileName:				CMibElement.h
 ** Created by:				WangChangRong
 ** Created date:			2011-8-23
 ** Version:				1.0
 ** Descriptions:			
 **---------------------------------------------------------------------------------------------------------------------
 ** Modified by:				
 ** Modified date:			
 ** Version:					
 ** Descriptions:			
 **********************************************************************************************************************/

#ifndef CMIBELEMENT_H_
#define CMIBELEMENT_H_

#include "CMibEntry.h"
#include "CMibColumn.h"

class CMibElement: public CMibEntry
{
public:
	CMibElement();
	virtual ~CMibElement();
	//初始化标准组中对象
	virtual uint32 InitialGroup();
};
class CMibElementColumn: public CMibColumn
{
public:
	CMibElementColumn();
	virtual ~CMibElementColumn();

	//Get的响应方法
	virtual uint32 GetResponse(CSnmpCell* objpCell);
	//Set的响应方法
	virtual uint32 SetResponse(CSnmpCell* objpCell);
	//GetNext调用方法1：获得第一个对象的OID
	virtual uint32 GetFirstValidOID(CSnmpCell* objpCell);
	//GetNext调用方法2：获得下一个对象的OID
	virtual uint32 GetNextValidOID(CSnmpCell* objpCell);
};
#endif /* CMIBELEMENT_H_ */
