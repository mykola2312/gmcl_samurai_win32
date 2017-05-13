#include "state.h"

func_t lua_api[] = {
	{"lua_gettop",NULL},				//0
	{"lua_settop",NULL},				//1
	{"lua_pushvalue",NULL},				//2
	{"lua_tonumber",NULL},				//3
	{"lua_toboolean",NULL},				//4
	{"lua_tolstring",NULL},				//5
	{"lua_tocfunction",NULL},			//6
	{"lua_touserdata",NULL},			//7
	{"lua_tothread",NULL},				//8
	{"lua_topointer",NULL},				//9
	{"lua_pushnil",NULL},				//10
	{"lua_pushnumber",NULL},			//11
	{"lua_pushlstring",NULL},			//12
	{"lua_pushcclosure",NULL},			//13
	{"lua_pushboolean",NULL},			//14
	{"lua_pushlightuserdata",NULL},		//15
	{"lua_pushthread",NULL},			//16
	{"lua_gettable",NULL},				//17
	{"lua_getfield",NULL},				//18
	{"lua_rawget",NULL},				//19
	{"lua_rawgeti",NULL},				//20
	{"lua_createtable",NULL},			//21
	{"lua_getmetatable",NULL},			//22
	{"lua_newuserdata",NULL},			//23
	{"lua_getfenv",NULL},				//24
	{"lua_settable",NULL},				//25
	{"lua_setfield",NULL},				//26
	{"lua_rawset",NULL},				//27
	{"lua_rawseti",NULL},				//28
	{"lua_setmetatable",NULL},			//29
	{"lua_setfenv",NULL},				//30
	{"lua_call",NULL},					//31
	{"lua_pcall",NULL},					//32
	{"luaL_loadbufferx",NULL},			//33
	{"luaL_newmetatable",NULL},			//34
	{"lua_type",NULL},					//35
	{"lua_typename",NULL},				//36
	{"luaL_checktype",NULL},			//37
	{"lua_objlen",NULL},				//38
	{"lua_pushinteger",NULL},			//39
	{"lua_tointeger",NULL},				//40
	{NULL,NULL}
};

CLuaState::CLuaState()
{
}

CLuaState::~CLuaState()
{
}

int CLuaState::GetTop()
{
	return (((lua_gettop)lua_api[0].ptr))(this);
}

void CLuaState::SetTop(int n)
{
	(((lua_settop)lua_api[1].ptr))(this,n);
}

void CLuaState::SetField(int idx,const char* name)
{
	(((lua_setfield)lua_api[26].ptr))(this,idx,name);
}

void CLuaState::GetField(int idx,const char* name)
{
	(((lua_getfield)lua_api[18].ptr))(this,idx,name);
}

void CLuaState::Push(int idx)
{
	(((lua_pushvalue)lua_api[2].ptr))(this,idx);
}

void CLuaState::PushNil()
{
	(((lua_pushnil)lua_api[10].ptr))(this);
}

void CLuaState::PushBool(bool val)
{
	(((lua_pushboolean)lua_api[14].ptr))(this,val);
}

void CLuaState::PushNumber(double num)
{
	(((lua_pushnumber)lua_api[11].ptr))(this,num);
}

void CLuaState::PushInteger(int n)
{
	(((lua_pushinteger)lua_api[39].ptr))(this,n);
}

void CLuaState::PushCClosure(CLuaFunction fn,int n)
{
	(((lua_pushcclosure)lua_api[13].ptr))(this,fn,n);
}

void CLuaState::PushString(const char* str,size_t len)
{
	if(!len)
		len = strlen(str);
	(((lua_pushlstring)lua_api[12].ptr))(this,str,len);
}

void CLuaState::PushVString(const char* fmt,va_list ap)
{
	char buf[512];
	vsnprintf(buf,512,fmt,ap);
	PushString(buf);
}

void CLuaState::PushFString(const char* fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	PushVString(fmt,ap);
	va_end(ap);
}

void CLuaState::PushLightUserData(void* p)
{
	(((lua_pushlightuserdata)lua_api[15].ptr))(this,p);
}

void CLuaState::PushThread(CLuaState* L)
{
	(((lua_pushthread)lua_api[16].ptr))(L);
}

bool CLuaState::GetBool(int idx)
{
	return (((lua_toboolean)lua_api[4].ptr))(this,idx);
}

double CLuaState::GetNumber(int idx)
{
	return (((lua_tonumber)lua_api[3].ptr))(this,idx);
}

int CLuaState::GetInteger(int idx)
{
	return (((lua_tointeger)lua_api[40].ptr))(this,idx);
}

const char* CLuaState::GetString(int idx,size_t* len)
{
	return (((lua_tolstring)lua_api[5].ptr))(this,idx,len);
}

userdata_t* CLuaState::GetUserData(int idx)
{
	return (userdata_t*)(((lua_touserdata)lua_api[7].ptr))(this,idx);
}

