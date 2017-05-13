#include "struct.h"

void Struct_Create(CLuaState* L,memobj_t* obj)
{
	struct_t* stc = new struct_t;
	stc->page = obj;
	stc->s_ptr = (char*)stc->page->mem;
	stc->bFree = false;

	userdata_t* pData = L->NewUserData();
	pData->data = (void*)stc;
	pData->type = STRUCT_TYPE;

	L->GetMetaFromRegistry(STRUCT_NAME);
	L->SetMetaTable(-2);
}

int Struct_FromPointer(CLuaState* L)
{
	struct_t* stc = new struct_t;
	L->CheckType(1,NUMBER);

	stc->page = new memobj_t;
	stc->s_ptr = (char*)stc->page->mem;
	stc->bFree = false;

	userdata_t* pData = L->NewUserData();
	pData->data = (void*)stc;
	pData->type = STRUCT_TYPE;

	L->GetMetaFromRegistry(STRUCT_NAME);
	L->SetMetaTable(-2);
	return 1;
}

struct_t* Check_Struct(CLuaState* L,int idx)
{
	userdata_t* pData = L->GetUserData(idx);
	if(pData->type == STRUCT_TYPE)
		return (struct_t*)pData->data;
	else
		return NULL;
}

int lStruct_tostr(CLuaState* L)
{
	struct_t* stc = Check_Struct(L,1);
	L->PushFString("struct mem %p s_ptr %p size %d",stc->page->mem,stc->s_ptr,stc->page->sz);
	return 1;
}

int lStruct_eq(CLuaState* L)
{
	struct_t* st1 = Check_Struct(L,1),*st2 = Check_Struct(L,2);
	L->PushBool(((st1->page == st2->page) && (st1->s_ptr == st2->s_ptr)));
	return 1;
}

int lStruct_gc(CLuaState* L)
{
	struct_t* stc = Check_Struct(L,1);
	if(stc->bFree && stc->page->mem)
		free(stc->page);
	return 0;
}

int lStruct_ResetPtr(CLuaState* L)
{
	struct_t* stc = Check_Struct(L,1);
	stc->s_ptr = (char*)stc->page->mem;
	return 0;
}

int lStruct_GetOffset(CLuaState* L)
{
	struct_t* stc = Check_Struct(L,1);
	L->PushInteger((int)(stc->s_ptr-(char*)stc->page->mem));
	return 1;
}

int lStruct_SetOffset(CLuaState* L)
{
	L->CheckType(2,NUMBER);
	struct_t* stc = Check_Struct(L,1);
	stc->s_ptr=(char*)L->GetInteger(2);
	return 0;
}

int lStruct_Skip(CLuaState* L)
{
	L->CheckType(2,NUMBER);
	struct_t* stc = Check_Struct(L,1);
	stc->s_ptr=(char*)(stc->s_ptr+L->GetInteger(2));
	return 0;
}

int lStruct_PushString(CLuaState* L)
{
	L->CheckType(2,STRING);
	struct_t* stc = Check_Struct(L,1);

	const char* str = L->GetString(2);
	size_t len = strlen(str) + 1;

	memcpy(stc->s_ptr,str,len);
	stc->s_ptr = (char*)(stc->s_ptr+len);

	return 0;
}

int lStruct_PushNumber(CLuaState* L)
{
	L->CheckType(2,NUMBER);
	L->CheckType(3,NUMBER);
	struct_t* stc = Check_Struct(L,1);

	size_t len = L->GetNumber(3);
	int data = L->GetInteger(2);

	if(len == 1)
		*(unsigned char*)stc->s_ptr = (unsigned char)data;
	else if(len == 2)
		*(unsigned short*)stc->s_ptr = (unsigned short)data;
	else if(len == 4)
		*(unsigned int*)stc->s_ptr = (unsigned int)data;
	stc->s_ptr = (char*)(stc->s_ptr+len);

	return 0;
}

int lStruct_GetStringFixed(CLuaState* L)
{
	L->CheckType(2,NUMBER);
	struct_t* stc = Check_Struct(L,1);

	size_t len = L->GetInteger(2);
	L->PushString(stc->s_ptr);
	stc->s_ptr = (char*)(stc->s_ptr+len);

	return 1;
}

int lStruct_GetString(CLuaState* L)
{
	struct_t* stc = Check_Struct(L,1);
	
	size_t len = strlen(stc->s_ptr)+1;
	L->PushString(stc->s_ptr);
	stc->s_ptr = (char*)(stc->s_ptr+len);

	return 1;
}

int lStruct_GetNumber(CLuaState* L)
{
	L->CheckType(2,NUMBER);
	struct_t* stc = Check_Struct(L,1);

	size_t len = L->GetInteger(2);
	int data;

	if(len == 1)
		data = stc->s_ptr[0];
	else if(len == 2)
		data = *(unsigned short*)stc->s_ptr;
	else if(len == 4)
		data = *(unsigned int*)stc->s_ptr;
	L->PushInteger(data);
	stc->s_ptr = (char*)(stc->s_ptr+len);
	return 1;
}

int lStruct_ToPointer(CLuaState* L)
{
	struct_t* stc = Check_Struct(L,1);
	L->PushInteger((int)stc->page->mem);
	return 1;
}

int Struct_Open(CLuaState* L)
{
	L->PushNumber(1);
	L->SetGlobal("BYTE");

	L->PushNumber(2);
	L->SetGlobal("WORD");

	L->PushNumber(4);
	L->SetGlobal("DWORD");

	L->CreateMetaTable(STRUCT_NAME);

	L->PushCFunction(lStruct_eq);
	L->SetField(-2,"__eq");

	L->PushCFunction(lStruct_gc);
	L->SetField(-2,"__gc");

	L->PushCFunction(lStruct_tostr);
	L->SetField(-2,"__tostring");

	L->PushCFunction(lStruct_ResetPtr);
	L->SetField(-2,"ResetPtr");

	L->PushCFunction(lStruct_GetOffset);
	L->SetField(-2,"GetOffset");

	L->PushCFunction(lStruct_SetOffset);
	L->SetField(-2,"SetOffset");

	L->PushCFunction(lStruct_Skip);
	L->SetField(-2,"Skip");

	L->PushCFunction(lStruct_PushString);
	L->SetField(-2,"PushString");

	L->PushCFunction(lStruct_PushNumber);
	L->SetField(-2,"PushNumber");

	L->PushCFunction(lStruct_GetStringFixed);
	L->SetField(-2,"GetStringFixed");

	L->PushCFunction(lStruct_GetString);
	L->SetField(-2,"GetString");

	L->PushCFunction(lStruct_GetNumber);
	L->SetField(-2,"GetNumber");

	L->PushCFunction(lStruct_ToPointer);
	L->SetField(-2,"ToPointer");

	L->Push(-1);
	L->SetField(-2,"__index");
	return 0;
}

int Struct_Close(CLuaState* L)
{
	L->PushNil();
	L->SetGlobal("BYTE");

	L->PushNil();
	L->SetGlobal("WORD");

	L->PushNil();
	L->SetGlobal("DWORD");
	return 0;
}