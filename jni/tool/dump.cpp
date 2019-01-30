/*
 * dump.c
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#include "dump.h"
#include "define.h"

#ifdef __cplusplus
extern "C"{
#endif

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

void* lookup_symbol(char* libraryname,char* symbolname)
{
    void *handle = dlopen(libraryname, RTLD_GLOBAL | RTLD_NOW);
    if (handle != NULL){
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

void mkdirs(const std::string &dir)
{
    int i,len;
    len = dir.length();
    char *str = (char *)malloc(len);
    strcpy(str, dir.c_str());
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
    free(str);
}

int dump_write(const char *path, const char *buff, const size_t &len)
{
	if (!buff || len <= 0)
		return -1;

	if (access(path, F_OK) == 0)
		return -1;

	//dump .dll .lua
	if (strstr(path, ".lua") == NULL &&
			strstr(path, ".dll") == NULL
			)
		return -1;

	std::string tmp(path);
	std::size_t i = tmp.find_last_of("/");
	if (i != std::string::npos)
	{
		std::string dir = tmp.substr(0, i);
		if (access(dir.c_str(), F_OK) != 0) {
			mkdirs(dir);
		}
	}

	FILE *fd = fopen(path, "wb");
	if (fd) {
		if (fwrite(buff, len, 1u, fd) > 0) {
			LOGI("fwrite:%s", path);
			fclose(fd);
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

int replace_buffer(const char *name, const vector<string> &r, void *&out_buffer, size_t &out_len)
{
	if (strlen(name) == 0 || r.size() ==0)
		return -1;

	for(auto i = r.begin(); i != r.end(); ++i) {
		const char *filename = (*i).c_str();

		const char *fullpath = get_sdcard_fullpath(filename);

		if (access(fullpath, F_OK) != 0 || strstr(name, filename) == NULL)
			continue;

		LOGI("check success path:%s", fullpath);

		FILE *fd = NULL;
		if ((fd = fopen(fullpath, "rb"))) {
			fseek(fd, 0, SEEK_END);
			size_t len = ftell(fd);
			if (len <= 0) {
				fclose(fd);
				LOGE("file len <= 0");
				continue;
			}
			fseek(fd, 0, SEEK_SET);
			void *buffer = malloc(len);

			if (fread(buffer, 1u, len, fd) <= 0) {
				fclose(fd);
				LOGE("file fread <= 0");
				continue;
			}

			LOGI("read buffer:%x", buffer);

			out_buffer = buffer;
			out_len = len;

			fclose(fd);

			return 0;
		}
	}

	return -1;
}

#ifdef __cplusplus
}
#endif

