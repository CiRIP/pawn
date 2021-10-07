@echo off

@echo Building sprites...
for %%x in (.\src\assets\*.png) do magick stream -map bgra -storage-type char %%x .\src\assets\%%~nx.bgra

@echo Assembling...
fasm .\src\main.asm .\build\EFI\Boot\BOOTX64.EFI

if %errorlevel% neq 0 exit /b %errorlevel%

copy .\build\EFI\BOOT\BOOTX64.EFI D:\EFI\BOOT\

@echo Running...
cd debugger
.\qemu.exe -drive file=OVMF_CODE-need-smm.fd,if=pflash,format=raw,unit=0,readonly=on^
           -drive file=OVMF_VARS-need-smm.fd,if=pflash,format=raw,unit=1^
           -drive file=fat:rw:..\build\,media=disk,if=virtio,format=raw^
           -m 512 -machine q35,smm=on -nodefaults -vga std^
           -global driver=cfi.pflash01,property=secure,value=on^
           -global ICH9-LPC.disable_s3=1^
           -usb -usbdevice mouse
