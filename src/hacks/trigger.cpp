#include "hacks.h"
#include <thread>
#include<cmath>

void h::trigger(const Memory& mem) noexcept {
	while (g::run) {

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
