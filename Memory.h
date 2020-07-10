#pragma once
#include "Offsets.h"
#ifdef MODULEENTRY32
#undef MODULEENTRY32
#endif
#ifdef Module32First
#undef Module32First
#endif
#ifdef Module32Next
#undef Module32Next
#endif

auto AxionGetModuleHandleExA(HANDLE Process, const char* Module) -> HMODULE
{
	DWORD dwInfo;
	if ((GetHandleInformation(Process, &dwInfo) && !(Module[0] == '\0')))
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(Process));
		if (hSnap && INVALID_HANDLE_VALUE != hSnap)
		{
			MODULEENTRY32 ME32;
			ME32.dwSize = sizeof(decltype(ME32));
			bool bResult = Module32First(hSnap, &ME32);
			while (bResult)
			{
				if (!_stricmp(Module, ME32.szModule))
				{
					HMODULE hBaseAddress = ME32.hModule;
					CloseHandle(hSnap);

					return hBaseAddress;
				}
				bResult = Module32Next(hSnap, &ME32);
			}
			CloseHandle(hSnap);
		}
	}
	return nullptr;
}

template<typename T>
T ReadMemory(DWORD Address, HANDLE hProcess)
{
	T read;
	ReadProcessMemory(hProcess, (LPCVOID)Address, &read, sizeof(T), 0);
	return read;
}

template <typename T>
T ReadMemory(DWORD Address, char *Offset, BOOL Type)
{
	int iSize = iSizeOfArray(Offset) - 1;
	Address = ReadMemory<DWORD>(Address);

	//Store Hex values
	for (int i = 0; i < iSize; i++)
		Address = ReadMemory<DWORD>(Address + Offset[i]);

	if (!Type)
		return Address + Offset[iSize];
	else
		return ReadMemory<T>(Address + Offset[iSize]);
}

template<typename T>
void WriteMemory(DWORD Address, T value, HANDLE hProcess)
{
	WriteProcessMemory(hProcess, (LPVOID)Address, &value, sizeof(T), 0);
}
template<typename T>
void WriteMemory(DWORD Address, char *Offset, T Value, HANDLE hProcess)
{
	WriteMemory<T>(ReadMemory<T>(Address, Offset, false), Value);
}
