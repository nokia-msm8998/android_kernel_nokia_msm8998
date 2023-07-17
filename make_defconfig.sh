#!/bin/bash

DEFCONFIG_FILE=$1
COMMIT=$2

if [ -z "$DEFCONFIG_FILE" ]; then
	echo "Script requires defconfig file (codename_defconfig)!"
	exit -1
fi

if [ ! -e arch/arm64/configs/$DEFCONFIG_FILE ]; then
	echo "Config file not available in directory: arch/arm64/configs/$DEFCONFIG_FILE"
	exit -1
fi

# make .config
make ARCH=arm64 ${DEFCONFIG_FILE} O=out

echo "Regenerating defconfig..."

# copy .config to defconfig
mv out/.config arch/arm64/configs/${DEFCONFIG_FILE}

if [ $2 -eq 1 ];
then
	# Commit everything.
	git add arch/arm64/configs/
	git commit -m "arm64: configs: Regenerate"
fi

echo "Done!"	