CLuaFunction CLuaState::GetCFunction(int idx)
{
	return (((lua_tocfunction)lua_api[6].ptr))(this,idx);
}

CLuaState* CLuaState::GetThread(int idx)
{
	return (((lua_tothread)lua_api[8].ptr))(this,idx);
}

const void* CLuaState::GetPointer(int idx)
{
	return (((lua_topointer)lua_api[9].ptr))(this,idx);
}

void CLuaState::CreateTable()
{
	(((lua_createtable)lua_api[21].ptr))(this,0,0);
}

void CLuaState::GetTable(int idx)
{
	(((lua_gettable)lua_api[17].ptr))(this,idx);
}

void CLuaState::SetTable(int idx)
{
	(((lua_settable)lua_api[25].ptr))(this,idx);
}

void CLuaState::RawGet(int idx)
{
	(((lua_rawget)lua_api[19].ptr))(this,idx);
}

void CLuaState::RawGeti(int idx,int n)
{
	(((lua_rawgeti)lua_api[20].ptr))(this,idx,n);
}

void CLuaState::RawSet(int idx)
{
	(((lua_rawset)lua_api[27].ptr))(this,idx);
}

void CLuaState::RawSeti(int idx,int n)
{
	(((lua_rawseti)lua_api[28].ptr))(this,idx,n);
}

void CLuaState::GetEnvironment(int idx)
{
	(((lua_getfenv)lua_api[24].ptr))(this,idx);
}

void CLuaState::SetEnvironment(int idx)
{
	(((lua_setfenv)lua_api[30].ptr))(this,idx);
}

userdata_t* CLuaState::NewUserData()
{
	return (userdata_t*)(((lua_newuserdata)lua_api[23].ptr))(this,sizeof(userdata_t));
}

void CLuaState::CreateMetaTable(const char* name)
{
	(((luaL_newmetatable)lua_api[34].ptr))(this,name);
}

int CLuaState::GetMetaTable(int idx)
{
	return (((lua_getmetatable)lua_api[22].ptr))(this,idx);
}

int CLuaState::SetMetaTable(int idx)
{
	return (((lua_setmetatable)lua_api[29].ptr))(this,idx);
}

luatype_t CLuaState::Type(int idx)
{
	return (luatype_t)(((lua_type)lua_api[35].ptr))(this,idx);
}

const char* CLuaState::TypeName(int idx)
{
	return (((lua_typename)lua_api[36].ptr))(this,Type(idx));
}

void CLuaState::CheckType(int idx,luatype_t tp)
{
	(((luaL_checktype)lua_api[37].ptr))(this,idx,tp);
}

void CLuaState::Call(int nargs,int nret)
{
	(((lua_call)lua_api[31].ptr))(this,nargs,nret);
}

int CLuaState::PCall(int nargs,int nret,int errfunc)
{
	return (((lua_pcall)lua_api[32].ptr))(this,nargs,nret,errfunc);
}

void CLuaState::Print(const char* msg,...)
{
	GetGlobal("print");
	va_list ap;
	va_start(ap,msg);
	PushVString(msg,ap);
	va_end(ap);
	Call(1,0);
}

int CLuaState::ObjLen(int idx)
{
	return (((lua_objlen)lua_api[38].ptr))(this,idx);
}

int CLuaState::ReferenceCreate()
{
	int len = ObjLen(-1);
	len++;
	PushInteger(len);
	Push(-2);
	SetTable(LUA_REGISTRYINDEX);
	Pop();
	return len;
}

void CLuaState::ReferencePush(int refId)
{
	PushInteger(refId);
	GetTable(LUA_REGISTRYINDEX);
}

void CLuaState::ReferenceFree(int refId)
{
	PushInteger(refId);
	PushNil();
	SetTable(LUA_REGISTRYINDEX);
}

const char* CLuaState::RunString(const char* name,const char* buf,size_t len)
{
	const char* err = NULL;
	if( (((luaL_loadbufferx)lua_api[33].ptr))(this,buf,len,name,NULL) != OK)
	{
		err = GetString(-1);
		Pop();
	}
	else
		PCall(0,0,0);
	return err;
}

BOOL WINAPI ConnectLuaShared()
{
	HMODULE hLuaShared = NULL;
	char szDebugInfo[128];
	if(!(hLuaShared = GetModuleHandle("lua_shared.dll")))
		return FALSE;
	for(func_t* i = lua_api; i->name; i++)
	{
		if(!(i->ptr = (void*)GetProcAddress(hLuaShared,i->name)))
		{
			_snprintf(szDebugInfo,128,"GetProcAddress failed at %s",i->name);
			MessageBoxA(NULL,szDebugInfo,"ConnectLuaShared",MB_ICONHAND);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hDll,DWORD fdwReason,LPVOID lpReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
		return ConnectLuaShared();
	return TRUE;
}