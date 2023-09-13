#include "hacks.h"
#include <thread>
#include<cmath>
#include "../utils/vector.h"

Vector3 CalculateAngle2(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

void h::legitbot(const Memory& mem) noexcept {
	while (g::run) {

		if (globals::legitbot) {

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
			const auto shotsFired = mem.Read<std::int32_t>(localPlayer + offsets::m_iShotsFired);



			const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

			// eye position = origin + viewOffset
			const auto localEyePosition = mem.Read<Vector3>(localPlayer + offsets::m_vecOrigin) +
				mem.Read<Vector3>(localPlayer + offsets::m_vecViewOffset);

			const auto clientState = mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState);

			const auto localPlayerId =
				mem.Read<std::int32_t>(clientState + offsets::dwClientState_GetLocalPlayer);

			const auto viewAngles = mem.Read<Vector3>(clientState + offsets::dwClientState_ViewAngles);
			const auto aimPunch = mem.Read<Vector3>(localPlayer + offsets::m_aimPunchAngle) * 2;

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

					const auto angle = CalculateAngle2(
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
			if (shotsFired) {

				if (!bestAngle.IsZero())
					mem.Write<Vector3>(clientState + offsets::dwClientState_ViewAngles, viewAngles + bestAngle / globals::smooth);
			}
		}
	}
}