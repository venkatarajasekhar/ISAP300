/*
 * Lpc3250upload.cpp
 *
 *  Created on: 2013-3-16
 *      Author: Administrator
 */

#include "Lpc3250upload.h"

//#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Mid_NandFlash.h"
#include "sysapi.h"
#include "bsp_interrupt.h"
#include "os.h"
#include "CPPTools.h"
#include <iostream>
#include "ProtectAssistCell.h"

//extern "C" {
//	void __swi(8)  EnterCritical (void);
//	void __swi(9)  ExitCritical (void);
//}
using namespace std;

TASK void tsk_upmain(void);

Lpc3250_upload::Lpc3250_upload(const char* name) {
	// TODO Auto-generated constructor stub
    if( Init_data_storage() != TRUE ) {
        throw;
    }

    if( !cfg_load(&cfg) ) {
        throw;
    }
	if( name ) {
        if( strlen(name) > 20 ) {
            throw;
        }
        strcpy( filename, name );
	}
	else {
	    strcpy(filename, "main.bit");
	}
};

Lpc3250_upload::~Lpc3250_upload() {
	// TODO Auto-generated destructor stub
}

bool Lpc3250_upload::startUpload(void) {
	FILE* inf = fopen((const char*)filename, "rb");
	if( inf != NULL ) {
        /*check file's CRC*/
        uint32 crcInFile = 0;
        CPPTools::getFileCRC(filename, &crcInFile, -4);
        uint32 CRCReal = 0;
        CPPTools::calculateFileCRC(filename, &CRCReal);
        if( crcInFile != CRCReal ) {
#ifdef EZ_DEBUG
            std::cout << filename << " is damaged!!"<< std::endl;
#endif
            fclose(inf);
            return false;
        }
		saveinfo.BlockFirst = cfg.fsave.block_first;
		fseek(inf, 0, SEEK_END);
		uint32 size = ftell(inf);
		fseek(inf, 0, SEEK_SET);
	    char* contents = new char [size];
		saveinfo.BufAddr = (uint32)contents;
		fread(contents, 1, size, inf);
		saveinfo.ByteNums = size;
		fclose(inf);

		savefile.setInfo(saveinfo);
		savefile.SaveData(0);
		cfg.fsave.num_bytes = saveinfo.ByteNums;
		cfg.fsave.secs_used = savefile.getInfo()->SecsUsed;
		cfg.fsave.blocks_used = savefile.getInfo()->BlockUsed;
		cfg_save(&cfg);

		delete [] contents;
		return true;
	}
	return false;
}

int updata_main(char* fname) {
    os_tsk_create(tsk_upmain, P_SYNC);
    return 1;
}

TASK void tsk_upmain(void) {
    Dcc_interrupt_disable();
    int_disable_eth_real();
    printf("\n updata_main start!\n");
    tsk_lock();
	Lpc3250_upload up3250("main.bit");
	up3250.startUpload();
	tsk_unlock();
    printf("\n updata_main end!\n");
    if( ProtectAssistCell::instance().getOMUCurrentWorkingState() == OMU_Working ) {
        Dcc_interrupt_enable();
    }
    int_enable_eth_real();

    os_tsk_delete_self();
}
