#include "samurai.h"
#include "memobj.h"
#include "struct.h"

int lGetModuleHandle(CLuaState* L)
{
	L->CheckType(1,STRING);
	L->PushInteger((int)GetModuleHandleA(L->GetString(1)));
	return 1;
}

int lGetProcAddress(CLuaState* L)
{
	L->CheckType(1,NUMBER);
	L->CheckType(2,STRING);
	L->PushInteger((int)GetProcAddress((HMODULE)L->GetInteger(1),L->GetString(2)));
	return 1;
}

__declspec(naked) int VStdCall(int func,int stack) //StdCall
{
	__asm
	{
		push ebp
		mov ebp,esp

		mov eax,[func]
		mov esp,[stack]
		call eax
		mov esp,ebp

		pop ebp
		retn
	}
}

int lInvoke(CLuaState* L)
{
	DWORD dwFunc,dwData;
	int argnum;
	int* s_ptr;
	char stack[10240];

	s_ptr = (int*)&stack[10240];
	memset(stack,'\0',10240);

	L->CheckType(1,NUMBER);

	dwFunc = (DWORD)L->GetInteger(1);
	argnum = L->GetTop();

	for(int i = argnum; i != 1; i--)
	{
		int type = L->Type(i);
		switch(type)
		{
		case NUMBER:
			dwData = L->GetInteger(i);
			break;
		case STRING:
			dwData = (DWORD)L->GetString(i);
			break;
		case TBOOLEAN:
			dwData = (DWORD)L->GetBool(i);
			break;
		default:
			fprintf(stderr,"%d invalid type!\n",i);
		}
		s_ptr--;
		*s_ptr = dwData;
	}

	L->PushInteger(VStdCall(dwFunc,(int)s_ptr));
	return 1;
}

void Export2Table(CLuaState* L,const char* name)
{
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS NtHeader;
	PIMAGE_SECTION_HEADER SecHeader;
	PIMAGE_EXPORT_DIRECTORY Exports;
	PIMAGE_DATA_DIRECTORY Data;
	HMODULE hDll;
	DWORD dwLocal;
	char* text,*end,*len;
	char funcName[MAX_PATH];

	LPVOID lpBase = (LPVOID)GetModuleHandle(name);

	DosHeader = (PIMAGE_DOS_HEADER)lpBase;
	NtHeader = (PIMAGE_NT_HEADERS)((DWORD)lpBase + DosHeader->e_lfanew);
	SecHeader = IMAGE_FIRST_SECTION(NtHeader);
	Data = (PIMAGE_DATA_DIRECTORY)(&NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
    Exports = (PIMAGE_EXPORT_DIRECTORY)((DWORD)lpBase + Data->VirtualAddress);

	L->CreateTable();

	PDWORD pdwFunctions = (PDWORD)((DWORD)lpBase+Exports->AddressOfFunctions);
	const char** Names = (const char**)((DWORD)lpBase+Exports->AddressOfNames);
	unsigned short* ordinal = (unsigned short*)((DWORD)lpBase+Exports->AddressOfNameOrdinals);
	
	for(int i = 0; i < Exports->NumberOfNames; i++)
	{
		const char* name = ((DWORD)lpBase+Names[i]);
		unsigned short ord = ordinal[i];
		dwLocal = (DWORD)lpBase+pdwFunctions[ord];
		DWORD dwNew = (DWORD)GetProcAddress((HMODULE)lpBase,name);
		if(dwLocal >= (DWORD)Exports && dwLocal <= (DWORD)Exports + Data->Size)
		{
			text = (char*)dwLocal;
			len = (char*)(text+strlen(text));
			memset(funcName,'\0',MAX_PATH);
			if(!(end = (char*)strchr(text,'.')))
				continue;
			memcpy(funcName,text,(size_t)(end-text));
			strcat(funcName,".dll");
			if(!(hDll = GetModuleHandle(funcName)))
				continue;
			end++;
			memcpy(funcName,end,(len-end));
			dwLocal = (DWORD)GetProcAddress(hDll,funcName);
		}

		L->PushInteger(dwLocal);
		L->SetField(-2,name);
	}
}

int lExport2Table(CLuaState* L)
{
	L->CheckType(1,STRING);
	Export2Table(L,L->GetString(1));
	return 1;
}

int Samurai_Open(CLuaState* L)
{
	L->CreateTable();

	L->PushCFunction(lGetModuleHandle);
	L->SetField(-2,"GetModuleHandle");

	L->PushCFunction(lGetProcAddress);
	L->SetField(-2,"GetProcAddress");

	L->PushCFunction(lExport2Table);
	L->SetField(-2,"ExportTable");

	L->PushCFunction(lMemObj_Create);
	L->SetField(-2,"Alloc");

	L->PushCFunction(Struct_FromPointer);
	L->SetField(-2,"StructFromPointer");

	L->SetGlobal("samurai");

	L->PushCFunction(lInvoke);
	L->SetGlobal("invoke");

	Export2Table(L,"kernel32.dll");
	L->SetGlobal("kernel32");

	Export2Table(L,"user32.dll");
	L->SetGlobal("user32");

	L->PushInteger(0);
	L->SetGlobal("NULL");
	return 0;
}

int Samurai_Close(CLuaState* L)
{
	L->PushNil();
	L->SetGlobal("samurai");

	L->PushNil();
	L->SetGlobal("invoke");

	L->PushNil();
	L->SetGlobal("NULL");
	return 0;
}