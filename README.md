# UEFI Rootkit
Bachelor thesis about a UEFI Rookit communitcating witht the TPM module in an effort to gain trusted information from the windows boot process.

# Current Research

## "First UEFI rootikit found in the wild"
- https://www.welivesecurity.com/wp-content/uploads/2018/09/ESET-LoJax.pdf

## "Second ever UEFI rootkit found in the wild"
- https://www.bleepingcomputer.com/news/security/mosaicregressor-second-ever-uefi-rootkit-found-in-the-wild/
- https://securelist.com/mosaicregressor/98849/

# UEFI
- https://en.wikipedia.org/wiki/Unified_Extensible_Firmware_Interface
- https://uefi.org/sites/default/files/resources/UEFI_Spec_2_9_2021_03_18.pdf
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/31_basic_programming_model
## Images
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/readme.7
- PE/COFF header (Microsoft Portable Executable and Common Object File Format Specification)
- relocatable images
- supported platform types:
    - IA32-Intel IA-32 platforms
    - X64-Intel(R) 64 platforms
    - IPF-Intel(R) Itanium(R)-based platforms
    - EBC-EFI Byte Code platforms
- three different image types:
    - UEFI applications
    - UEFI boot services drivers
    - UEFI runtime drivers
- supported storage locations:
    - Expansion ROMs on a PCI card
    - System ROM or system flash
    - A media device such as a hard disk, floppy, CD-ROM, DVD, FLASH drive
    - LAN server
- image entry points receive own image handle
- pointer to UEFI system table
![UEFI image types](doc/images/uefi_image_types.jpg)
- [UEFI image types](https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/readme.7)
### Applications
- returns execution
- unloaded from memory afterwards
- not resident
- exception is os loader which does not return and instead calls `ExitBootServices()`
### Drivers
#### Boottime
- exist before `ExitBootServices()` is called
- `EfiBootServicesData` memory freed and reusable afterwards
#### Runtime
- stay after `ExitBootServices()` is called
- `EfiRuntimeServicesData` memory
- alternative calling mechanism during runtime
- handle database does not persist
## Platform initialization
https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/readme.15

# Library Comparison

## TianoCore EDK II
- https://github.com/tianocore/edk2
- https://en.wikipedia.org/wiki/TianoCore_EDK_II
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/
The first open source UEFI implementation, Tiano, was released by Intel in 2004. Tiano has since then been superseded by EDK[14] and EDK2[15] and is now maintained by the TianoCore community.[16]
- which uefi spec is supported?

## Project Mu
- https://github.com/microsoft/mu/
- https://microsoft.github.io/mu/faq/

## GNU-EFI
- https://wiki.osdev.org/GNU-EFI
- https://sourceforge.net/p/gnu-efi/code/ci/master/tree/

## POSIX-UEFI
- https://wiki.osdev.org/POSIX-UEFI
- https://gitlab.com/bztsrc/posix-uefi
- inspired by GNU-EFI
- a lot smaller

# Rootkit
- https://docs.microsoft.com/en-us/windows/security/information-protection/secure-the-windows-10-boot-process

# Implementation
- https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
- https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Platforms
- https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documents
- https://github.com/tianocore/tianocore.github.io/wiki/UEFI-EDKII-Learning-Dev
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/2_uefi_driver_implementation_checklist/22_how_to_implement_features_in_edk_ii
- https://stackoverflow.com/questions/63725239/build-edk2-in-linux
- https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-Writing-Simple-Application
- https://edk2-docs.gitbook.io/edk-ii-build-specification/

## UefiExamplePkg
This is a simple EDK II package consiting of an application, a driver and a protocol. It serves as a first introductory exercise to UEFI programming. The driver implements and installs a protocol called `Answer` which the application then can search for. If the application is able to find an instance of the protocol the protocol function `GetAnswer` is called and it's output is printed to console. If the application is not able to find an instance it will print that to the console. In both cases the application terminates and can be launched to try again.


