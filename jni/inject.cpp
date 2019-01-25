/*
 * inject.c
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#include "tool/define.h"

extern void libc_entry(void *handle);
extern void cocos_entry(void *handle);
extern void unity_entry(void *handle);

void* (*old_dlopen)(const char* filename, int myflags);
void* new_dlopen(const char* filename, int myflags)
{
    void *handle = old_dlopen(filename, myflags);
    if (handle)
    {
    	LOGD("so:%s", filename);

    	if (strstr(filename, "libc.so"))
    	{
    		//libc entry
    		//LOGE("libc_entry");
    		//libc_entry(handle);
    	}

    	if (strstr(filename, "libcocos2dlua.so") ||
    			strstr(filename, "libgame.so") ||
    			strstr(filename, "libCEGUILuaScriptModule-0.so")
    			//strstr(filename, "libxlua.so")
    			)
    	{
    		LOGE("cocos_entry");
    		cocos_entry(handle);
    	}
    	if (strstr(filename, "libmono.so") ||
    			strstr(filename, "libil2cpp.so")
    			)
    	{
    		//to unity
    		LOGE("unity_entry");
    		unity_entry(handle);
    	}
    }
    return handle;
}

__attribute__((__constructor__)) void _MSInitialize()
{
	LOGD("_MSInitialize .");

	void *dlopen_addr = get_remote_addr(getpid(), "/system/bin/linker", (void *)dlopen);
	LOGI("[+] dlopen_addr: [%x]", dlopen_addr);

	MSHookFunction((void*)dlopen_addr, (void*)new_dlopen, (void**)&old_dlopen);
}

