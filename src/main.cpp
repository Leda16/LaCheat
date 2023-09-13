#include <iostream>
#include <format>
#include <thread>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "globals.h"
#include "ui.h"
#include "hacks/hacks.h"

const int dialog(const std::string& text) noexcept {
	MessageBeep(MB_ICONERROR);
	return MessageBox(
		u::window ? u::window : NULL,
		text.c_str(),
		"LaCheat",
		MB_ICONERROR | MB_OK
	);
}

int __stdcall WinMain(
	const HINSTANCE instance,
	const HINSTANCE prev_instance,
	const LPSTR args,
	const int cmd_show
) {


	Memory mem{"csgo.exe"};

	globals::clientAddress = mem.GetModuleAddress("client.dll");
	globals::engineAddress = mem.GetModuleAddress("engine.dll");

	if (globals::clientAddress == 0 || globals::engineAddress == 0) {
		return dialog("Failed to get module addresses.");
	}

	if (!u::create_window("LaCheat")) {
		return dialog("Failed to create window.");
	}

	if (!u::create_device()) {
		u::destroy_window();
		return dialog("Failed to create device.");
	}

	u::create_menu();

	std::thread(h::visuals, mem).detach();
	std::thread(h::aimbot, mem).detach();
	std::thread(h::bhop, mem).detach();
	std::thread(h::recoil, mem).detach();
	std::thread(h::trigger, mem).detach();
	std::thread(h::chams, mem).detach();
	std::thread(h::skinchanger, mem).detach();





	while (g::run) {
		u::render();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// cleanup
	u::destroy_menu();
	u::destroy_device();
	u::destroy_window();


	return 0;
}
