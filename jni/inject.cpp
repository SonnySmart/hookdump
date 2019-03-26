/*
 * inject.c
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#include "tool/define.h"

extern void cocos_entry(void *handle);
extern void unity_entry(void *handle);
extern void tersafe_entry(void *handle);
extern void mqm_entry(void *handle);

size_t n_unity_load_count = 0;
size_t n_cocos_load_count = 0;
size_t n_tersafe_load_count = 0;
size_t n_mqm_load_count = 0;

void* (*old_dlopen)(const char* filename, int myflags);
void* new_dlopen(const char* filename, int myflags)
{
    void *handle = old_dlopen(filename, myflags);
    if (handle)
    {
    	LOGD("so:%s", filename);

    	do {
    	if (strstr(filename, "libcocos2dlua.so") ||
    			strstr(filename, "libgame.so") ||
    			strstr(filename, "libCEGUILuaScriptModule-0.so") ||
    			strstr(filename, "libslua.so") ||
    			strstr(filename, "libxlua.so")
    			)
    	{
    		if (n_cocos_load_count++ > 0) break;
    		//to cocos
    		LOGE("cocos_entry");
    		cocos_entry(handle);
    	}
    	if ((strstr(filename, "libmono.so") ||
    			strstr(filename, "libil2cpp.so"))
    			)
    	{
    		if (n_unity_load_count++ > 0) break;
    		//to unity
    		LOGE("unity_entry");
    		//unity_entry(handle);
    	}
    	if (strstr(filename, "libtersafe.so"))
    	{
    		if (n_tersafe_load_count++ > 0) break;
    		//LOGE("tersafe_entry");
    		//tersafe_entry(handle);
    	}
    	if (strstr(filename, "libmqm.so"))
    	{
    		if (n_mqm_load_count++ > 0) break;
			//to mqm
			LOGE("mqm_entry");
			mqm_entry(handle);
    	}
    	} while (false);
    }

    return handle;
}

__attribute__((__constructor__)) void _MSInitialize()
{
	LOGD("_MSInitialize .");

	n_unity_load_count = 0;
	n_cocos_load_count = 0;
	n_tersafe_load_count = 0;
	n_mqm_load_count = 0;

	void *dlopen_addr = get_remote_addr(getpid(), "/system/bin/linker", (void *)dlopen);
	LOGI("[+] dlopen_addr: [%x]", dlopen_addr);

	MSHookFunction((void*)dlopen_addr, (void*)new_dlopen, (void**)&old_dlopen);
}

