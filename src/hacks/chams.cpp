#include "hacks.h"
#include <thread>
#include<cmath>

struct ColorOff
{
	std::uint8_t r{}, g{}, b{};
};

void h::chams(const Memory& mem) noexcept {

	constexpr const auto TeamChamsColorOff = ColorOff{255,255,255 };
	constexpr const auto EnemyChamsColorOff = ColorOff{ 255,255,255 };

	while (g::run) {

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

			const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
			const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

			for (auto i =1; i <= 32; ++i)
			{ 
				const auto& entity = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);

				if (globals::chams) {

					if (mem.Read<std::int32_t>(entity + offsets::m_iTeamNum) == localTeam)
					{
						mem.Write<Color>(entity + offsets::m_clrRender, globals::TeamChamsColor);
					}
					else
					{
						mem.Write<Color2>(entity + offsets::m_clrRender, globals::EnemyChamsColor);
					}

					float brightness = 25.f;

					const auto _this = static_cast<std::uintptr_t>(globals::engineAddress + offsets::model_ambient_min - 0x2c);
					mem.Write<std::int32_t>(globals::engineAddress + offsets::model_ambient_min, *reinterpret_cast<std::uintptr_t*>(&brightness) ^ _this);
				}
				else {


					if (mem.Read<std::int32_t>(entity + offsets::m_iTeamNum) == localTeam)
					{
						mem.Write<ColorOff>(entity + offsets::m_clrRender, TeamChamsColorOff);
					}
					else
					{
						mem.Write<ColorOff>(entity + offsets::m_clrRender, EnemyChamsColorOff);
					}

					float brightnessOff = 0.f;

					const auto _this = static_cast<std::uintptr_t>(globals::engineAddress + offsets::model_ambient_min - 0x2c);
					mem.Write<std::int32_t>(globals::engineAddress + offsets::model_ambient_min, *reinterpret_cast<std::uintptr_t*>(&brightnessOff) ^ _this);
				}
			}

	}
}
