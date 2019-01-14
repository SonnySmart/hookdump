/*
 * cocos.c
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#include "dump/dump.h"
#include "cocos/CCData.h"
#include <string>
#include <time.h>
#include <stdlib.h>

const char *des_root = "/sdcard/Android/data/com.tc.tbnn";

int (*old_luaLoadBuffer)(void * L, char ** a2, char *buffer, int len, char * path);
int new_luaLoadBuffer(void * L, char ** a2, char *buffer, int len, char * path) {
	LOGI("L:%x", L);
	LOGI("a2:%s", *a2);
	LOGI("buffer:%s", buffer);
	LOGI("len:%d", len);
	LOGI("path:%s", path);
	return old_luaLoadBuffer(L, a2, buffer, len, path);
}

int (*old_luaL_loadbuffer) (void *L, const char *buff, size_t len, const char *name);
int new_luaL_loadbuffer (void *L, const char *buff, size_t len, const char *name) {
	LOGI("buff:%x", buff);
	LOGI("len:%d", len);
	LOGI("name:%s", name);

	char des[300] = { 0 };
	sprintf(des, "%s/%s", des_root, name);

	dump_write(des, buff, len);

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

FILE *my_fopen(const char *path, const char *mode)
{
	LOGI("path . %s mode . %s", path, mode);
    return old_fopen(path, mode);
}

int (*old_getDataFromFile)(Data *a1, std::string *a2, int a3);
int new_getDataFromFile(Data *a1, std::string *a2, int a3) {

	//LOGI("filename:%s", a2);
	LOGI("a2:%s", (*a2).c_str());

	int ret = old_getDataFromFile(a1, a2, a3);
	LOGI("buffer:%d", (*a1).getSize());
	return ret;
}

bool (*old_isCCZBuffer)(const unsigned char *buffer, ssize_t len);
bool new_isCCZBuffer(const unsigned char *buffer, ssize_t len) {
	//int index = rand();

	char des[300] = { 0 };
	sprintf(des, "%s/%x.png", des_root, buffer);
	LOGI("des:%s", des);
	dump_write(des, (const char *)buffer, len);
	//LOGI("buffer:%x", buffer);
	//LOGI("len:%d", len);
	return old_isCCZBuffer(buffer, len);
}

void cocos_entry(void *handle)
{
	void * symbol = NULL;
//		symbol = dlsym(handle, (char *)"_ZN7cocos2d8LuaStack13luaLoadBufferEP9lua_StatePKciS4_");
//		if (symbol)
//		{
//			LOGI("hook_symbol addr . %x", symbol);
//			MSHookFunction(symbol, (void *)&new_luaLoadBuffer, (void **)&old_luaLoadBuffer);
//		}
//	symbol = dlsym(handle, (char *)"luaL_loadbuffer");
//	if (symbol)
//	{
//		LOGI("hook_symbol addr . %x", symbol);
//		MSHookFunction(symbol, (void *)&new_luaL_loadbuffer, (void **)&old_luaL_loadbuffer);
//	}
//	symbol = dlsym(handle, (char *)"_Z13xxtea_decryptPhjS_jPj");
//	if (symbol)
//	{
//		LOGI("hook_symbol addr . %x", symbol);
//		MSHookFunction(symbol, (void *)&xxtea_decrypt, (void **)&old_xxtea_decrypt);
//	}
	symbol = dlsym(handle, (char *)"_ZN7cocos2d8ZipUtils11isCCZBufferEPKhi");
	if (symbol)
	{
		LOGI("hook_symbol addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_isCCZBuffer, (void **)&old_isCCZBuffer);
	}

	//MSHookFunction(&fopen, &my_fopen, &old_fopen);
}


