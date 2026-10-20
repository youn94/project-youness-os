@echo off
setlocal

echo ===========================
echo Compilation du bootloader
echo ===========================

nasm -f bin bootloader.asm -o boot.bin
if errorlevel 1 goto error

echo ===========================
echo Compilation du kernel ASM
echo ===========================

nasm -f elf32 kernel.asm -o entry.o
if errorlevel 1 goto error


echo ===========================
echo Compilation du pilote video
echo ===========================

i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -mgeneral-regs-only -Ikernel/video -c kernel/video/video.c -o video.o
if errorlevel 1 goto error

echo ===========================
echo Compilation du pilote inout
echo ===========================

i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -mgeneral-regs-only -Ikernel/inout -c kernel/inout/inout.c -o inout.o
if errorlevel 1 goto error

echo ===========================
echo Compilation du pilote pci
echo ===========================

i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -mgeneral-regs-only -Ikernel/pci -c kernel/pci/pci.c -o pci.o
if errorlevel 1 goto error

echo ===========================
echo Compilation du pilote pc2
echo ===========================

i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -mgeneral-regs-only -Ikernel/ps2 -c kernel/ps2/ps2.c -o ps2.o
if errorlevel 1 goto error


echo ===========================
echo Compilation du kernel C
echo ===========================

i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -mgeneral-regs-only -c kernel.c -o kernel.o
if errorlevel 1 goto error

echo ===========================
echo Link du kernel
echo ===========================

i686-elf-gcc -m32 -T linker.ld -ffreestanding -nostdlib -no-pie entry.o kernel.o video.o inout.o pci.o ps2.o -lgcc -o kernel.elf
if errorlevel 1 goto error

echo ===========================
echo Conversion en binaire
echo ===========================

i686-elf-objcopy -O binary kernel.elf kernel.bin
if errorlevel 1 goto error

echo ===========================
echo Padding de kernel.bin
echo ===========================

REM Le bootloader lit 0x20 (32) secteurs = 16384 octets a partir du secteur 2.
REM On complete kernel.bin avec des zeros jusqu'a cette taille minimum, sinon
REM le disque ne contient pas assez de secteurs et int 0x13 lit hors du fichier
REM (kernel charge tronque / corrompu, ou erreur de lecture disque).

REM Le bootloader lit 0x30 (48) secteurs = 24576 octets a partir du secteur 2.
set MINSIZE=24576

powershell -NoProfile -Command ^
    "$path = 'kernel.bin';" ^
    "$min = %MINSIZE%;" ^
    "$size = (Get-Item $path).Length;" ^
    "Write-Host ('Taille kernel.bin avant padding : ' + $size + ' octets (' + [math]::Ceiling($size/512) + ' secteurs)');" ^
    "if ($size -lt $min) {" ^
    "    $pad = $min - $size;" ^
    "    $fs = [System.IO.File]::Open($path, 'Append');" ^
    "    $fs.Write((New-Object byte[] $pad), 0, $pad);" ^
    "    $fs.Close();" ^
    "    Write-Host ('Padding ajoute : ' + $pad + ' octets -> nouvelle taille ' + $min + ' octets (' + ($min/512) + ' secteurs)');" ^
    "} else {" ^
    "    $rem = $size %% 512;" ^
    "    if ($rem -ne 0) {" ^
    "        $pad = 512 - $rem;" ^
    "        $fs = [System.IO.File]::Open($path, 'Append');" ^
    "        $fs.Write((New-Object byte[] $pad), 0, $pad);" ^
    "        $fs.Close();" ^
    "        Write-Host ('ATTENTION: kernel.bin depasse deja ' + $min + ' octets.');" ^
    "        Write-Host ('Aligne sur secteur avec ' + $pad + ' octets de padding -> ' + ($size+$pad) + ' octets.');" ^
    "        Write-Host ('Il faut augmenter la valeur AL dans bootloader.asm pour lire assez de secteurs !');" ^
    "    } else {" ^
    "        Write-Host ('ATTENTION: kernel.bin depasse deja ' + $min + ' octets et est deja aligne.');" ^
    "        Write-Host ('Il faut augmenter la valeur AL dans bootloader.asm pour lire assez de secteurs !');" ^
    "    }" ^
    "}"

if errorlevel 1 goto error

echo ===========================
echo Creation de l'image disque
echo ===========================

copy /b boot.bin+kernel.bin os.bin >nul

qemu-system-i386 -boot a -fda os.bin ^
  -device qemu-xhci,id=xhci,bus=pci.0,addr=05.0 
  -device usb-kbd,bus=xhci.0
echo.
echo ===== Build termine =====
pause
exit

:error
echo.
echo ===== ERREUR =====
pause
