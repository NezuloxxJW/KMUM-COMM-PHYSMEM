#include "includes.h"

bool writeRegistryDword(HKEY hKeyRoot, LPCWSTR subKey, LPCWSTR valueName, DWORD data);
bool writeRegistryQword(HKEY hKeyRoot, LPCWSTR subKey, LPCWSTR valueName, DWORD_PTR data);
bool cleanRegistry();