#include "hacks.h"
#include <thread>
#include<cmath>
#include <Windows.h>
#include <WinUser.h>
#include <iostream>
void h::crouch(const Memory& mem) noexcept {
	while (g::run) {

		if (globals::crouch) {

			bool controlKeyPressed = false;

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (GetAsyncKeyState(VK_LBUTTON)) {
				if (!controlKeyPressed) {
					keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY, 0);
					controlKeyPressed = true;
				}
			}
			else {
				if (controlKeyPressed) {
					keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
					controlKeyPressed = false;
				}
			}

			Sleep(10);

		}
	}
}
