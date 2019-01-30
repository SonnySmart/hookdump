#ifndef myInclude_H
#define myInclude_H
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>
#include <dlfcn.h>
#include <sys/mman.h>
#ifdef __i386__

typedef struct JmpStruct{

	unsigned char code[20];
	uint32_t address;

} JmpStruct;

#else

typedef struct JmpStruct{

	unsigned char code[32];
	uint32_t address;

} JmpStruct;

#endif
#endif
