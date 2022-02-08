# UEFI Rootkit

Bachelor thesis about a UEFI Rookit communitcating witht the TPM module in an effort to gain trusted information from the windows boot process.

# UEFI

<!--
- https://en.wikipedia.org/wiki/Unified_Extensible_Firmware_Interface
- https://uefi.org/sites/default/files/resources/PI_Spec_1_7_A_final_May1.pdf
- https://uefi.org/sites/default/files/resources/UEFI_Spec_2_7.pdf
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/31_basic_programming_model
-->

<!--
## Acronyms
Unified Extensible Firmware Interface (UEFI)
Platform Initialization (PI)
-->

## UEFI/PI Firmware Images

<!--
https://edk2-docs.gitbook.io/edk-ii-build-specification/2_design_discussion/22_uefipi_firmware_images
 -->

Firmware Images are stored in Flash Devices (FD), a Firmware Volume (FV) serves as file level interface. Usually multiple FVs are present in a single FD but a signle FV can also be distributed via multiple FDs. A FV is formatted with a binary file system, typically with Firmware File System (FFS).

In a FFS modules are stored as files, they can be executed at the fixed address from Read Only Memory (ROM) or through relocation in loaded memory. Within a file are multiple sections which then contain the "leaf" images. These are for example PE32 images.


