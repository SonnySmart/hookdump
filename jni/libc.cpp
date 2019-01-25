/*
 * libc.cpp
 *
 *  Created on: 2019-1-25
 *      Author: Administrator
 */
#include "tool/define.h"

__noreturn void (*old_exit)(int);
__noreturn void new_exit(int)
{
	LOGE("program want exit ?");
}

void libc_entry(void *handle)
{
	MSHookFunction((void *)&exit, (void *)&new_exit, (void **)&old_exit);
	MSHookFunction((void *)&_exit, (void *)&new_exit, (void **)&old_exit);
}


