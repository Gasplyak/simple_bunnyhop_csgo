#include "stdafx.h"
#include "iostream"
#include <Windows.h>
#include <TlHelp32.h>
#include "Memory.h"
#include "Offsets.h"
#include <wchar.h>

#pragma region Settings
//Stealth
bool StealthActive = true;
//Glow
bool GlowActive = true;
bool GlowTeamCheck = true;
float GlowTerroristRed = 1.f;
float GlowTerroristGreen = 0.f;
float GlowTerroristBlue = 0.f;
float GlowTerroristAlpha = 1.f;

float GlowCounterTerroristRed = 0.f;
float GlowCounterTerroristGreen = 0.f;
float GlowCounterTerroristBlue = 1.f;
float GlowCounterTerroristAlpha = 1.f;
#pragma endregion

int main(int argc, char* argv[]);
void Bhop(HANDLE hProcess);
void Glow(HANDLE hProcess);

using namespace std;

bool BunnyHop = false;
bool checkGlow = false;

void Bhop(HANDLE hProcess)
{
	Offsets.bClient = (DWORD)AxionGetModuleHandleExA(hProcess, "client.dll");
	Offsets.LocalBase = ReadMemory<int>(Offsets.bClient + Offsets.LocalPlayer, hProcess);
	int flags = ReadMemory<int>(Offsets.LocalBase + Offsets.oFlags, hProcess);
	if (GetAsyncKeyState(VK_SPACE) && flags == 257)
	{
		BunnyHop = true;
		WriteMemory<int>(Offsets.bClient + Offsets.ForceJump, 6, hProcess);
		Sleep(10);
		if (BunnyHop && flags == 256)
		{
			VK_SPACE;
			WriteMemory<int>(Offsets.bClient + Offsets.ForceJump, 6, hProcess);
			Sleep(10);
		}
	}
}

void Glow(HANDLE hProcess)
{
	checkGlow = true;
	DWORD glow_LocalBase = ReadMemory<DWORD>(Offsets.bClient + Offsets.LocalPlayer, hProcess);
	DWORD glow_Pointer = ReadMemory<DWORD>(Offsets.bClient + m_dwGlowObject, hProcess);
		int MyTeamID = ReadMemory<int>(glow_LocalBase + m_iTeamNum, hProcess);
		
		for (int i = 0; i < 64; i++)
		{
			int glow_currentPlayer = ReadMemory<int>(Offsets.bClient + m_dwEntityList + i * 0x10, hProcess);
			bool glow_currentPlayerDormant = ReadMemory<bool>(glow_currentPlayer + m_bDormant, hProcess);
			int glow_currentPlayerGlowIndex = ReadMemory<int>(glow_currentPlayer + m_iGlowIndex, hProcess);
			int EntityBaseTeamID = ReadMemory<int>(glow_currentPlayer + m_iTeamNum, hProcess);

			if (glow_currentPlayerDormant == 1 || EntityBaseTeamID == 0)
				continue;
			else
				if (MyTeamID != EntityBaseTeamID || GlowTeamCheck == false)
					switch (EntityBaseTeamID)	// 1 GoTV; 2 T; 3 CT
					{
					case 2:
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x4)), GlowTerroristRed, hProcess);
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x8)), GlowTerroristGreen, hProcess);
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0xC)), GlowTerroristBlue, hProcess);
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x10)), GlowTerroristAlpha, hProcess);
						WriteMemory<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x24)), true, hProcess);
						WriteMemory<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x25)), false, hProcess);
						break;
					case 3:
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x4)), GlowCounterTerroristRed, hProcess);
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x8)), GlowCounterTerroristGreen, hProcess);
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0xC)), GlowCounterTerroristBlue, hProcess);
						WriteMemory<float>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x10)), GlowCounterTerroristAlpha, hProcess);
						WriteMemory<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x24)), true, hProcess);
						WriteMemory<BOOL>((glow_Pointer + ((glow_currentPlayerGlowIndex * 0x34) + 0x25)), false, hProcess);
						break;
					}
		}
}

int main(int argc, char* argv[])
{
	SetConsoleTitle("Nightmare Hack for CS:GO");
	SYSTEMTIME st;
	GetLocalTime(&st);
	DWORD pId;
	HWND hwnd = FindWindowA(0, "Counter-Strike: Global Offensive");
	GetWindowThreadProcessId(hwnd, &pId);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);

	system("color 0A");
	printf("Nightmare Hack was started on: ");
	printf("%d-%02d-%02d% 02d:%02d:%02d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);

	while (true)
	{
		Bhop(hProcess);
	}
	if (GetAsyncKeyState(VK_F1))
	{
		while (true)
		{
			Glow(hProcess);
		}
	}
	
	return 0;
}