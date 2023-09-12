#pragma once
#include "memory.h"

namespace hacks
{
	// run visual hacks
	void VisualsThread(const Memory& mem) noexcept;
	void AimbotThread(const Memory& mem) noexcept;
	void BhopThread(const Memory& mem) noexcept;
	void RecoilThread(const Memory& mem) noexcept;
	void TriggerThread(const Memory& mem) noexcept;




}
