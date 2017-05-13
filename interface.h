#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "lua.h"
#include "state.h"

class CLuaInterface;

class CLuaShared
{
public:
	void** vtable;
	char tmp[88];
	CLuaInterface* m_pClient;
	CLuaInterface* m_pServer;
	CLuaInterface* m_pMenu;
};

class CLuaInterface
{
public:
	void** vtable;
	CLuaState* m_pState;
};

#ifndef DLL_EXPORT
#if defined(__GCC__)
#define DLL_EXPORT __attribute__((dllexport))
#elif defined (_MSC_VER)
#define DLL_EXPORT __declspec(dllexport)
#endif
#endif

#define GMOD_MODULE_OPEN() extern "C" DLL_EXPORT int gmod13_open(CLuaState* L)
#define GMOD_MODULE_CLOSE() extern "C" DLL_EXPORT int gmod13_close(CLuaState* L)

#endif