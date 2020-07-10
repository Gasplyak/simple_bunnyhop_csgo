#pragma once
#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H
const DWORD m_dwEntityList = 0x4A8574C;
const DWORD m_dwGlowObject = 0x4FB2540; //here is offset of GlowObjectManager(I don't know when I can find the offset with this name)
const DWORD m_iGlowIndex = 0xA310;
const DWORD m_iTeamNum = 0xF0;
const DWORD m_bDormant = 0x000000E9;

struct Offsets
{
	DWORD bClient, LocalBase;
	DWORD LocalPlayer = 0xAAAAB4;
	DWORD ForceJump = 0x4F1C9F0;
	DWORD oFlags = 0x100;
}Offsets;
#endif