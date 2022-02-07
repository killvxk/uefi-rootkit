[Defines]
PLATFORM_NAME                  = UefiExamplePkg
PLATFORM_GUID                  = af49bc4a-daa0-4348-92bb-9bdb7cf86c22
PLATFORM_VERSION               = 1.0
DSC_SPECIFICATION              = 0x00010005
OUTPUT_DIRECTORY               = build
SUPPORTED_ARCHITECTURES        = X64
BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
SKUID_IDENTIFIER               = DEFAULT

[LibraryClasses]
UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
DebugLib|MdePkg/Library/UefiDebugLibStdErr/UefiDebugLibStdErr.inf
DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
PostCodeLib|MdePkg/Library/BasePostCodeLibPort80/BasePostCodeLibPort80.inf
PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf

[Components]
UefiExamplePkg/application/application.inf
UefiExamplePkg/driver/driver.inf