#pragma once

#include "datatable.h"
#include <unordered_map>
#include <string>

void SetUpNetvars();

void Dump(const char* baseClass, RecvTable* table, uint32_t offset = 0);

inline std::unordered_map<std::string, uint32_t> netvars;

#define NETVAR(func_name, netvar, type) type& func_name() {\
	static auto offset = netvars[netvar];\
	return *reinterpret_cast<type*> (uintptr_t(this) + offset);\
}

