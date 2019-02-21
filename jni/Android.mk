LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := inject
LOCALC_INCLUDES := $(LOCAL_PATH)/tool \
					$(LOCAL_PATH)/unity
					
LOCAL_SRC_FILES := tool/dump.cpp \
					tool/jnibridge.cpp \
					inject.cpp \
					cocos.cpp \
					unity.cpp \
					libc.cpp \
					tersafe.cpp
					
#LOCAL_LDLIBS := -lm -llog
#LOCAL_CFLAGS := -std=c99
LOCAL_ARM_MODE := arm
LOCAL_SHARED_LIBRARIES := liblog libcutils

LOCAL_LDLIBS := $(LOCAL_PATH)/SNativeHook/obj/local/$(TARGET_ARCH_ABI)/libHooker.a \
				$(LOCAL_PATH)/SNativeHook/obj/local/$(TARGET_ARCH_ABI)/libstdc++.a
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libtersafe
LOCAL_SRC_FILES := unity/libtersafe.so
include $(PREBUILT_SHARED_LIBRARY)