/*
 * dump.c
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#include "dump.h"

#ifdef __cplusplus
extern "C"{
#endif

//����������� android inject ���ڻ�ȡ��ַ
void* get_remote_addr(int target_pid, const char* module_name, void* local_addr)
{
    void* local_handle, *remote_handle;

    local_handle = get_module_base(-1, module_name);
    remote_handle = get_module_base(target_pid, module_name);

    LOGI("[+] get_remote_addr: local[%x], remote[%x]\n", local_handle, remote_handle);

    void * ret_addr = (void *)((uint32_t)local_addr + (uint32_t)remote_handle - (uint32_t)local_handle);

#if defined(__i386__)
    if (!strcmp(module_name, "/system/lib/libc.so")) {
        ret_addr += 2;
    }
#endif
    return ret_addr;
}

//����������� android inject ���ڻ�ȡ��ַ
void* get_module_base(int pid, const char* module_name)
{
    FILE *fp;
    long addr = 0;
    char *pch;
    char filename[32];
    char line[1024];
    if (pid < 0) {
        /* self process */
        snprintf(filename, sizeof(filename), "/proc/self/maps", pid);
    }
    else {
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    }
    fp = fopen(filename, "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, module_name)) {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);

                if (addr == 0x8000)
                    addr = 0;

                break;
            }
        }
        fclose(fp);
    }
    return (void *)addr;
}

//ͨ��so��ľ���·���ͺ��������ҵ��亯����ӳ���ַ
void* lookup_symbol(char* libraryname,char* symbolname)
{
    //��ȡso��ľ��
    void *handle = dlopen(libraryname, RTLD_GLOBAL | RTLD_NOW);
    if (handle != NULL){
        //����so�����ͷ�������������������ȡ������ַ
        void * symbol = dlsym(handle, symbolname);
        if (symbol != NULL){
            return symbol;
        }else{
            LOGD("symbol not find . %s", dlerror());
            return NULL;
        }
    }
    else
    {
    	LOGD("handle not find .");
        return NULL;
    }
}

//�����༶Ŀ¼
void mkdirs(char *muldir)
{
    int i,len;
    char str[512];
    strncpy(str, muldir, 512);
    len=strlen(str);
    for( i=0; i<len; i++ )
    {
        if( str[i]=='/' )
        {
            str[i] = '\0';
            if( access(str,0)!=0 )
            {
                mkdir( str, 0777 );
            }
            str[i]='/';
        }
    }
    if( len>0 && access(str,0)!=0 )
    {
        mkdir( str, 0777 );
    }
    return;
}

//�ַ�����ȡ
char* substring(char* ch,int pos,int length)
{
    //�����ַ�ָ�� ָ�򴫵ݽ�����ch��ַ
    char* pch=ch;
    //ͨ��calloc������һ��length���ȵ��ַ����飬���ص����ַ�ָ�롣
    char* subch=(char*)calloc(sizeof(char),length+1);
    int i;
    //ֻ����C99��forѭ���вſ�����������������д�����棬��߼����ԡ�
    pch=pch+pos;
    //��pchָ��ָ��posλ�á�
    for(i=0;i<length;i++)
    {
        subch[i]=*(pch++);
    //ѭ��������ֵ���顣
    }
    subch[length]='\0';//�����ַ�����������
    return subch;       //���ط�����ַ������ַ��
}

//��������ַ���
int rfind(const char*source ,const char* match)
{
    // for(int i=strlen(source);i>=0;i--)
     for(int i=strlen(source)-strlen(match)-1;i>=0;i--) //ԭ���Ĵ����е�С���⡣
     {
           if(source[i]==match[0] && strncmp(source+i, match, strlen(match))==0) return i;
     }
      return -1;
}

//д���ļ�
int dump_write(const char *path, const char *buff, const size_t len, bool write)
{
	if (!buff || len <= 0)
		return -1;

	if (write && access(path, F_OK) == 0)
		return -1;

	//�������ļ��оʹ���
	int i = rfind(path, "/");
	if (i != -1) {
		char *des_ = (char *)path;
		char *dir = substring(des_, 0, i);
		//LOGD("dir:%s", dir);
		if (access(path, F_OK) != 0) {
			mkdirs(dir);
		}
	}

	//д��Ŀ¼
	FILE *fd = fopen(path, "wb");
	if (fd) {
		if (fwrite(buff, len, 1u, fd) > 0) {
			LOGI("fwrite:%s", path);
			return 0;
		}
		else {
			LOGE("fwrite failed:%x", fd);
		}
		fclose(fd);
	}
	else {
		LOGE("fopen failed:%s", path);
	}

	return -1;
}

#ifdef __cplusplus
}
#endif

