/*
 * inject.c
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#include "dump/dump.h"

// cocos��ں���
extern void cocos_entry(void *handle);

void* (*old_dlopen)(const char* filename, int myflags);
void* new_dlopen(const char* filename, int myflags)
{
    void *handle = old_dlopen(filename, myflags);
    if (handle)
    {
    	LOGD("so:%s", filename);

    	if (strstr(filename, "libcocos2dlua.so") ||
    			strstr(filename, "libgame.so"))
    	{
    		LOGE("cocos_entry");
    		cocos_entry(handle);
    	}
    	if (strstr(filename, "libmono.so") ||
    			strstr(filename, "libil2cpp.so"))
    	{
    		//to unity
    		LOGE("unity_entry");
    	}
    }
    return handle;
}

__attribute__((__constructor__)) static void _MSInitialize()
{
	LOGD("_MSInitialize .");

	//��ȡdlopen��ַ
	void *dlopen_addr = get_remote_addr(getpid(), "/system/bin/linker", (void *)dlopen);
	LOGI("[+] dlopen_addr: [%x]", dlopen_addr);
	//hook dlopen����  ���淽������
	MSHookFunction((void*)dlopen_addr, (void*)new_dlopen, (void**)&old_dlopen);
}

