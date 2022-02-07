#include "driver.h"

#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>

EFI_STATUS EFIAPI GetDriverName(IN EFI_COMPONENT_NAME2_PROTOCOL *This, IN CHAR8 *Language, OUT CHAR16 **DriverName);
EFI_STATUS EFIAPI GetControllerName(IN EFI_COMPONENT_NAME2_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN EFI_HANDLE ChildHandle, OPTIONAL IN CHAR8 *Language, OUT CHAR16 **ControllerName);

GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME2_PROTOCOL gComponentName2 = {
    GetDriverName,
    GetControllerName,
    "en"};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE gDriverNameTable[] = {{"eng;en", (CHAR16 *)L"The Wise"}, {NULL, NULL}};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE gControllerNameTable[] = {{"eng;en", (CHAR16 *)L"none"}, {NULL, NULL}};

EFI_STATUS EFIAPI GetDriverName(IN EFI_COMPONENT_NAME2_PROTOCOL *This, IN CHAR8 *Language, OUT CHAR16 **DriverName)
{
    return LookupUnicodeString2(
        Language,
        This->SupportedLanguages,
        gDriverNameTable,
        DriverName,
        (BOOLEAN)(This != &gComponentName2));
}

EFI_STATUS EFIAPI GetControllerName(IN EFI_COMPONENT_NAME2_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN EFI_HANDLE ChildHandle, OPTIONAL IN CHAR8 *Language, OUT CHAR16 **ControllerName)
{
    return LookupUnicodeString2(
        Language,
        This->SupportedLanguages,
        gControllerNameTable,
        ControllerName,
        (BOOLEAN)(This != &gComponentName2));
}