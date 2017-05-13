#ifndef __SAMURAI_H
#define __SAMURAI_H

#include "interface.h"
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>

int Samurai_Open(CLuaState* L);
int Samurai_Close(CLuaState* L);

#endif