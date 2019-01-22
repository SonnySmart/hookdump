/*
 * cocos.c
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#include "tool/define.h"

#define DUMP_LUA 1
#define REPLACE_LUA 0

int (*old_luaL_loadbuffer) (void *L, const char *buff, size_t len, const char *name);
int new_luaL_loadbuffer (void *L, const char *buff, size_t len, const char *name) {

	//LOGI("len:%d", len);
	//LOGI("name:%s", name);

	const char *fullpath = get_sdcard_fullpath(name);
	//LOGD("fullpath:%s", fullpath);

#if DUMP_LUA
	dump_write(fullpath, buff, len);
#endif


#if REPLACE_LUA
	vector<string> r = {
			"HeroFightUtil.lua"
	};
	void *out_buffer = NULL;
	size_t out_len = 0;
	if (replace_buffer(name, r, out_buffer, out_len) == 0) {
		LOGE("buff_old:%x", buff);
		LOGE("old_len:%d", len);
		LOGE("buff_new:%x", out_buffer);
		LOGE("out_len:%d", out_len);
		if (out_buffer != NULL && out_len > 0) {
			LOGE("call return old_luaL_loadbuffer");
			return old_luaL_loadbuffer(L, (const char *)out_buffer, out_len, name);
		}
	}
#endif

	return old_luaL_loadbuffer(L, buff, len, name);
}

unsigned char *(*old_xxtea_decrypt)(unsigned char *data, unsigned int data_len, unsigned char *key, unsigned int key_len, unsigned int *ret_length);
unsigned char *xxtea_decrypt(unsigned char *data, unsigned int data_len, unsigned char *key, unsigned int key_len, unsigned int *ret_length) {
	LOGI("data:%x", data);
	LOGI("data_len:%d", data_len);
	LOGI("key:%s", key);
	LOGI("key_len:%d", key_len);
	LOGI("ret_length:%d", ret_length);

	return old_xxtea_decrypt(data, data_len, key, key_len, ret_length);
}

FILE *(*old_fopen)(const char *path, const char *mode);

FILE *new_fopen(const char *path, const char *mode)
{
	LOGI("path . %s mode . %s", path, mode);
    return old_fopen(path, mode);
}

bool (*old_isCCZBuffer)(const unsigned char *buffer, ssize_t len);
bool new_isCCZBuffer(const unsigned char *buffer, ssize_t len) {
	//const char *fullpath = get_sdcard_fullpath(name);
	//LOGD("fullpath:%s", fullpath);
	//dump_write(fullpath, (const char *)buffer, len);
	//LOGI("buffer:%x", buffer);
	//LOGI("len:%d", len);
	return old_isCCZBuffer(buffer, len);
}

void cocos_entry(void *handle)
{
	void * symbol = NULL;
	symbol = dlsym(handle, (char *)"luaL_loadbuffer");
	if (symbol)
	{
		LOGI("hook_symbol addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_luaL_loadbuffer, (void **)&old_luaL_loadbuffer);
	}
//	symbol = dlsym(handle, (char *)"_Z13xxtea_decryptPhjS_jPj");
//	if (symbol)
//	{
//		LOGI("hook_symbol addr . %x", symbol);
//		MSHookFunction(symbol, (void *)&xxtea_decrypt, (void **)&old_xxtea_decrypt);
//	}
//	symbol = dlsym(handle, (char *)"_ZN7cocos2d8ZipUtils11isCCZBufferEPKhi");
//	if (symbol)
//	{
//		LOGI("hook_symbol addr . %x", symbol);
//		MSHookFunction(symbol, (void *)&new_isCCZBuffer, (void **)&old_isCCZBuffer);
//	}

	//MSHookFunction(&fopen, &new_fopen, &old_fopen);
}


