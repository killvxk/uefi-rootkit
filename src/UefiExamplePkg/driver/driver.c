#include "driver.h"

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/Answer.h>

// Constants
const UINT32 kAnswer = 42;

// Declarations
VOID EFIAPI NotifyExitBootServices(IN EFI_EVENT Event, IN VOID *Context);
VOID EFIAPI NotifySetVirtualAddressMap(IN EFI_EVENT Event, IN VOID *Context);

EFI_STATUS EFIAPI GetAnswer(IN EFI_ANSWER_PROTOCOL *This, OUT UINT32 *data);

// Global Data
BOOLEAN gRuntime = FALSE;

EFI_EVENT gExitBootServicesEvent = NULL;
EFI_EVENT gVirtualAddressChangeEvent = NULL;

EFI_ANSWER_PROTOCOL gAnswer = {GetAnswer};

EFI_STATUS EFIAPI
DriverEntryPoint(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS status;
    status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle,
                                                    &gEfiComponentName2ProtocolGuid, &gComponentName2,
                                                    &gEfiAnswerProtocolGuid, &gAnswer,
                                                    NULL);
    if (EFI_ERROR(status))
    {
        return status;
    }

    status = gBS->CreateEvent(
        EVT_SIGNAL_EXIT_BOOT_SERVICES,
        TPL_NOTIFY,
        NotifyExitBootServices,
        NULL,
        &gExitBootServicesEvent);
    if (EFI_ERROR(status))
    {
        return status;
    }

    status = gBS->CreateEvent(
        EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
        TPL_NOTIFY,
        NotifySetVirtualAddressMap,
        NULL,
        &gVirtualAddressChangeEvent);
    if (EFI_ERROR(status))
    {
        return status;
    }

    return status;
}

EFI_STATUS EFIAPI GetAnswer(IN EFI_ANSWER_PROTOCOL *This, OUT UINT32 *data)
{
    *data = kAnswer;
    return EFI_SUCCESS;
}

VOID EFIAPI NotifyExitBootServices(IN EFI_EVENT Event, IN VOID *Context)
{
    gRuntime = TRUE;
}

VOID EFIAPI NotifySetVirtualAddressMap(IN EFI_EVENT Event, IN VOID *Context) {}