LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)
#LOCAL_MODULE := Hooker
#LOCAL_SRC_FILES := $(LOCAL_PATH)/SNativeHook/obj/local/$(TARGET_ARCH_ABI)/libHooker.a \
#					$(LOCAL_PATH)/SNativeHook/obj/local/$(TARGET_ARCH_ABI)/libstdc++.a
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := dump
LOCALC_INCLUDES := $(LOCAL_PATH)/dump \
					$(LOCAL_PATH)/cocos
LOCAL_SRC_FILES := dump/dump.cpp \
					inject.cpp \
					cocos.cpp \
					cocos/CCData.cpp
#LOCAL_LDLIBS := -lm -llog
#LOCAL_CFLAGS := -std=c99
LOCAL_ARM_MODE := arm
LOCAL_SHARED_LIBRARIES := liblog libcutils

LOCAL_LDLIBS := $(LOCAL_PATH)/SNativeHook/obj/local/$(TARGET_ARCH_ABI)/libHooker.a \
				$(LOCAL_PATH)/SNativeHook/obj/local/$(TARGET_ARCH_ABI)/libstdc++.a
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)