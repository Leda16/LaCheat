#pragma once
#include <vector>
#include <thread>
#include <array>

namespace offsets {
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEB99C;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x535BAD0;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4E0102C;
	constexpr ::std::ptrdiff_t dwForceJump = 0x52BCD88;
	constexpr ::std::ptrdiff_t dwClientState = 0x59F19C;
	constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D90;
	constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;
	constexpr ::std::ptrdiff_t dwForceAttack = 0x322EE98;

	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
	constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
	constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
	constexpr ::std::ptrdiff_t m_bDormant = 0xED;
	constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
	constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
	constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
	constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
	constexpr ::std::ptrdiff_t m_iShotsFired = 0x103E0;
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_iCrosshairId = 0x11838;

	constexpr ::std::ptrdiff_t model_ambient_min = 0x5A1194;
	constexpr ::std::ptrdiff_t m_clrRender = 0x70;

	//Skinchanger OFFSETAS
	constexpr ::std::ptrdiff_t m_hMyWeapons = 0x2E08;
	constexpr ::std::ptrdiff_t m_flFallbackWear = 0x31E0;
	constexpr ::std::ptrdiff_t m_nFallbackPaintKit = 0x31D8;
	constexpr ::std::ptrdiff_t m_nFallbackSeed = 0x31DC;
	constexpr ::std::ptrdiff_t m_nFallbackStatTrak = 0x31E4;
	constexpr ::std::ptrdiff_t m_iItemDefinitionIndex = 0x2FBA;
	constexpr ::std::ptrdiff_t m_iItemIDHigh = 0x2FD0;
	constexpr ::std::ptrdiff_t m_iEntityQuality = 0x2FBC;
	constexpr ::std::ptrdiff_t m_iAccountID = 0x2FD8;
	constexpr ::std::ptrdiff_t m_OriginalOwnerXuidLow = 0x31D0;

	//Legitbot Teste

	constexpr ::std::ptrdiff_t m_hActiveWeapon = 0x2F08;



}

struct Color
{
	std::uint8_t r{}, g{}, b{};
};

struct Color2
{
	std::uint8_t r{}, g{}, b{};
};

namespace globals {

	inline std::uintptr_t clientAddress = 0;
	inline std::uintptr_t engineAddress = 0;

	inline bool aimbot = false;
	inline bool bhop = false;
	inline float fov = 170.0f;
	inline float smooth = 1.0f;
	inline bool recoil = false;
	inline bool trigger = false;

	inline bool glowTeam = false;
	inline bool glowEnemy = false;
	inline float glowColorTeam[] = { 1.f, 0.f, 0.f, 1.f };
	inline float glowColorEnemy[] = { 1.f, 0.f, 0.f, 1.f };

	inline bool radar = false;

	inline bool chams = false;

	inline Color TeamChamsColor{ 0, 0, 255 };
	inline Color2 EnemyChamsColor{ 255, 0, 0 };

	inline bool skinchanger = false;
	inline bool statTrack = false;
	inline int keyAimbot;

	inline bool legitbot = false;
}

namespace g {
	inline bool run = true;

}