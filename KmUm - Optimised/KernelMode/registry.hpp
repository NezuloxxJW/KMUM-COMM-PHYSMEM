#include "globals.h"

NTSTATUS readAddressFromRegistry() {
	UNICODE_STRING RegPath;
	UNICODE_STRING AddrValueName;
	OBJECT_ATTRIBUTES ObjectAttributes;
	HANDLE KeyHandle;
	ULONG ResultLength;
	uintptr_t Buffer;

	RtlInitUnicodeString(&RegPath, L"\\Registry\\Machine\\");
	RtlInitUnicodeString(&AddrValueName, L"oAddr");

	InitializeObjectAttributes(&ObjectAttributes, &RegPath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	NTSTATUS status = ZwOpenKey(&KeyHandle, KEY_READ, &ObjectAttributes);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	KEY_VALUE_PARTIAL_INFORMATION KeyValueInfo;
	status = ZwQueryValueKey(KeyHandle, &AddrValueName, KeyValuePartialInformation, &KeyValueInfo, sizeof(KeyValueInfo) + sizeof(DWORD), &ResultLength);

	if (NT_SUCCESS(status)) {
		if (KeyValueInfo.Type == REG_QWORD && KeyValueInfo.DataLength == sizeof(uintptr_t)) {
			RtlCopyMemory(&Buffer, KeyValueInfo.Data, sizeof(uintptr_t));
			oAddr = Buffer;
		}
		else {
			status = STATUS_INVALID_PARAMETER;
		}
	}

	ZwClose(KeyHandle);
	return status;
}

NTSTATUS readPidFromRegistry() {
	UNICODE_STRING RegPath;
	UNICODE_STRING PidValueName;
	UNICODE_STRING AddrValueName;
	OBJECT_ATTRIBUTES ObjectAttributes;
	HANDLE KeyHandle;
	ULONG ResultLength;
	DWORD Buffer;

	RtlInitUnicodeString(&RegPath, L"\\Registry\\Machine\\");
	RtlInitUnicodeString(&PidValueName, L"oPid");

	InitializeObjectAttributes(&ObjectAttributes, &RegPath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	NTSTATUS status = ZwOpenKey(&KeyHandle, KEY_READ, &ObjectAttributes);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	KEY_VALUE_PARTIAL_INFORMATION KeyValueInfo;
	status = ZwQueryValueKey(KeyHandle, &PidValueName, KeyValuePartialInformation, &KeyValueInfo, sizeof(KeyValueInfo) + sizeof(DWORD), &ResultLength);

	if (NT_SUCCESS(status)) {
		if (KeyValueInfo.Type == REG_DWORD && KeyValueInfo.DataLength == sizeof(DWORD)) {
			RtlCopyMemory(&Buffer, KeyValueInfo.Data, sizeof(DWORD));
			oPid = Buffer;
		}
		else {
			status = STATUS_INVALID_PARAMETER;
		}
	}

	ZwClose(KeyHandle);
	return status;
}