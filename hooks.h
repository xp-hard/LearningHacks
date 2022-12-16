#pragma once

#include "gui.h"

namespace hooks {
	void Setup();
	void Destroy() noexcept;

	constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept {
		return (*static_cast<void***>(thisptr))[index];
	}

	using EndScenceFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndScenceFn EndScenceOriginal = nullptr;
	long __stdcall EndScence(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
}