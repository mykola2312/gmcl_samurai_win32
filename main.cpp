#include <Windows.h>
#include "interface.h"
#include "memobj.h"
#include "struct.h"
#include "samurai.h"

GMOD_MODULE_OPEN()
{
	MemObj_Open(L);
	Struct_Open(L);
	Samurai_Open(L);
	return 0;
}

GMOD_MODULE_CLOSE()
{
	MemObj_Close(L);
	Struct_Close(L);
	Samurai_Close(L);
	return 0;
}