#include <Uefi.h>
#include <Library/UefiLib.h>

// #include "driver.h"
UINT32 GetData() { return 1; }

EFI_STATUS
EFIAPI
ApplicationEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    Print(L"Driver Data:%u\n", GetData());
    return EFI_SUCCESS;
}