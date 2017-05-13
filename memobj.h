#ifndef __MEMOBJ_H
#define __MEMOBJ_H

#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define MEMOBJ_NAME "luaL_MemObj"
#define MEMOBJ_TYPE 195

typedef struct {
	void* mem;
	size_t sz;
} memobj_t;

int lMemObj_Create(CLuaState* L);

int MemObj_Open(CLuaState* L);
int MemObj_Close(CLuaState* L);

#endif