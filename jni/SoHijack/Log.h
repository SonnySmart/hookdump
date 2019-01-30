#ifndef LOG_H
#define LOG_H

#define JNI_DEBUG 1

#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "zeif", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "zeif", __VA_ARGS__)
#define LOGD(...) __android_log_print(3, "zeif", __VA_ARGS__)

#define JNI_ASSERT_P_RTN_VAR(var, rtn)\
 	if (NULL == var)\
	{\
		LOGD("[C]-File:%s, Line:%d, %s==NULL/n", __FILE__, __LINE__, #var);\
		return rtn;\
	}

#define JNI_ASSERT_V_RTN_VAR(var, rtn)\
 	if (0 == var)\
	{\
		LOGD("[C]-File:%s, Line:%d, %s==NULL/n", __FILE__, __LINE__, #var);\
		return rtn;\
	}



#endif
