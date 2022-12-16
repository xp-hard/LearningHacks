#include <Windows.h>
#include <iostream>
#include <thread>
<<<<<<< HEAD
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

=======
#include "interface.h"

template<class T>
T* GetInterface(const char* name, const char* library) {
	const auto handle = GetModuleHandle(library);

	if (!handle) {
		throw ERROR;
	}

	const auto functionAddress = GetProcAddress(handle, "CreateInterface");

	if (!functionAddress) {
		throw ERROR;
	}

	using Fn = T * (*)(const char*, int*);
	const auto CreateInteface = (Fn)functionAddress;

	return CreateInteface(name, nullptr);
}

void HackThread(HMODULE instance) {
	client = GetInterface<IClient>("VClient017", "client.dll");
	entityList = GetInterface<IEntityList>("VClientEntityList003", "client.dll");

	SetUpNetvars();

	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);

	while (!GetAsyncKeyState(VK_END)) {
		Sleep(200);
		for (int i = 0; i < 64; ++i) {
			auto ent = entityList->GetClientEntity(i);
			if (!ent) {
				break;
			}
			std::cout << i << " " << ent->m_iHealth() << "\n";
		}
	}

	if (file) {
		fclose(file);
	}

	FreeConsole();
>>>>>>> 14d6d91 (Printing health of all entitys in console)
	FreeLibraryAndExitThread(instance, 0);
}

BOOL WINAPI DllMain(HMODULE instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

<<<<<<< HEAD
		std::thread(Setup, instance).detach();
=======
		std::thread(HackThread, instance).detach();
>>>>>>> 14d6d91 (Printing health of all entitys in console)
	}

	return TRUE;
}