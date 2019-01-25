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

static JavaVM* g_vm = NULL;
static std::string g_package;
static std::string g_sdcard_root;

extern jboolean native_call_java(const char *method, const char *sig, std::string &ret);

const char *getPackageString()
{
	std::string ret("");
	native_call_java(__FUNCTION__, "()Ljava/lang/String;", ret);
	return ret.c_str();
}

const char *getSdcardDirString()
{
	std::string ret("");
	native_call_java(__FUNCTION__, "()Ljava/lang/String;", ret);
	return ret.c_str();
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

jboolean native_call_java(const char *method, const char *sig, std::string &ret)
{
	JNIEnv *env  =  NULL;
	jclass clazz = NULL;
	jmethodID mid = NULL;

	if (g_vm == NULL)
		return JNI_FALSE;

	if ( g_vm->AttachCurrentThread(&env, NULL) != JNI_OK) {
		LOGE("native_call_java AttachCurrentThread != JNI_OK");
		return JNI_FALSE;
	}

	if ((clazz = env->FindClass(JNI_CLASS)) == NULL) {
		LOGE("native_call_java clazz null");
		return JNI_FALSE;
	}

	if ((mid = env->GetStaticMethodID( clazz, method, sig)) == NULL) {
		LOGE("native_call_java mid null");
		return JNI_FALSE;
	}

	jstring js = (jstring)env->CallStaticObjectMethod(clazz, mid);
	char *str = (char *)env->GetStringUTFChars(js, 0);
	ret = str;
	env->ReleaseStringUTFChars(js, 0);

	//destory
	//env->DeleteLocalRef(clazz);
	//g_vm->DetachCurrentThread();

	return JNI_TRUE;
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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	g_vm = vm;

	JNIEnv *env = NULL;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return JNI_ERR;
	}
	assert(env != NULL);

	LOGD("JNI_OnLoad env has . %x", env);

	if (!registerNatives(env)) {
		LOGE("JNI_OnLoad registerNatives FALSE");
		return JNI_ERR;
	}

	return JNI_VERSION_1_4;
}

