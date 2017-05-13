#include "memobj.h"
#include "struct.h"

int lMemObj_Create(CLuaState* L)
{
	L->CheckType(1,NUMBER);

	memobj_t* obj = new memobj_t;
	obj->sz = L->GetInteger(1);
	obj->mem = (void*)malloc(obj->sz);

	userdata_t* pData = L->NewUserData();
	pData->data = obj;
	pData->type = MEMOBJ_TYPE;

	L->GetMetaFromRegistry(MEMOBJ_NAME);
	L->SetMetaTable(-2);

	return 1;
}

memobj_t* MemObj_Check(CLuaState* L,int idx)
{
	userdata_t* pData = L->GetUserData(idx);
	if(pData->type == MEMOBJ_TYPE)
		return (memobj_t*)pData->data;
	else
		return NULL;
}

int lMemObj_tostr(CLuaState* L)
{
	memobj_t* obj = MemObj_Check(L,1);
	if(!obj)
		return 0;
	else
		L->PushFString("memobj_t mem %p size %d",obj->mem,obj->sz);
	return 1;
}

int lMemObj_eq(CLuaState* L)
{
	memobj_t* obj1 = MemObj_Check(L,1),*obj2 = MemObj_Check(L,2);
	L->PushBool(((obj1->mem == obj2->mem) && (obj1->sz == obj2->sz)));
	return 1;
}

int lMemObj_gc(CLuaState *L)
{
	memobj_t* obj = MemObj_Check(L,1);
	if(obj->mem)
		free(obj->mem);
	delete obj;
	return 0;
}

int lMemObj_ToString(CLuaState* L)
{
	memobj_t* obj = MemObj_Check(L,1);
	L->PushString((const char*)obj->mem,obj->sz);
	return 1;
}

int lMemObj_ToStruct(CLuaState* L)
{
	memobj_t* obj = MemObj_Check(L,1);
	Struct_Create(L,obj);
	return 1;
}

int lMemObj_ToPointer(CLuaState* L)
{
	memobj_t* obj = MemObj_Check(L,1);
	L->PushInteger((int)obj->mem);
	return 1;
}

int lMemObj_Free(CLuaState* L)
{
	memobj_t* obj = MemObj_Check(L,1);
	if(obj->mem)
	{
		free(obj->mem);
		obj->mem = NULL;
	}
	return 0;
}

int MemObj_Open(CLuaState* L)
{
	L->CreateMetaTable(MEMOBJ_NAME);

	L->PushCFunction(lMemObj_tostr);
	L->SetField(-2,"__tostring");

	L->PushCFunction(lMemObj_eq);
	L->SetField(-2,"__eq");

	L->PushCFunction(lMemObj_gc);
	L->SetField(-2,"__gc");

	L->PushCFunction(lMemObj_Free);
	L->SetField(-2,"Free");

	L->PushCFunction(lMemObj_ToString);
	L->SetField(-2,"ToString");

	L->PushCFunction(lMemObj_ToStruct);
	L->SetField(-2,"ToStruct");

	L->PushCFunction(lMemObj_ToPointer);
	L->SetField(-2,"ToPointer");

	L->Push(-1);
	L->SetField(-2,"__index");
	return 0;
}
int MemObj_Close(CLuaState* L)
{
	return 0;
}