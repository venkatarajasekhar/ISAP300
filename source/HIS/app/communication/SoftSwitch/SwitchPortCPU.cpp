/*
 * SwitchPortCPU.cpp
 *
 *  Created on: 2013-6-26
 *      Author: Administrator
 */

#include <Net_Config.h>
#include "os.h"
#include "SwitchPortCPU.h"
//#include "Task_define.h"
#include "SoftSwitch_define.h"
#include "PriPacket.h"
#include <string.h>
//#include "MidoptDCC.h"
#include "sysapi.h"
#include <iostream>

extern OS_TID t_tcpTask;
SwitchPortCPU::SwitchPortCPU() : SwitchPortStd(CPU_Port_Sn){
	// TODO Auto-generated constructor stub

}

SwitchPortCPU::~SwitchPortCPU() {
	// TODO Auto-generated destructor stub
}

//PriPacket& SwitchPortCPU::makePacket(uint8* d, uint16 len) {
//	static uint16 tValue = 1;
//	Private_tag tag = {0};
//	tag.sn = tValue++;
//	memcpy( tag.decMAC, d, 6 );
//	memcpy( tag.srcMAC, d+6, 6 );
//	PriPacket* pkt = new PriPacket(tag, getPortSn(), d, len);
//	return *pkt;
//}


int SwitchPortCPU::outputPacket(PriPacket& pkg) {

	if( pkg.getSourcePort() == CPU_Port_Sn ) {
		return -1;
	}
	uint16 RxLen = 0;
	uint8* RxData = pkg.getStdStream(&RxLen);
	if( RxLen < 30 || RxData == 0 ) {
        std::cout << "SwitchPortCPU send error: len " << (int)RxLen << std::endl;
        return -1;
	}
#ifdef EZ_DEBUG
	trace->sendOnePkg();
#endif
	tsk_lock();
    OS_FRAME* frame = alloc_mem (RxLen | 0x80000000);
    tsk_unlock();
	if (frame != NULL) {
		memcpy( &frame->data[0], RxData, RxLen);
		put_in_queue(frame);
		os_evt_set(0x0001, t_tcpTask);
		return 1;
	}
	else {
	    std::cout << "SwitchPortCPU::outputPacket() alloc_mem(" << (int)RxLen << ") error" << std::endl;
	}
	return -1;
}
