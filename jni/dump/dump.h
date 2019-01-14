/*
 * dump.h
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#ifndef DUMP_H_
#define DUMP_H_

#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "../SNativeHook/jni/Hooker.h"

#define TAG "hookdump"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

//����������� android inject ���ڻ�ȡ��ַ
void* get_remote_addr(int target_pid, const char* module_name, void* local_addr);
//����������� android inject ���ڻ�ȡ��ַ
void* get_module_base(int pid, const char* module_name);
//ͨ��so��ľ���·���ͺ��������ҵ��亯����ӳ���ַ
void* lookup_symbol(char* libraryname,char* symbolname);
//�����༶Ŀ¼
void mkdirs(char *muldir);
//�ַ�����ȡ
char* substring(char* ch,int pos,int length);
//��������ַ���
int rfind(const char*source ,const char* match);
//д���ļ�
int dump_write(const char *path, const char *buff, const size_t len, bool write = true);

#ifdef __cplusplus
}
#endif

#endif /* DUMP_H_ */