Below is an image from the open source program [UEFITool](https://github.com/LongSoft/UEFITool), which lets you view the FFS of a FD. I opened an OVMF.fd built from the EDK II repository.

![UEFI Tool latest EDKII OVMF](doc/images/uefitool.png)

## Images

<!--
https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/readme.7
https://uefi.org/sites/default/files/resources/UEFI_Spec_2_7.pdf (2.1.1)
-->

UEFI Images are files containting executable code, they use a subset of the PE32+ (Microsoft Portable Executable and Common Object File Format Specification) format with a modified header signature. The format comes with relocation tables, this makes it possible that the images can be loaded at non predetermined addresses.

The images come in three different types:
- UEFI Applications
- UEFI Boot Services Drivers
- UEFI Runtime Drivers

Main differences between these types is  how and where they reside in memory. Applications are always unloaded when they return execution while drivers are only unloaded when they return an error code. Boot Services are unloaded after the bootloader calls `ExitBootServices()` while Runtime Drivers remain.

<!--
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
-->

## System Table

The UEFI System Table is an important dat structure, it provides access to system configuration information, boot services, runtime services and protocols.

## Protocols

Protocols provide an interface between two serperately built modules. They are associated with GUIDs and installed onto a handle by drivers.

They are essentially a C structure made up of function pointers and other data structures.

When a protocol is installed this structure is filled with pointers to function implementations within the drivers code. A client then searching with the same GUID and receives (device) handles associated with that protocol. After selecting the desired handle the same structure now is filled with pointers on the client side, allowing to call the function implementations provided by the driver.

![](doc/images/uefi_spec_protocols.png)

<!--
## Platform initialization

- https://edk2-docs.gitbook.io/edk-ii-build-specification/2_design_discussion/23_boot_sequence
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/readme.15
-->

# Current Research

<!--
- https://www.kb.cert.org/vuls/id/758382
- https://www.kb.cert.org/vuls/id/976132
- https://www.kb.cert.org/vuls/id/631788
- https://github.com/hackedteam/vector-edk
- https://github.com/Mattiwatti/EfiGuard
- https://github.com/quarkslab/dreamboot
-->

## LoJax

<!--
- https://www.welivesecurity.com/wp-content/uploads/2018/09/ESET-LoJax.pdf
-->

The LoJax rootkit works only on misconfigured or older systems, this is because it is written directly into the firmware's SPI flash with an exploit that is was fixed with the introducation of the Platform Controller Hub chipsets in 2008. The rootkit dumps the current firmware into an image and places malicious drivers into the Firmware File Sytstem (FFS). These drivers register Notify functions on the event `EFI_EVENT_GROUP_READY_TO_BOOT` which is signaled right before a bootloader chooses a boot device. When the Notify callback function is called it searches for windows partitions and installs user level software in their windows installations. For this it comes with it's own NTFS driver and removes potentially conflicting drivers already in the FFS.

Since the malicious component resides in the SPI flash memory a hard drive wipe or replacement can't restore the system integrity.

## MosaicRegressor

<!--
- https://www.bleepingcomputer.com/news/security/mosaicregressor-second-ever-uefi-rootkit-found-in-the-wild/
- https://media.kasperskycontenthub.com/wp-content/uploads/sites/43/2020/10/07080558/MosaicRegressor_Technical-details.pdf
- https://securelist.com/mosaicregressor/98849/
- https://github.com/hackedteam/vector-edk
-->

The MosaicRegressor rootkit is similar to LoJax as it also resides in the FFS. It is mostly based on a [bootkit from Hacking Team](https://github.com/hackedteam/vector-edk) which also registers a Notify function on the `EFI_EVENT_GROUP_READY_TO_BOOT` event and also comes with an NTFS driver.

## FinSpy

<!--
https://securelist.com/finspy-unseen-findings/104322/
 -->

The FinSpy rootkit is, contrary to LoJax' and MosaicRegressor's approach of residing in the FFS, an EFI System Partition rootkit. It replaces the original windows boot manager which loads the original and patches it before execution. The patched version spawns another thread installing mallicious user level software.
## ESPecter

<!--
https://www.welivesecurity.com/2021/10/05/uefi-threats-moving-esp-introducing-especter-bootkit/
 -->

The ESPEcter rootkit is similar to FinSpy also a EFI Sistem Partition based rootkit. It directly patches the windows boot manager allowing it to install windows drivers while bypassing the Windows Driver Signature Enforcement (DSE).

## MoonBounce

<!--
https://securelist.com/moonbounce-the-dark-side-of-uefi-firmware/105468/
 -->

The MoonBounce rootkit is like LoJax and MosaicRegressor an SPI flash rootkit. Instead of installing malicious UEFI drivers it hooks functions in the core of the Driver Execution Environment (DXE) of the firmware. This allows it to introduce mallicious drivers to the windows kernel address space which in turn install user level software.

<!--
## DerStarke QuarkMatter

https://wikileaks.org/ciav7p1/cms/page_26968082.html
https://wikileaks.org/ciav7p1/cms/page_21561434.html
-->

# Library Comparison

## TianoCore EDK II

https://github.com/tianocore/edk2

The first Open source implementation of the UEFI specification.

## Project Mu

https://github.com/microsoft/mu/

Microsoft fork of EDK II with a focus of small deployment cycles.


## GNU-EFI

https://sourceforge.net/p/gnu-efi/code/ci/master/tree/

A small implementation using the native GNU tool chain, which only supports building applications.

## POSIX-UEFI

https://gitlab.com/bztsrc/posix-uefi

A small library inspired by GNU-EFI and even smaller in size.

<!--
# Rootkit

- https://docs.microsoft.com/en-us/windows/security/information-protection/secure-the-windows-10-boot-process
-->

# Implementation

<!--
- https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
- https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Platforms
- https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documents
- https://github.com/tianocore/tianocore.github.io/wiki/UEFI-EDKII-Learning-Dev
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/2_uefi_driver_implementation_checklist/22_how_to_implement_features_in_edk_ii
- https://stackoverflow.com/questions/63725239/build-edk2-in-linux
- https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-Writing-Simple-Application
- https://edk2-docs.gitbook.io/edk-ii-build-specification/
-->

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
- https://uefi.org/learning_center/papers/
- https://docs.fedoraproject.org/en-US/Fedora/18/html-single/UEFI_Secure_Boot_Guide/index.html#idm59538656
- https://wiki.ubuntu.com/UEFI/SecureBoot
- https://github.com/rhboot/shim

<!--
# Writer Guide Broken:
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/34_handle_database
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/readme.8
- https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/appendix_b_edk_ii_sample_drivers
-->