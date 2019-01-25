/*
 * unity.cpp
 *
 *  Created on: 2019-1-22
 *      Author: Administrator
 */
#include "tool/define.h"
#include "unity/define.h"

#define DUMP_DLL 1
#define REPLACE_DLL 0

MonoImage *
(*old_mono_image_open_from_data_with_name)(char *data, guint32 data_len, gboolean need_copy, MonoImageOpenStatus *status, gboolean refonly, const char *name);
MonoImage *
new_mono_image_open_from_data_with_name(char *data, guint32 data_len, gboolean need_copy, MonoImageOpenStatus *status, gboolean refonly, const char *name)
{
	MonoImage *image = old_mono_image_open_from_data_with_name(data, data_len, need_copy, status, refonly, name);

	//LOGD("data:%x", data);
	//LOGD("data_len:%d", data_len);
	//LOGD("name:%s", name);

	if (strstr(name, "Assembly-CSharp.dll") ||
			strstr(name, "UnityEngine.dll")
			) {
		LOGI("mono_image_open_end raw_data:%x", image->raw_data);
		LOGI("mono_image_open_end raw_data_len:%d", image->raw_data_len);

		const char *fullpath = get_sdcard_fullpath(name);

#if DUMP_DLL
		dump_write(fullpath, image->raw_data, image->raw_data_len);
#endif
	}

	return image;
}

void (*old_mono_image_init)(MonoImage *image);
void new_mono_image_init(MonoImage *image)
{
	if (strstr(image->name, "Assembly-CSharp.dll")) {
		LOGI("mono_image_init raw_data:%x", image->raw_data);
		LOGI("mono_image_init raw_data_len:%d", image->raw_data_len);

#if REPLACE_DLL
		vector<string> r = {
				"Assembly-CSharp.dll",
				"UnityInject.dll"
		};
		void *out_buffer = NULL;
		size_t out_len = 0;
		if (replace_buffer(image->name, r, out_buffer, out_len) == 0) {
			LOGI("buff_old:%x", image->raw_data);
			LOGI("old_len:%d", image->raw_data_len);
			LOGI("buff_new:%x", out_buffer);
			LOGI("out_len:%d", out_len);
			if (out_buffer != NULL && out_len > 0) {
				LOGI("call image->raw_data = out_buffer;");
				image->raw_data = (char *)out_buffer;
				image->raw_data_len = out_len;
			}
		}
		if (replace_buffer("UnityInject.dll", r, out_buffer, out_len) == 0) {
			LOGI("buff_old:%x", image->raw_data);
			LOGI("old_len:%d", image->raw_data_len);
			LOGI("buff_new:%x", out_buffer);
			LOGI("out_len:%d", out_len);
			if (out_buffer != NULL && out_len > 0) {
				LOGI("call image->raw_data = out_buffer;");
				image->raw_data = (char *)out_buffer;
				image->raw_data_len = out_len;
			}
		}
#endif
	}

	old_mono_image_init(image);
}

void unity_entry(void *handle)
{
	LOGI("handle addr[%x]", handle);
	void * symbol = NULL;
	symbol = dlsym(handle, "mono_image_open_from_data_with_name");
	if (symbol)
	{
		LOGI("hook_symbol addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_mono_image_open_from_data_with_name, (void **)&old_mono_image_open_from_data_with_name);
	}
	symbol = dlsym(handle, "mono_image_init");
	if (symbol)
	{
		LOGI("hook_symbol addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_mono_image_init, (void **)&old_mono_image_init);
	}
}



