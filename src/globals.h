#pragma once
#include <vector>
#include <thread>
#include <array>
#include "sdk/entity.h"

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
}

namespace globals {

	inline std::uintptr_t clientAddress = 0;
	inline std::uintptr_t engineAddress = 0;

	inline bool aimbot = false;
	inline bool bhop = false;
	inline float fov = 0.0f;
	inline float smooth = 0.0f;
	inline bool recoil = false;
	inline bool trigger = false;
	inline bool crouch = false;


}

namespace g {
	inline bool run = true;

	inline std::vector<c_entity> entity_list;
	inline c_entity local_player;

	void entities() noexcept;

	constexpr const std::uintptr_t client_state() noexcept {
		return m::read<std::uintptr_t>(m::engine + offsets::dwClientState);
	}

	constexpr const std::int32_t max_players() noexcept {
		return m::read<std::int32_t>(client_state() + 0x388);
	}

	constexpr const std::uintptr_t glow_object_manager() noexcept {
		return m::read<std::uintptr_t>(m::client + offsets::dwGlowObjectManager);
	}

	constexpr void force_jump(const int& force_jump) noexcept {
		m::write(m::client + offsets::dwForceJump, force_jump);
	}
}

namespace v {
	inline std::pair<bool, std::array<float, 4U>> team_glow{
		false,
		{ 0.f, 0.f, 1.f, 1.f }
	};

	inline std::pair<bool, std::array<float, 4U>> enemy_glow{
		false,
		{ 1.f, 0.f, 1.f, 1.f }
	};

	inline bool radar = false;
}
