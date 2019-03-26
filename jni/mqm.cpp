/*
 * mqm.cpp
 *
 *  Created on: 2019-3-26
 *      Author: Administrator
 */

#include "tool/define.h"

int (*old_open) (const char *file, int __oflag, ...);

int new_open(const char *file, int __oflag, ...)
{
	if (strstr(file, ".lc")
			|| strstr(file, ".prop")
			|| strstr(file, ".ui")
			|| strstr(file, ".uip"))
	{
		LOGI("file:%s __oflag:%d", file, __oflag);
	}

	va_list valist;
	va_start(valist, file);

    int ret = old_open(file, __oflag, valist);

    va_end(valist);

    return ret;
}

FILE *(*old_fopen1)(const char *path, const char *mode);

FILE *new_fopen1(const char *path, const char *mode)
{
	if (strstr(path, ".lc")
				|| strstr(path, ".prop")
				|| strstr(path, ".ui")
				|| strstr(path, ".uip"))
	{
		LOGI("path . %s mode . %s", path, mode);
	}

    return old_fopen1(path, mode);
}

signed int (*old_LoadFile)(std::vector<char, std::allocator<char>> &);
signed int new_LoadFile(std::vector<char, std::allocator<char>> &param)
{
	char *data = param.data();
	LOGI("data . %s", data);

	return old_LoadFile(param);
}

signed int (*old_M_open)(char const*,int,long);
signed int new_M_open(const char *pathname, int flags, long mode)
{
	if (strstr(pathname, ".lc")
					|| strstr(pathname, ".prop")
					|| strstr(pathname, ".ui")
					|| strstr(pathname, ".uip"))
		{
			LOGI("pathname . %s", pathname);
		}
	return old_M_open(pathname, flags, mode);
}

void mqm_entry(void *handle)
{
	LOGI("handle addr[%x]", handle);
	void * symbol = NULL;

	symbol = dlsym(handle, "_ZN10CMacroFile8LoadFileERSt6vectorIcSaIcEE");
	if (symbol)
	{
		LOGI("_ZN10CMacroFile8LoadFileERSt6vectorIcSaIcEE addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_LoadFile, (void **)&old_LoadFile);
	}

	symbol = dlsym(handle, "_ZNSt13_Filebuf_base7_M_openEPKcil");
	if (symbol)
	{
		LOGI("_ZNSt13_Filebuf_base7_M_openEPKcil addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_M_open, (void **)&old_M_open);
	}

	//MSHookFunction(&fopen, &new_fopen1, &old_fopen1);

	MSHookFunction(&open, &new_open, &old_open);
}


