#include "hooks.h"
#include <stdexcept>
#include <intrin.h>
#include "thirdparty/minhook/include/MinHook.h"
#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/backends/imgui_impl_dx9.h"
#include "thirdparty/imgui/backends/imgui_impl_win32.h"

void hooks::Setup() {
	if (MH_Initialize()) {
		throw std::runtime_error("Failed to initialize minhook!");
	}

	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScence,
		reinterpret_cast<void**>(&EndScenceOriginal)
	)) {
		throw "Unable to hook EndScence()!";
	}

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&EndScence,
		reinterpret_cast<void**>(&ResetOriginal)
	)) {
		throw "Unable to hook Reset()!";
	}

	if (MH_EnableHook(MH_ALL_HOOKS)) {
		throw std::runtime_error("Failed to enable hooks!");
	}

	gui::DestroyDirectx();
}
void hooks::Destroy() noexcept {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScence(IDirect3DDevice9* device) noexcept {
	//static const auto returnAddres = _ReturnAddress();

	const auto result = EndScenceOriginal(device, device);
	
	/*if (_ReturnAddress == returnAddres) {
		return result;
	}*/

	if (!gui::setup) {
		gui::SetupMenu(device);
	}

	if (gui::open) {
		gui::Render();
	}

	return result;
}
HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}