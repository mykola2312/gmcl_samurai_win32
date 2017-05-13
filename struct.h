#ifndef __STRUCT_H
#define __STRUCT_H

#define STRUCT_NAME "luaL_Struct"
#define STRUCT_TYPE 196

#include "interface.h"
#include "memobj.h"

typedef struct {
	memobj_t* page;
	char* s_ptr;
	bool bFree;
} struct_t;

void Struct_Create(CLuaState* L,memobj_t* obj);
int Struct_FromPointer(CLuaState* L);

int Struct_Open(CLuaState* L);
int Struct_Close(CLuaState* L);

#endif