#include "hacks.h"
#include <thread>
#include<cmath>
#include <array>

constexpr const int PinturaDaArma(const short& itemDefinition) {
	switch (itemDefinition) {
	case 1: return 711; //deagle
	case 4: return 38; //glock
	case 7: return 490; //ak
	case 9: return 344; //awp
	case 61: return 653; //usp
	default: return 0;
	}
}

void h::skinchanger(const Memory& mem) noexcept {
	while (g::run) {

		if (globals::skinchanger) {

			std::this_thread::sleep_for(std::chrono::milliseconds(2));

			const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
			const auto weapons = mem.Read<std::array<unsigned long, 8>>(localPlayer + offsets::m_hMyWeapons);

			for (const auto handle : weapons) {
				const auto weapon = mem.Read<std::uintptr_t>((globals::clientAddress + offsets::dwEntityList + (handle & 0xFFF) * 0x10) - 0x10);

				if (!weapon)
					continue;

				if (const auto paint = PinturaDaArma(mem.Read<short>(weapon + offsets::m_iItemDefinitionIndex))) {
					const bool shouldUpdate = mem.Read<std::int32_t>(weapon + offsets::m_nFallbackPaintKit) != paint;

					mem.Write<std::int32_t>(weapon + offsets::m_iItemIDHigh, - 1);

					mem.Write<std::int32_t>(weapon + offsets::m_nFallbackPaintKit, paint);
					mem.Write<float>(weapon + offsets::m_flFallbackWear, 0.1f);

					if (globals::statTrack) {

						mem.Write<std::int32_t>(weapon + offsets::m_iItemIDHigh, -1);

						mem.Write<std::int32_t>(weapon + offsets::m_nFallbackPaintKit, paint);
						mem.Write<float>(weapon + offsets::m_flFallbackWear, 0.1f);

						mem.Write<std::int32_t>(weapon + offsets::m_nFallbackSeed, 0);
						mem.Write < std::int32_t>(weapon + offsets::m_nFallbackStatTrak, 1337);
						mem.Write<std::int32_t>(weapon + offsets::m_iAccountID, mem.Read<std::int32_t>(weapon + offsets::m_OriginalOwnerXuidLow));

						if (shouldUpdate)
							mem.Write<std::int32_t>(mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState) + 0x174, -1);
					}
					if (shouldUpdate)
						mem.Write<std::int32_t>(mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState) + 0x174, - 1);
				}

			}

		}
	}
}
