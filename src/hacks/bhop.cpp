#include "hacks.h"
#include <thread>
#include<cmath>



void h::bhop(const Memory& mem) noexcept {
	while (g::run) {

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
