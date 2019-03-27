/*
 * mqm.cpp
 *
 *  Created on: 2019-3-26
 *      Author: Administrator
 */

#include "tool/define.h"
#include <iostream>

std::vector<int> gFds;

int (*old_open) (const char *file, int __oflag, ...);

int new_open(const char *file, int __oflag, ...)
{
	if (strstr(file, ".lc")
			|| strstr(file, ".ui"))
	{
		LOGI("file:%s __oflag:%d", file, __oflag);
	}

	va_list valist;
	va_start(valist, file);

    int ret = old_open(file, __oflag, valist);

    if (strstr(file, ".lc")
    			|| strstr(file, ".ui"))
    {
    	auto find = std::find(gFds.begin(), gFds.end(), ret);
    	if (find != gFds.end())
    	{
    		gFds.erase(find);
    	}
    	gFds.push_back(ret);

    	LOGI("add fd:%d", ret);
    }

    va_end(valist);

    return ret;
}

ssize_t (*old_read)(int fd, void *buf, size_t nbytes);
ssize_t new_read(int fd, void *buf, size_t nbytes)
{
	ssize_t ret = old_read(fd, buf, nbytes);

	for (auto &i : gFds)
	{
		if (fd == i)
			LOGI("fd:%d buf:%s", i, buf);
	}

	return ret;
}

bool (*old_M_open_char) ( const char *, std::ios_base::openmode );
bool new_M_open_char ( const char *name, std::ios_base::openmode mode)
{
	if (strstr(name, ".lc")
					|| strstr(name, ".prop")
					|| strstr(name, ".ui")
					|| strstr(name, ".uip"))
		{
			//LOGI("new_M_open_char name:%s", name);
		}

	LOGI("new_M_open_char name:%s mode:%d", name, mode);
	return old_M_open_char(name, mode);
}

bool (*old_M_open_int) ( int, std::ios_base::openmode );
bool new_M_open_int ( int fd, std::ios_base::openmode mode)
{
	LOGI("new_M_open_int fd:%d", fd);
	return old_M_open_int(fd, mode);
}

int (*old_M_read)(char *, int);
int new_M_read(char * buf, int len)
{
	int ret = old_M_read(buf, len);

	LOGI("new_M_read buf:%s", buf);

	return ret;
}

void mqm_entry(void *handle)
{
	LOGI("handle addr[%x]", handle);
	void * symbol = NULL;

	symbol = dlsym(handle, "_ZNSt13_Filebuf_base7_M_openEii");
	if (symbol)
	{
		LOGI("_ZNSt13_Filebuf_base7_M_openEii addr . %x", symbol);
		//MSHookFunction(symbol, (void *)&new_M_open_int, (void **)&old_M_open_int);
	}

	symbol = dlsym(handle, "_ZNSt13_Filebuf_base7_M_openEPKci");
	if (symbol)
	{
		LOGI("_ZNSt13_Filebuf_base7_M_openEPKci addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_M_open_char, (void **)&old_M_open_char);
	}

	symbol = dlsym(handle, "_ZNSt13_Filebuf_base7_M_readEPci");
	if (symbol)
	{
		LOGI("_ZNSt13_Filebuf_base7_M_readEPci addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_M_read, (void **)&old_M_read);
	}

	//MSHookFunction(&fopen, &new_fopen1, &old_fopen1);

	MSHookFunction(&open, &new_open, &old_open);
	//MSHookFunction(&read, &new_read, &old_read);
}


