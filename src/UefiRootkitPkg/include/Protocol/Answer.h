#ifndef ANSWER_H
#define ANSWER_H

#include <Uefi.h>
#include <Library/UefiLib.h>

#define EFI_ANSWER_PROTOCOL_GUID                                                       \
    {                                                                                  \
        0x2c2c69ed, 0xe8F9, 0x4f7f, { 0xba, 0xda, 0x75, 0xfb, 0x30, 0x25, 0xce, 0x1F } \
    }

typedef struct _EFI_ANSWER_PROTOCOL EFI_ANSWER_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_ANSWER_GET_ANSWER)(IN EFI_ANSWER_PROTOCOL *This, OUT UINT32 *data);

typedef struct _EFI_ANSWER_PROTOCOL
{
    EFI_ANSWER_GET_ANSWER GetAnswer;
} EFI_ANSWER_PROTOCOL;

extern EFI_GUID gEfiAnswerProtocolGuid;

#endif /* ANSWER_H */
