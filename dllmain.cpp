#include <Windows.h>
#include <iostream>
#include <thread>
#include "hooks.h"


void Setup(HMODULE instance) {
	try {
		gui::Setup();
		hooks::Setup();
	}
	catch (const std::exception& error) {
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "Cheat error", MB_OK | MB_ICONEXCLAMATION);

		goto UNLOAD;
	}

	while (!GetAsyncKeyState(VK_END)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

UNLOAD:
	hooks::Destroy();
	gui::Destroy();

	FreeLibraryAndExitThread(instance, 0);
}

BOOL WINAPI DllMain(HMODULE instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		std::thread(Setup, instance).detach();
	}

	return TRUE;
}