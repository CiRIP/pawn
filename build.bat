@echo off

@echo Building sprites...
for %%x in (.\src\assets\*.png) do magick stream -map bgra -storage-type char %%x .\src\assets\%%~nx.bgra

@echo Assembling...
fasm .\src\main.asm .\build\EFI\Boot\BOOTX64.EFI

if %errorlevel% neq 0 exit /b %errorlevel%

copy .\build\EFI\BOOT\BOOTX64.EFI D:\EFI\BOOT\

@echo Running...
@REM cd debugger
@REM .\qemu.exe -drive file=OVMF_CODE-need-smm.fd,if=pflash,format=raw,unit=0,readonly=on^
@REM            -drive file=OVMF_VARS-need-smm.fd,if=pflash,format=raw,unit=1^
@REM            -drive file=fat:rw:..\build\,media=disk,if=virtio,format=raw^
@REM            -m 512 -machine q35,smm=on -nodefaults -vga std^
@REM            -global driver=cfi.pflash01,property=secure,value=on^
@REM            -global ICH9-LPC.disable_s3=1^
@REM            -usb -usbdevice mouse^
@REM            -nic user,model=virtio-net-pci

start "" "c:\Program Files\qemu\qemu-system-x86_64.exe"^
        -bios ovmf-x64\OVMF-pure-efi.fd^
        -drive file=fat:rw:build\,media=disk,if=virtio,format=raw^
        -usb -usbdevice mouse^
        -netdev tap,id=pawnnet0,ifname=pawn0^
        -device virtio-net-pci,netdev=pawnnet0,mac=52:54:00:00:00:00

start "" "c:\Program Files\qemu\qemu-system-x86_64.exe"^
        -bios ovmf-x64\OVMF-pure-efi.fd^
        -drive file=fat:rw:build\,media=disk,if=virtio,format=raw^
        -usb -usbdevice mouse^
        -netdev tap,id=pawnnet0,ifname=pawn1^
        -device virtio-net-pci,netdev=pawnnet0,mac=52:54:00:00:00:01
