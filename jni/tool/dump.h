/*
 * dump.h
 *
 *  Created on: 2019-1-4
 *      Author: Administrator
 */

#ifndef DUMP_H_
#define DUMP_H_

#include <vector>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

void* get_remote_addr(int target_pid, const char* module_name, void* local_addr);
void* get_module_base(int pid, const char* module_name);
void* lookup_symbol(char* libraryname,char* symbolname);
// dump lua
int dump_write(const char *path, const char *buff, const size_t &len);
// replace lua
int replace_buffer(const char *name, const std::vector<std::string> &r, void *&out_buffer, size_t &out_len);

#ifdef __cplusplus
}
#endif

#endif /* DUMP_H_ */
