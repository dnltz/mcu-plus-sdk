/*
 * Copyright (c) 2023 Texas Instruments Incorporated
 * Copyright (c) 2023 Excelfore Corporation (https://excelfore.com)
 *
 * All rights reserved not granted herein.
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free,
 * non-exclusive license under copyrights and patents it now or hereafter
 * owns or controls to make, have made, use, import, offer to sell and sell ("Utilize")
 * this software subject to the terms herein. With respect to the foregoing patent
 * license, such license is granted solely to the extent that any such patent is necessary
 * to Utilize the software alone. The patent license shall not apply to any combinations which
 * include this software, other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license (including the
 * above copyright notice and the disclaimer and (if applicable) source code license limitations below)
 * in the documentation and/or other materials provided with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided that the following
 * conditions are met:
 *
 * * No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any
 * software provided in binary form.
 * * any redistribution and use are licensed by TI for use only with TI Devices.
 * * Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the source code are permitted
 * provided that the following conditions are met:
 *
 * * any redistribution and use of the source code, including any resulting derivative works, are licensed by
 * TI for use only with TI Devices.
 * * any redistribution and use of any object code compiled from the source code and any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or
 * promote products derived from this software without specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI"S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI"S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/
/* Automatically generated file.  Don't edit this file.*/
#include "../yang_db_access.h"
#include "../yang_node.h"
#include "ietf-yang-library.h"
#include "ietf-netconf-monitoring.h"
#include "yang_db_identiyref.h"
#include "yang_db_enumeration.h"
extern uint8_t IETF_YANG_LIBRARY_func(uc_dbald *dbald);
#define IETF_YANG_LIBRARY_RW IETF_YANG_LIBRARY_func(dbald)
#define IETF_YANG_LIBRARY_RO (IETF_YANG_LIBRARY_func(dbald)|0x80u)
extern uint8_t IETF_NETCONF_MONITORING_func(uc_dbald *dbald);
#define IETF_NETCONF_MONITORING_RW IETF_NETCONF_MONITORING_func(dbald)
#define IETF_NETCONF_MONITORING_RO (IETF_NETCONF_MONITORING_func(dbald)|0x80u)
int ieee1588_ptp_tt_nconf_config_init(uc_dbald *dbald)
{
	uint8_t aps[6]={IETF_YANG_LIBRARY_RO,
		IETF_YANG_LIBRARY_MODULES_STATE, IETF_YANG_LIBRARY_MODULE, 0, 255u, 255u};
	uint32_t res;
	void *kvs[5]={NULL};
	uint8_t kss[5];
        void *nvalue;
        uint32_t nvsize;
	yang_db_access_para_t dbpara={YANG_DB_ACTION_CREATE,YANG_DB_ONHW_NOACTION,
	                              NULL,aps,kvs,kss,NULL,0};

	kvs[0]=(void*)"ieee1588-ptp-tt";
	kss[0]=16;
	kvs[1]=(void*)"2023-08-14";
	kss[1]=11;
	kvs[2]=NULL;
	aps[3]=IETF_YANG_LIBRARY_CONFORMANCE_TYPE;
	aps[4]=255u;
	dbpara.atype=YANG_DB_ACTION_READ;
	res=1;
	if(yang_db_action(dbald, NULL, &dbpara)==0){
		res=*((uint32_t*)dbpara.value);
		dbpara.atype=YANG_DB_ACTION_READ_RELEASE;
		yang_db_action(dbald, NULL, &dbpara);
		// if res==0, this item was already registered
	}
	if(res==1){
		res=0;
		dbpara.value=&res;
		dbpara.vsize=4;
		dbpara.atype=YANG_DB_ACTION_CREATE;
		if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}
		aps[3]=IETF_YANG_LIBRARY_NAMESPACE;
		dbpara.value=(void*)"urn:ieee:std:1588:yang:ieee1588-ptp-tt";
		dbpara.vsize=39;
		if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}
		aps[3]=IETF_YANG_LIBRARY_FEATURE;
		dbpara.atype=YANG_DB_ACTION_APPEND;
		dbpara.value=(void*)"external-port-config";
		dbpara.vsize=21;
		if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}
		dbpara.value=(void*)"performance-monitoring";
		dbpara.vsize=23;
		if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}
		aps[3]=IETF_YANG_LIBRARY_DEVIATION;
		aps[4]=IETF_YANG_LIBRARY_DUMMY;
		res=0;
		dbpara.value=&res;
		dbpara.vsize=1;
		dbpara.atype=YANG_DB_ACTION_CREATE;
		kvs[2]=(void*)"xl4-ieee1588-ptp-tt";
		kss[2]=20;
		kvs[3]=(void*)"2022-11-25";
		kss[3]=11;
		if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}
	};

	dbpara.atype=YANG_DB_ACTION_CREATE;
	kvs[0]=NULL;
	aps[2]=IETF_YANG_LIBRARY_MODULE_SET_ID;
	aps[3]=255u;
	dbpara.value=(void*)"db79ff1f92110761ada83e9127ce69cf0ab80a62";
	dbpara.vsize=41;
	if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}

	// ieee1588-ptp-tt ietf-netconf-monitoring
	aps[0]=IETF_NETCONF_MONITORING_RO;
	aps[1]=IETF_NETCONF_MONITORING_NETCONF_STATE;
	aps[2]=IETF_NETCONF_MONITORING_CAPABILITIES;
	aps[3]=IETF_NETCONF_MONITORING_CAPABILITY;
	aps[4]=255u;
	kvs[0]=NULL;
	dbpara.atype=YANG_DB_ACTION_APPEND;

	nvalue=(void*)"urn:ieee:std:1588:yang:ieee1588-ptp-tt?module=ieee1588-ptp-tt&revision=2023-08-14&features=external-port-config,performance-monitoring&deviations=xl4-ieee1588-ptp-tt";
	nvsize=166;
	dbpara.value=nvalue;
	dbpara.vsize=61;
	if(yang_db_leaflist_capupdate(dbald, NULL, &dbpara, nvalue, nvsize)!=0){return -1;}

	// ieee1588-ptp-tt ietf-netconf-monitoring schemas
	aps[2]=IETF_NETCONF_MONITORING_SCHEMAS;
	aps[3]=IETF_NETCONF_MONITORING_SCHEMA;
	kvs[3]=NULL;

	kvs[0]=(void*)"ieee1588-ptp-tt";
	kss[0]=16;
	kvs[1]=(void*)"2023-08-14";
	kss[1]=11;
	res=yang_identityref_getval("yang", "format");
	kvs[2]=&res;
	kss[2]=4;
	aps[4]=IETF_NETCONF_MONITORING_NAMESPACE;
	dbpara.value=(void*)"urn:ieee:std:1588:yang:ieee1588-ptp-tt";
	dbpara.vsize=39;
	if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}
	aps[4]=IETF_NETCONF_MONITORING_LOCATION;
	dbpara.value=(void*)"NETCONF";
	dbpara.vsize=8;
	if(yang_db_action(dbald, NULL, &dbpara)!=0){return -1;}

	return 0;
}
