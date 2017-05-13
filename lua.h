#ifndef __LUA_H
#define __LUA_H

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)

class CLuaState;
typedef int (*CLuaFunction)(CLuaState*);

typedef int             (*lua_type) (CLuaState *L, int idx);
typedef const char     *(*lua_typename) (CLuaState *L, int tp);

typedef int   (*lua_gettop) (CLuaState *L);
typedef void  (*lua_settop) (CLuaState *L, int idx);
typedef void  (*lua_pushvalue) (CLuaState *L, int idx);

typedef double      (*lua_tonumber) (CLuaState *L, int idx);
typedef int             (*lua_toboolean) (CLuaState *L, int idx);
typedef const char     *(*lua_tolstring) (CLuaState *L, int idx, size_t *len);
typedef CLuaFunction   (*lua_tocfunction) (CLuaState *L, int idx);
typedef void	       *(*lua_touserdata) (CLuaState *L, int idx);
typedef CLuaState      *(*lua_tothread) (CLuaState *L, int idx);
typedef const void     *(*lua_topointer) (CLuaState *L, int idx);

typedef void  (*lua_pushnil) (CLuaState *L);
typedef void  (*lua_pushnumber) (CLuaState *L, double n);
typedef void  (*lua_pushlstring) (CLuaState *L, const char *s, size_t l);
typedef void  (*lua_pushcclosure) (CLuaState *L, CLuaFunction fn, int n);
typedef void  (*lua_pushboolean) (CLuaState *L, int b);
typedef void  (*lua_pushlightuserdata) (CLuaState *L, void *p);
typedef int   (*lua_pushthread) (CLuaState *L);

typedef void  (*lua_gettable) (CLuaState *L, int idx);
typedef void  (*lua_getfield) (CLuaState *L, int idx, const char *k);
typedef void  (*lua_rawget) (CLuaState *L, int idx);
typedef void  (*lua_rawgeti) (CLuaState *L, int idx, int n);
typedef void  (*lua_createtable) (CLuaState *L, int narr, int nrec);
typedef void *(*lua_newuserdata) (CLuaState *L, size_t sz);
typedef int   (*lua_getmetatable) (CLuaState *L, int objindex);
typedef void  (*lua_getfenv) (CLuaState *L, int idx);

typedef void  (*lua_settable) (CLuaState *L, int idx);
typedef void  (*lua_setfield) (CLuaState *L, int idx, const char *k);
typedef void  (*lua_rawset) (CLuaState *L, int idx);
typedef void  (*lua_rawseti) (CLuaState *L, int idx, int n);
typedef int   (*lua_setmetatable) (CLuaState *L, int objindex);
typedef int   (*lua_setfenv) (CLuaState *L, int idx);

typedef void  (*lua_call) (CLuaState *L, int nargs, int nresults);
typedef int   (*lua_pcall) (CLuaState *L, int nargs, int nresults, int errfunc);

typedef int (*luaL_loadbufferx)(CLuaState *L, const char *buff,
	size_t sz,const char *name,const char* mode);
typedef int   (*luaL_newmetatable) (CLuaState *L, const char *tname);
typedef void (*luaL_checktype) (CLuaState *L, int narg, int t);
typedef size_t          (*lua_objlen) (CLuaState *L, int idx);
typedef void  (*lua_pushinteger)(CLuaState *L, int n);
typedef int (*lua_tointeger)(CLuaState* L,int idx);

typedef enum {
	OK = 0,
	YIELD,
	ERRRUN,
	ERRSYNTAX,
	ERRMEM,
	ERRERR
} ret_t;

typedef enum {
	NONE = -1,
	NIL,
	TBOOLEAN,
	LIGHTUSERDATA,
	NUMBER,
	STRING,
	TABLE,
	FUNCTION,
	USERDATA,
	THREAD,
} luatype_t;

#endif