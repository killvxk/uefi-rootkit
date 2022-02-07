#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/Answer.h>

EFI_STATUS
EFIAPI
ApplicationEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    UINTN handle_count = 0;
    EFI_HANDLE *handles;
    EFI_STATUS status = gBS->LocateHandleBuffer(ByProtocol, &gEfiAnswerProtocolGuid, NULL, &handle_count, &handles);
    if (EFI_ERROR(status) || handle_count == 0)
    {
        Print(L"Nobodoy can give an answer.\n");
        return status;
    }

    for (UINTN i = 0; i < handle_count; i++)
    {
        EFI_ANSWER_PROTOCOL *answer_protocol;
        status = gBS->OpenProtocol(handles[i], &gEfiAnswerProtocolGuid, (VOID **)&answer_protocol, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
        if (EFI_ERROR(status))
        {
            continue;
        }

        UINT32 answer;
        status = answer_protocol->GetAnswer(answer_protocol, &answer);
        if (EFI_ERROR(status))
        {
            continue;
        }

        Print(L"The answer is: %u.\n", answer);

        goto cleanup;
    }

cleanup:
    gBS->FreePool(handles);

    return status;
}