#ifndef MEM_H_
#define MEM_H_

#include <vector>
#include <windows.h>
#include <tlhelp32.h>

uintptr_t FindDMAddress(HANDLE process_handle, uintptr_t ptr, std::vector<unsigned int> offsets);

uintptr_t FindDMAddress(uintptr_t ptr, std::vector<unsigned int> offsets);

uintptr_t GetModuleBaseAddress(DWORD process_id, const char* modName);

DWORD GetProcessIdByProcessName(const char* process_name);

void Patch(char* dst, char* src, size_t size);

#endif
