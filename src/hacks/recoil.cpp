#include "hacks.h"
#include <thread>
#include<cmath>

void h::recoil(const Memory& mem) noexcept {

	struct Vector2 {
		float x = { }, y = { };
	};

	auto oldPunch = Vector2{ };

	while (g::run) {

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
