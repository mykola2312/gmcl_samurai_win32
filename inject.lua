--Struct size 292
local fullPath = "D:\\WinAPI\\Release\\dx9hook.dll"
local strLen = 0

function GetProcessName(obj)
	obj:ResetPtr()
	obj:Skip(36)
	return obj:GetString()
end

function GetProcessId(obj)
	obj:ResetPtr()
	obj:Skip(8)
	return obj:GetNumber(DWORD)
end

mem = samurai.Alloc(292)
pcEntry = mem:ToStruct()

pcEntry:ResetPtr()
pcEntry:PushNumber(DWORD,292)

snapshot = invoke(kernel32.CreateToolhelp32Snapshot,2,NULL)

if invoke(kernel32.Process32First,snapshot,mem:ToPointer()) == 1 then
	while invoke(kernel32.Process32Next,snapshot,mem:ToPointer()) == 1 do
		if GetProcessName(pcEntry) == "SimpleApp.exe" then
			pId = GetProcessId(pcEntry)
			print(pId)
		end
	end
else
	print("Process32First failed")
end

invoke(kernel32.CloseHandle,snapshot)
mem:Free()
strLen = string.len(fullPath)

hProcess = invoke(kernel32.OpenProcess,2035711,0,pId)

nmem = invoke(kernel32.VirtualAllocEx,hProcess,NULL,strLen,12288,64)
print("nmem "..nmem)
invoke(kernel32.WriteProcessMemory,hProcess,nmem,fullPath,strLen,NULL)

thr = invoke(kernel32.CreateRemoteThread,hProcess,0,0,kernel32.LoadLibraryA,nmem,0,0)
print("thr "..thr)
invoke(kernel32.WaitForSingleObject,thr,4294967295)
invoke(kernel32.CloseHandle,thr)

invoke(kernel32.VirtualFreeEx,hProcess,nmem,strLen,49152)
invoke(kernel32.CloseHandle,hProcess)