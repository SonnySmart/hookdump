

#include "tool/define.h"

void (*old_tss_unity_str)(char * data, size_t len);
void new_tss_unity_str (char * data, size_t len)
{
	LOGI("%s data:%08x len:%d", __FUNCTION__, data, len);
	return old_tss_unity_str(data, len);
}

int (*old_sub_8E290)(void * a1);
int new_sub_8E290(void * a1)
{
	LOGI("%s a1:%08x", __FUNCTION__, a1);
	return old_sub_8E290(a1);
}

void tersafe_entry(void *handle)
{
//	void * symbol = NULL;
//	symbol = dlsym(handle, "tss_unity_str");
//	if (symbol)
//	{
//		LOGI("hook_symbol addr . %x", symbol);
//		MSHookFunction(symbol, (void *)&new_tss_unity_str, (void **)&old_tss_unity_str);
//	}
//	symbol = dlsym(handle, (const char *)0x0008E290);
//	if (symbol)
//	{
//		LOGI("hook_symbol addr . %08x", symbol);
//		MSHookFunction(symbol, (void *)&new_sub_8E290, (void **)&old_sub_8E290);
//	}

	xhook_register(".*/libtersafe\\.so$", "tss_unity_str",  (void *) &new_tss_unity_str,  (void **) &old_tss_unity_str);

	xhook_refresh(1);
}
