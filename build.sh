#!/bin/bash

export ARCH=arm64
export SUBARCH=arm64
export HEADER_ARCH=arm64
export PATH="/home/log1cs/clang/bin:$PATH"
export STRIP="/home/log1cs/clang/bin/aarch64-linux-gnu/bin/strip"

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
make -j12 LLVM=1 LD=ld.lld HOSTLD=ld.lld CC=clang O=out ARCH=arm64 AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump \
     STRIP=llvm-strip CROSS_COMPILE=aarch64-linux-gnu- CROSS_COMPILE_ARM32=arm-linux-gnueabi- | tee error.log

echo "Zipping kernel..."
# Move the kernel to AnyKernel folder
mv out/arch/arm64/boot/Image.gz-dtb anykernel
cd anykernel
zip -r Lycoris.zip .
cd ../
echo "Done!"
