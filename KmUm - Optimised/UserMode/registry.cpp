#include "registry.h"

bool writeRegistryDword(HKEY hKeyRoot, LPCWSTR subKey, LPCWSTR valueName, DWORD data) {
    HKEY hKey;
    LONG result;

    result = RegOpenKeyEx(hKeyRoot, subKey, 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to open registry key: " << subKey << std::endl;
        return false;
    }

    result = RegSetValueEx(hKey, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to write DWORD to registry: " << valueName << std::endl;
        RegCloseKey(hKey);
        return false;
    }

    RegCloseKey(hKey);
    return true;
}

bool writeRegistryQword(HKEY hKeyRoot, LPCWSTR subKey, LPCWSTR valueName, DWORD_PTR data) {
    HKEY hKey;
    LONG result;

    result = RegOpenKeyEx(hKeyRoot, subKey, 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to open registry key: " << subKey << std::endl;
        return false;
    }

    result = RegSetValueEx(hKey, valueName, 0, REG_QWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to write QWORD to registry: " << valueName << std::endl;
        RegCloseKey(hKey);
        return false;
    }

    RegCloseKey(hKey);
    return true;
}

bool cleanRegistry() {
    HKEY hKey;
    LONG result;

    result = RegDeleteKeyValueA(HKEY_LOCAL_MACHINE, "", "oPid");
    if (result != ERROR_SUCCESS) {
        std::cout << result << std::endl;
        return false;
    }

    result = RegDeleteKeyValueA(HKEY_LOCAL_MACHINE, "", "oAddr");
    if (result != ERROR_SUCCESS) {
        std::cout << result << std::endl;
        return false;
    }

    return true;
}
