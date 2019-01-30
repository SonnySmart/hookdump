/*
 * define.h
 *
 *  Created on: 2019-1-22
 *      Author: Administrator
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include "../SNativeHook/jni/Hooker.h"
#include "jnibridge.h"
#include "dump.h"

using namespace std;

#define TAG "hookdump"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#define Hook(handle, __func__) \
		void *symbol = dlsym(handle, __func__); \
		if (symbol) \
		{ \
			LOGI("hook_symbol %s", __func__); \
			MSHookFunction(symbol, (void *)&new_###__func__, (void **)&old_###__func__); \
		} \

#endif /* DEFINE_H_ */
