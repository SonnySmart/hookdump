/*
 * tersafe.cpp
 *
 *  Created on: 2019-1-30
 *      Author: Administrator
 */
#include "tool/define.h"

void (*old_tss_log_str);
void new_tss_log_str()
{

}

void tss_log_str(void *handle) {
	Hook(handle, __FUNCTION__);
}

void tersafe_entry(void *handle)
{
	LOGI("handle addr[%x]", handle);

	tss_log_str(handle);
}


