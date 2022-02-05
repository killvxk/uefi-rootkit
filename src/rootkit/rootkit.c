#include "rootkit.h"

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/DriverBinding.h>

// Declarations
EFI_STATUS EFIAPI RootkitDriverBindingSupported(IN EFI_DRIVER_BINDING_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL);
EFI_STATUS EFIAPI RootkitDriverBindingStart(IN EFI_DRIVER_BINDING_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL);
EFI_STATUS EFIAPI RootkitDriverBindingStop(IN EFI_DRIVER_BINDING_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN UINTN NumberOfChildren, IN EFI_HANDLE *ChildHandleBuffer OPTIONAL);
VOID EFIAPI NotifyExitBootServices(IN EFI_EVENT Event, IN VOID *Context);
VOID EFIAPI NotifySetVirtualAddressMap(IN EFI_EVENT Event, IN VOID *Context);

// Global Data
EFI_DRIVER_BINDING_PROTOCOL gRootkitDriverBinding = {
    RootkitDriverBindingSupported,
    RootkitDriverBindingStart,
    RootkitDriverBindingStop,
    ROORKIT_VERSION,
    NULL,
    NULL};

BOOLEAN gRuntime = FALSE;

EFI_EVENT gExitBootServicesEvent = NULL;
EFI_EVENT gVirtualAddressChangeEvent = NULL;

UINT32 gData = 0;

EFI_STATUS EFIAPI DriverEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS status;
    status = EfiLibInstallDriverBindingComponentName2(
        ImageHandle,
        SystemTable,
        &gRootkitDriverBinding,
        ImageHandle,
        &gRootkitComponentName,
        &gRootkitComponentName2);
    ASSERT_EFI_ERROR(status);

    status = gBS->CreateEvent(
        EVT_SIGNAL_EXIT_BOOT_SERVICES,
        TPL_NOTIFY,
        NotifyExitBootServices,
        NULL,
        &gExitBootServicesEvent);
    ASSERT_EFI_ERROR(status);

    status = gBS->CreateEvent(
        EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
        TPL_NOTIFY,
        NotifySetVirtualAddressMap,
        NULL,
        &gVirtualAddressChangeEvent);
    ASSERT_EFI_ERROR(status);

    return status;
}

EFI_STATUS EFIAPI RootkitDriverBindingSupported(IN EFI_DRIVER_BINDING_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL)
{
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI RootkitDriverBindingStart(IN EFI_DRIVER_BINDING_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL)
{
    gData = 42;
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI RootkitDriverBindingStop(IN EFI_DRIVER_BINDING_PROTOCOL *This, IN EFI_HANDLE ControllerHandle, IN UINTN NumberOfChildren, IN EFI_HANDLE *ChildHandleBuffer OPTIONAL)
{
    gData = 0;
    return EFI_SUCCESS;
}

UINT32 RootkitProtocolGetData()
{
    return gData;
}

VOID EFIAPI NotifyExitBootServices(IN EFI_EVENT Event, IN VOID *Context)
{
    gRuntime = TRUE;
}

VOID EFIAPI NotifySetVirtualAddressMap(IN EFI_EVENT Event, IN VOID *Context) {}