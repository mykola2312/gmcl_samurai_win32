# gmcl_samurai_win32
This module allows you to call **__stdcall/__cdecl** functions by pointer from **Lua**

# Functions:

handle = **samurai.GetModuleHandle**("user32.dll") - Get DLL HMODULE (return value caster to number)

func = **samurai.GetProcAddress**(handle,"MessageBoxA") - GetProcAddress, handle is a number (32 bit)

api = **samurai.ExportTable**("ntdll.dll") - Walks through dll's export table and pushes function in returned table

obj = **samurai.Alloc**(size) - Allocate memory, returns memobj metatable

stc = **samurai.StructFromPointer**(ptr) - Makes a struct metatable, ptr is a pointer (casted to number)

n = **invoke**(FuncPointer,...) - Call __cdecl/__stdcall by pointer (number), return value is integer, stored in eax (aka func's return value)

# MemObj Metatable:

You can get this object by samurai.Alloc(n)

obj = **samurai.Alloc**(292)

## Metamethods

**obj:Free**() - Free allocated memory

**obj:ToString**() - Returns memory as string

**obj:ToStruct**() - Returns **struct** metatable

**obj:ToPointer()** - Returns mem ptr as number

# Struct Metatable:

You can get this object by casting from memobj

stc = **obj:ToStruct**()

Or by casting from pointer

stc = **samurai.StructFromPointer**(ptr)

Struct organized as stack, which start from mem ptr

## Metamethods

**stc:ResetPtr**() - Reset s_ptr (aka stack pointer) to default value (mem ptr)

n = **stc:GetOffset**() - Get length from s_ptr and mem ptr, aka struct offset

**stc:SetOffset**(n) - Directly set stack pointer (s_ptr = mem ptr + offset)

**stc:Skip**(n) - Skip n bytes, add to offset n

**stc:PushString**(str) - Write string to s_ptr and add offset - string len + 1

**stc:PushNumber**(sz,n) - Which sz is (BYTE/WORD/DWORD) and n - number; Write number and his length to offset

str = **stc:GetStringFromPtr**() - return string by pointer from s_ptr and add 4 (DWORD size) to offset (aka char*)

str = **stc:GetStringFixed**(sz) - returns string from s_ptr and add to offset sz (string with fixed size aka char str[256])

str = **stc:GetString**() - Same as **GetStringFixed**, but sz is = strlen(str) (aka char str[?])

n = **stc:GetNumber**(sz) - Return number at s_ptr and add to offset sz (which BYTE/WORD/DWORD)

ptr = **stc:ToPointer**() - Return mem ptr

## Practical usage

You need set struct like this

```lua
typedef struct {
  int a;
  int b;
  int c
} something_t;

just

mem = samurai.Alloc(12) -- sizeof(int)*3
stc = mem:ToStruct()
stc:ResetPtr()
stc:PushNumber(DWORD,1)
stc:PushNumber(DWORD,2)
stc:PushNumber(DWORD,3)

--Or parse

a = stc:GetNumber(DWORD)
b = stc:GetNumber(DWORD)
c = stc:GetNumber(DWORD)

mem:Free() -- Don't forget to free memory!
```

# Global

When module loading, in global table (_G), samurai makes a few tables: kernel32,user32

Which filled by function pointers from kernel32.dll and user32.dll

n = **invoke**(FuncPointer,...) - Call __cdecl/__stdcall by pointer (number), return value is integer, stored in eax (aka func's return value)

Some global values:

```
NULL = 0
BYTE = 1
WORD = 2
DWORD = 4
```

# Post Scriptum

See example in inject.lua

Also this module writed with custom lua interface (**see in repo gmcl_jack_win32**)
