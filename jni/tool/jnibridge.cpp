/*
 * jnibridge.cpp
 *
 *  Created on: 2019-1-22
 *      Author: Administrator
 */
#include "jnibridge.h"
#include "define.h"
#include <assert.h>

#define JNI_CLASS "com/dump/inject/HookLogic"

static JNIEnv* env = NULL;
static std::string g_package;
static std::string g_sdcard_root;

jmethodID getMethod(const char *method, const char *sig, jclass &jc)
{
	jmethodID jid = NULL;
	if (env != NULL) {
		jclass clazz;
		clazz = env->FindClass(JNI_CLASS);
		if (clazz != NULL) {
			jc = clazz;
			jid = env->GetStaticMethodID(clazz, method, sig);
		}
		else {
			LOGE("getMethod jclass null .");
		}
	}
	else {
		LOGE("getMethod env null .");
	}
	return jid;
}

const char *getPackageString()
{
	char *ret = NULL;
	jclass clazz = NULL;
	jmethodID jid = getMethod(__FUNCTION__, "()Ljava/lang/String;", clazz);
	if (jid != NULL && clazz != NULL) {
		jstring js = (jstring)env->CallStaticObjectMethod(clazz, jid);
		ret = (char *)env->GetStringUTFChars(js, 0);
		env->ReleaseStringUTFChars(js, 0);
		LOGD("ret:%s", ret);
	}
	return ret;
}

const char *getSdcardDirString()
{
	char *ret = NULL;
	jclass clazz = NULL;
	jmethodID jid = getMethod(__FUNCTION__, "()Ljava/lang/String;", clazz);
	if (jid != NULL && clazz != NULL) {
		jstring js = (jstring)env->CallStaticObjectMethod(clazz, jid);
		ret = (char *)env->GetStringUTFChars(js, 0);
		env->ReleaseStringUTFChars(js, 0);
		LOGD("ret:%s", ret);
	}
	return ret;
}

const char *get_packge()
{
	return g_package.c_str();
}

const char *get_sdcard_fullpath(const char *name)
{
	if (name == NULL) {
		return (g_sdcard_root).c_str();
	}
	return (g_sdcard_root + "/" + name).c_str();
}

JNIEXPORT jstring JNICALL native_hello(JNIEnv *env, jclass clazz)
{
	g_package = getPackageString();
	g_sdcard_root = getSdcardDirString();
	LOGD("g_package:%s", get_packge());
	LOGD("g_sdcard_root:%s", get_sdcard_fullpath());
	return env->NewStringUTF((const char *)"native_hello");
}

static JNINativeMethod gmethod[] = {
        {"native_hello","()Ljava/lang/String;",(void *)native_hello},
};

static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gmethod, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
    	LOGE("registerNativeMethods clazz null");
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gmethod, numMethods) < 0) {
    	LOGE("registerNativeMethods RegisterNatives < 0");
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static int registerNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env,JNI_CLASS, gmethod,
                               sizeof(gmethod) / sizeof(gmethod[0])))
        return JNI_FALSE;

    return JNI_TRUE;
}

//jint (*old_JNI_OnLoad)(JavaVM *vm, void *reserved);
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	assert(env != NULL);

	LOGD("JNI_OnLoad env has . %x", env);

	if (!registerNatives(env)) {
		LOGE("JNI_OnLoad registerNatives FALSE");
		return -1;
	}
	/* success -- return valid version number */
	result = JNI_VERSION_1_4;

	return result;
}

//void jni_entry(void *handle)
//{
//	void * symbol = NULL;
//	symbol = dlsym(handle, (char *)"JNI_OnLoad");
//	if (symbol)
//	{
//		LOGI("hook_symbol JNI_OnLoad addr . %x", symbol);
//		MSHookFunction(symbol, (void *)&new_JNI_OnLoad, (void **)&old_JNI_OnLoad);
//	}
//}

