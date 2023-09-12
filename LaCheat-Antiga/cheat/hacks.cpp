#include "hacks.h"
#include "gui.h"
#include "globals.h"
#include "vector.h"
#include <thread>

void hacks::VisualsThread(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);

		if (!localPlayer)
			continue;

		const auto glowManager = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwGlowObjectManager);

		if (!glowManager)
			continue;

		const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

		for (auto i = 1; i <= 32; ++i)
		{
			const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);

			if (!player)
				continue;

			const auto team = mem.Read<std::int32_t>(player + offsets::m_iTeamNum);

			if (team == localTeam)
				continue;

			const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);

			if (lifeState != 0)
				continue;

			if (globals::glow)
			{
				const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

				mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::glowColor[0]); // red
				mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::glowColor[1]); // green
				mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::glowColor[2]); // blue
				mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::glowColor[3]); // alpha

				mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
				mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);
			}

			if (globals::radar)
				mem.Write(player + offsets::m_bSpotted, true);
		}
	}
}

Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

void hacks::AimbotThread(const Memory& mem) noexcept
{
	while (gui::isRunning) {

		if (globals::aimbot) {

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (!GetAsyncKeyState(VK_XBUTTON1))
				continue;

			const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
			const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

			// eye position = origin + viewOffset
			const auto localEyePosition = mem.Read<Vector3>(localPlayer + offsets::m_vecOrigin) +
				mem.Read<Vector3>(localPlayer + offsets::m_vecViewOffset);

			const auto clientState = mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState);

			const auto localPlayerId =
				mem.Read<std::int32_t>(clientState + offsets::dwClientState_GetLocalPlayer);

			const auto viewAngles = mem.Read<Vector3>(clientState + offsets::dwClientState_ViewAngles);
			const auto aimPunch = mem.Read<Vector3>(localPlayer + offsets::m_aimPunchAngle) * 2;

			// aimbot fov
			auto bestAngle = Vector3{ };

			for (auto i = 1; i <= 32; ++i)
			{
				const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);

				if (mem.Read<std::int32_t>(player + offsets::m_iTeamNum) == localTeam)
					continue;

				if (mem.Read<bool>(player + offsets::m_bDormant))
					continue;

				if (mem.Read<std::int32_t>(player + offsets::m_lifeState))
					continue;

				if (mem.Read<std::int32_t>(player + offsets::m_bSpottedByMask) & (1 << localPlayerId))
				{
					const auto boneMatrix = mem.Read<std::uintptr_t>(player + offsets::m_dwBoneMatrix);

					// Posicao 8 da cabeca em 3d, 8 em bone index
					const auto playerHeadPosition = Vector3{
						mem.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
						mem.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
						mem.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
					};

					const auto angle = CalculateAngle(
						localEyePosition,
						playerHeadPosition,
						viewAngles + aimPunch
					);

					const auto fov = std::hypot(angle.x, angle.y);

					if (fov < globals::fov)
					{
						bestAngle = angle;
					}
				}
			}

			if (!bestAngle.IsZero())
				mem.Write<Vector3>(clientState + offsets::dwClientState_ViewAngles, viewAngles + bestAngle / globals::smooth);
		}
	}
}

void hacks::BhopThread(const Memory& mem) noexcept
{
	while (gui::isRunning) {

		if (globals::bhop) {

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);

			if (!localPlayer)
				continue;

			const auto localPlayerTeam = mem.Read<std::uintptr_t>(localPlayer + offsets::m_iTeamNum);
			const auto localPlayerFlags = mem.Read<std::uintptr_t>(localPlayer + offsets::m_fFlags);

			if (GetAsyncKeyState(VK_SPACE))
				(localPlayerFlags & (1 << 0)) ?
				mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceJump, 6) :
				mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceJump, 4);
		}
	}
}






void hacks::RecoilThread(const Memory& mem) noexcept {

	struct Vector2 {
		float x = { }, y = { };
	};

	auto oldPunch = Vector2{ };


	while (gui::isRunning) {


		if (globals::recoil) {

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
			const auto shotsFired = mem.Read<std::int32_t>(localPlayer + offsets::m_iShotsFired);

			if (shotsFired) {
				const auto clientState = mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState);
				const auto viewAngles = mem.Read<Vector2>(clientState + offsets::dwClientState_ViewAngles);

				const auto aimPunch = mem.Read<Vector2>(localPlayer + offsets::m_aimPunchAngle);

				auto newAngles = Vector2{
					viewAngles.x + oldPunch.x - aimPunch.x * 2.f,
					viewAngles.y + oldPunch.y - aimPunch.y * 2.f
				};

				if (newAngles.x > 89.f)
					newAngles.x = 89.f;

				if (newAngles.x < -89.f)
					newAngles.x = -89.f;

				while (newAngles.y > 180.f)
					newAngles.y -= 360.f;

				while (newAngles.y < -180.f)
					newAngles.y += 360.f;

				mem.Write<Vector2>(clientState + offsets::dwClientState_ViewAngles, newAngles);

				oldPunch.x = aimPunch.x * 2.f;
				oldPunch.y = aimPunch.y * 2.f;

			}
			else {
				oldPunch.x = oldPunch.y = 0.f;
			}
		}
	}
}


void hacks::TriggerThread(const Memory& mem) noexcept
{
	while (gui::isRunning) {

		if (globals::trigger) {

			std::this_thread::sleep_for(std::chrono::milliseconds(1));


			if (!GetAsyncKeyState(VK_XBUTTON1))
				continue;

			const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
			const auto localHealth = mem.Read<std::int32_t>(globals::clientAddress + offsets::m_iHealth);
			
			if (!localHealth)
				continue;

			const auto crosshairId = mem.Read<std::int32_t>(localPlayer + offsets::m_iCrosshairId);

			if (!crosshairId || crosshairId > 64)
				continue;

			const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + (crosshairId - 1) * 0x10);

			if (!mem.Read<std::int32_t>(player + offsets::m_iHealth))
				continue;

			if (mem.Read<std::int32_t>(player + offsets::m_iTeamNum) ==
				mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum))
				continue;

			mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceAttack, 6);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceAttack, 4);

		}
	}
}