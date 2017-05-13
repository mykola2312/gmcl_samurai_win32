#ifndef __STATE_H
#define __STATE_H

#include "lua.h"
#include <Windows.h>
#include <stdarg.h>
#include <varargs.h>

typedef struct {
	const char* name;
	void* ptr;
} func_t;

typedef struct {
	void* data;
	unsigned char type;
} userdata_t;

class CLuaState
{
public:
	CLuaState();
	~CLuaState();

	int GetTop();
	void SetTop(int n);

	void SetField(int idx,const char* name);
	void GetField(int idx,const char* name);

	inline void GetGlobal(const char* name){GetField(LUA_GLOBALSINDEX,name);}
	inline void SetGlobal(const char* name){SetField(LUA_GLOBALSINDEX,name);}
	inline void GetMetaFromRegistry(const char* name){GetField(LUA_REGISTRYINDEX,name);}

	void Push(int idx);
	void Pop(int amt = 1){SetTop(-(amt)-1);}

	void PushNil();
	void PushBool(bool val);
	void PushNumber(double num);
	void PushInteger(int n);
	void PushCClosure(CLuaFunction fn,int n);
	inline void PushCFunction(CLuaFunction fn){PushCClosure(fn,0);}
	void PushString(const char* str,size_t len = 0);
	void PushVString(const char* fmt,va_list ap);
	void PushFString(const char* fmt,...);
	void PushLightUserData(void* p);
	void PushThread(CLuaState* L);

	bool GetBool(int idx);
	double GetNumber(int idx);
	int GetInteger(int idx);
	const char* GetString(int idx,size_t* len = NULL);
	userdata_t* GetUserData(int idx);
	CLuaFunction GetCFunction(int idx);
	CLuaState* GetThread(int idx);
	const void* GetPointer(int idx);

	void CreateTable();
	void GetTable(int idx);
	void SetTable(int idx);
	
	void RawGet(int idx);
	void RawGeti(int idx,int n);
	void RawSet(int idx);
	void RawSeti(int idx,int n);

	void GetEnvironment(int idx);
	void SetEnvironment(int idx);

	userdata_t* NewUserData();
	void CreateMetaTable(const char* name);
	int GetMetaTable(int idx);
	int SetMetaTable(int idx);

	luatype_t Type(int idx);
	const char* TypeName(int idx);
	void CheckType(int idx,luatype_t tp);

	void Call(int nargs,int nret);
	int PCall(int nargs,int nret,int errfunc);

	int ObjLen(int idx);
	int ReferenceCreate();
	void ReferencePush(int refId);
	void ReferenceFree(int refId);

	void Print(const char*,...);
	const char* RunString(const char* name,const char* buf,size_t len);

	inline bool IsNil(int idx){return (Type(idx) == NIL);}
	inline bool IsBool(int idx){return (Type(idx) == TBOOLEAN);}
	inline bool IsLightUserData(int idx){return (Type(idx) == LIGHTUSERDATA);}
	inline bool IsNumber(int idx){return (Type(idx) == NUMBER);}
	inline bool IsString(int idx){return (Type(idx) == STRING);}
	inline bool IsTable(int idx){return (Type(idx) == TABLE);}
	inline bool IsFunction(int idx){return (Type(idx) == FUNCTION);}
	inline bool IsUserData(int idx){return (Type(idx) == USERDATA);}
	inline bool IsThread(int idx){return (Type(idx) == THREAD);}
};

#endif