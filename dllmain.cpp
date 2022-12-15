#include <iostream>
#include <Windows.h>
#include <thread>

void HackThread(HMODULE instance) {
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);

	while (!GetAsyncKeyState(VK_END)) {
		std::cout << "Injected\n";
	}

	if (file) {
		fclose(file);
	}

	FreeConsole();
	FreeLibraryAndExitThread(instance, 0);
}

BOOL WINAPI DllMain(HMODULE instace, DWORD reason, LPVOID lpvReserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instace);

		std::thread(HackThread, instace).detach();
	}
}