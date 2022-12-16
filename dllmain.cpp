#include <Windows.h>
#include <iostream>
#include <thread>
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
	FreeLibraryAndExitThread(instance, 0);
}

BOOL WINAPI DllMain(HMODULE instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		std::thread(HackThread, instance).detach();
	}

	return TRUE;
}