# How to Build
## Setting up EDK II
Follow [Getting Started with EDK II](https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II#how-to-setup-a-local-edk-ii-tree) by selecting the appropriate guide for your build environment. Stop at [Setup build shell environment](https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions#setup-build-shell-environment) in the [common instructions](https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions).

Clone repository and change directory:
```
git clone https://github.com/powware/uefi-rootkit.git
cd uefi-rootkit
```

Now set the following environment variables:
- `WORKSPACE` to the cloned repository (e.g. `export WORKSPACE=~/repos/uefi-rootkit`)
- `PACKAGES_PATH` to the `src` folder of this repository as well as the edk2 repository (e.g. `export PACKAGES_PATH=~/repos/uefi-rootkit/src/:~/repos/edk2`)
- Add the edk2 repository to your `PATH`


and run this script:
```
. edksetup.sh
```

_**Note**: Every new session requires the environment variables to be set and the script to be run._


In `Conf/target.txt` you need to change `TARGET_ARCH` to `X64` and `TOOL_CHAIN_TAG` to use your tool chain (`Conf/tools_def.txt` has a list of all tool chains and allows for custom definitions). I entered `GCC5` to use my gcc 11.2.0 installation.

## Building UefiExamplePkg
To build this package have EDK II setup with all the needed environment variables and enter:
```
build -p src/UefiRootkitPkg/UefiRootkitPkg.dsc
```

# Usage

## Obtaining an OVMF image
You will need a UEFI firmware image for QEMU emulation. You can either [build one yourself](https://github.com/tianocore/tianocore.github.io/wiki/How-to-build-OVMF) from the EDK II repository or download precompiled images [here](https://www.kraxel.org/repos/) or install the `ovmf` apt repository.

<!--
## UEFI Shell
-->

## Running UefiExmaplePkg
Using QEMU and the UEFI emulation firmware you need to enter (make sure to use an absolute path with the `-hda` option):
```
qemu-system-x86_64 --bios <path to>/OVMF.fd -net none -hda fat:rw:<absolute path to>/uefi-rootkit/build/<tool chain>/X64
```

You should now see the UEFI shell, change to the file directory:
```
fs0:
```
No you can launch the application:
```
.\Application.efi
```
The application was not able to find an instance of the protocol it wanted to access and is unable to print an answer.

Start the driver first and then launch the application this time:
```
load Driver.efi
.\Application.efi
```
Loading the driver before launching the application allows the driver to install an instance of the protocol. The application was now able to find a protocol instance and could call the offered function to print an answer.

That's it!

# Sources
- https://docs.microsoft.com/en-us/windows/security/information-protection/secure-the-windows-10-boot-process
- https://www.welivesecurity.com/wp-content/uploads/2018/09/ESET-LoJax.pdf
- https://www.bleepingcomputer.com/news/security/mosaicregressor-second-ever-uefi-rootkit-found-in-the-wild/
- https://securelist.com/mosaicregressor/98849/
- https://en.wikipedia.org/wiki/Unified_Extensible_Firmware_Interface
- https://github.com/tianocore/edk2
- https://en.wikipedia.org/wiki/TianoCore_EDK_II
- https://github.com/microsoft/mu/
- https://microsoft.github.io/mu/faq/
- https://uefi.org/sites/default/files/resources/UEFI_Spec_2_3_1.pdf
- https://uefi.org/sites/default/files/resources/UEFI_Spec_2_9_2021_03_18.pdf
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/
- https://www.youtube.com/watch?v=CGBpil0S5NI
- https://www.youtube.com/watch?v=P3yMXspLzoY
- https://wiki.osdev.org/UEFI
- https://wiki.osdev.org/GNU-EFI
- https://sourceforge.net/p/gnu-efi/code/ci/master/tree/
- https://wiki.osdev.org/POSIX-UEFI
- https://gitlab.com/bztsrc/posix-uefi
- https://doc.coreboot.org/
- https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
- https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documents
- https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Platforms
- https://github.com/tianocore/tianocore.github.io/wiki/UDK

<!--
# Writer Guide Broken:
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/34_handle_database
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/readme.8
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/appendix_b_edk_ii_sample_drivers
-->