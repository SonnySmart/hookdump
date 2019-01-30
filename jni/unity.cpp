/*
 * unity.cpp
 *
 *  Created on: 2019-1-22
 *      Author: Administrator
 */
#include "tool/define.h"
#include "unity/define.h"

#define DUMP_DLL 0
#define REPLACE_DLL 0
#define INJECT_DLL 0

mono_get_root_domain_t mono_get_root_domain = NULL;
mono_thread_attach_t mono_thread_attach = NULL;
mono_thread_detach_t mono_thread_detach = NULL;
mono_thread_current_t mono_thread_current = NULL;
mono_image_open_from_data_t mono_image_open_from_data = NULL;
mono_assembly_load_from_full_t mono_assembly_load_from_full = NULL;
mono_assembly_get_image_t mono_assembly_get_image = NULL;
mono_class_from_name_t mono_class_from_name = NULL;
mono_class_get_method_from_name_t mono_class_get_method_from_name = NULL;
mono_runtime_invoke_t mono_runtime_invoke = NULL;

void inject_dll();

void *inject_dll_func(void *arg)
{
	inject_dll();
	return (void *)0;
}

void (*old_mono_image_init)(MonoImage *image);
void new_mono_image_init(MonoImage *image)
{
	LOGI("mono_image_init name:%s", image->name);
	if (strstr(image->name, "Assembly-CSharp.dll") ||
			strstr(image->name, "Assembly-CSharp-firstpass.dll")
			) {
		//LOGI("mono_image_init raw_data:%x", image->raw_data);
		//LOGI("mono_image_init raw_data_len:%d", image->raw_data_len);

		const char *fullpath = get_sdcard_fullpath(image->name);

#if DUMP_DLL
		dump_write(fullpath, image->raw_data, image->raw_data_len);
#endif
	}

	if (strstr(image->name, "Assembly-CSharp.dll")) {
#if REPLACE_DLL
		vector<string> r = {
				"Assembly-CSharp.dll"
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
#endif
	}

	old_mono_image_init(image);

	if (strstr(image->name, "System.Xml.dll")) {
		pthread_t tid;
		int err;
		if ((err = pthread_create(&tid, NULL, inject_dll_func, NULL)) != 0) {
			LOGE("can't create thread: %s\n",strerror(err));
		}
	}
}

void inject_dll()
{
#if INJECT_DLL
	vector<string> r = {
			"UnityInject.dll"
	};
	void *buffer = NULL;
	size_t len = 0;
	if (replace_buffer("UnityInject.dll", r, buffer, len) == 0) {
		LOGI("buffer:%x", buffer);
		LOGI("len:%d", len);
		if (buffer != NULL && len > 0) {
			LOGI("call inject method .");

			mono_thread_attach(mono_get_root_domain());

			MonoImageOpenStatus *status;
			void* image = mono_image_open_from_data((char *)buffer, len, 1, status);
			if (image == NULL)
			{
				LOGE("mono_image_open_from_data image == NULL");
				return;
			}
			void* assembly = mono_assembly_load_from_full(image, NULL, status, 0);
			if (assembly == NULL)
			{
				LOGE("mono_assembly_load_from_full assembly == NULL");
				return;
			}
			image = mono_assembly_get_image(assembly);
			if (image == NULL)
			{
				LOGE("mono_assembly_get_image image == NULL");
				return;
			}
			void* pClass = mono_class_from_name(image, "UnityEx", "Loader");
			if (pClass == NULL)
			{
				LOGE("mono_class_from_name pClass == NULL");
				return;
			}
			void* method = mono_class_get_method_from_name(pClass, "Load", 0);
			if (method == NULL)
			{
				LOGE("mono_class_get_method_from_name method == NULL");
				return;
			}
			void *res = mono_runtime_invoke(method, NULL, NULL, NULL);
			if (res == NULL)
			{
				LOGE("mono_runtime_invoke res == NULL");
				return;
			}
			//mono_thread_detach (mono_thread_current ());
		}
	}
#endif
}

void unity_entry(void *handle)
{
	LOGI("handle addr[%x]", handle);
	void * symbol = NULL;
	symbol = dlsym(handle, "mono_image_init");
	if (symbol)
	{
		LOGI("hook_symbol addr . %x", symbol);
		MSHookFunction(symbol, (void *)&new_mono_image_init, (void **)&old_mono_image_init);
	}

	// inject func
	symbol = dlsym(handle, "mono_get_root_domain");
	if (symbol) mono_get_root_domain = (mono_get_root_domain_t)symbol;
	else LOGE("symbol NULL mono_get_root_domain");
	symbol = dlsym(handle, "mono_thread_attach");
	if (symbol) mono_thread_attach = (mono_thread_attach_t)symbol;
	else LOGE("symbol NULL mono_thread_attach");
	if (symbol) mono_thread_detach = (mono_thread_detach_t)symbol;
	else LOGE("symbol NULL mono_thread_detach");
	if (symbol) mono_thread_current = (mono_thread_current_t)symbol;
	else LOGE("symbol NULL mono_thread_current");
	symbol = dlsym(handle, "mono_image_open_from_data");
	if (symbol) mono_image_open_from_data = (mono_image_open_from_data_t)symbol;
	else LOGE("symbol NULL mono_image_open_from_data");
	symbol = dlsym(handle, "mono_assembly_load_from_full");
	if (symbol) mono_assembly_load_from_full = (mono_assembly_load_from_full_t)symbol;
	else LOGE("symbol NULL mono_assembly_load_from_full");
	symbol = dlsym(handle, "mono_assembly_get_image");
	if (symbol) mono_assembly_get_image = (mono_assembly_get_image_t)symbol;
	else LOGE("symbol NULL mono_assembly_get_image");
	symbol = dlsym(handle, "mono_class_from_name");
	if (symbol) mono_class_from_name = (mono_class_from_name_t)symbol;
	else LOGE("symbol NULL mono_class_from_name");
	symbol = dlsym(handle, "mono_class_get_method_from_name");
	if (symbol) mono_class_get_method_from_name = (mono_class_get_method_from_name_t)symbol;
	else LOGE("symbol NULL mono_class_get_method_from_name");
	symbol = dlsym(handle, "mono_runtime_invoke");
	if (symbol) mono_runtime_invoke = (mono_runtime_invoke_t)symbol;
	else LOGE("symbol NULL mono_runtime_invoke");
}



