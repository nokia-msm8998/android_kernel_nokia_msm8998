#!/bin/bash

export ARCH=arm64
export SUBARCH=arm64
export HEADER_ARCH=arm64
export CROSS_COMPILE=/home/log1cs/eva/eva-arm64/bin/aarch64-elf-
export CROSS_COMPILE_ARM32=/home/log1cs/eva/eva-arm/bin/arm-eabi-

DEFCONFIG_FILE=$1
MAKE_MRPROPER=$2

if [ -z "$DEFCONFIG_FILE" ];
then
        echo "Script requires defconfig file (lineageos_codename_defconfig)!"
        exit -1
fi

if [ ! -e arch/arm64/configs/$DEFCONFIG_FILE ];
then
        echo "Config file not available in directory: arch/arm64/configs/$DEFCONFIG_FILE"
        exit -1
fi

if [ $2 -eq 1 ];
then
        echo "Running make mrproper..."
        make mrproper
fi

# Setup the environment
make ${DEFCONFIG_FILE} O=out

echo "Build operation starts!"

# Build the kernel
make O=out -j12

echo "Zipping kernel..."
# Move the kernel to AnyKernel folder
mv out/arch/arm64/boot/Image.gz-dtb anykernel
cd anykernel
zip -r Lycoris.zip .
cd ../
echo "Done!"
