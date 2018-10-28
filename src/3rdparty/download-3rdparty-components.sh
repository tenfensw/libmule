#!/bin/sh
# This will download third-party components

if test "$1" = "clean"; then
	rm -r -f ./.stamp* ./.stamp
	rm -r -f ./*/
	exit 0
fi

DOWNLOAD_CMD=wget

if command -v aria2c > /dev/null 2>&1; then
	DOWNLOAD_CMD="aria2c -o"
elif command -v curl > /dev/null 2>&1; then
	DOWNLOAD_CMD="curl -o"
elif command -v wget > /dev/null 2>&1; then
	DOWNLOAD_CMD="wget -O"
else
	echo "[ERROR] No supported download managers were found."
	exit 1
fi

COMPONENTS="ev3api=https://github.com/c4ev3/EV3-API/archive/master.zip"
if test -e "./.stamp"; then
	echo "[INFO] Everything was already downloaded"
	exit 0
fi
for Component in $COMPONENTS; do
	CNAME=`echo "$Component" | cut -d '=' -f1`
	DLURL=`echo "$Component" | cut -d '=' -f2`
	STAMPNAME=`pwd`
	STAMPNAME="$STAMPNAME/.stamp_$CNAME"
	echo "[INFO] Downloading component \"$CNAME\" from \"$DLURL\""
	CNAME=`basename "$DLURL"`
	if $DOWNLOAD_CMD "$CNAME" "$DLURL"; then
		echo "[INFO] Download finished, trying to unpack it"
	else
		echo "[ERROR] Download failed!"
		exit 2
	fi
	if test `echo "$CNAME" | cut -d '.' -f2` = "zip"; then
		if command -v unzip > /dev/null 2>&1; then
			unzip "$CNAME"
		elif command -v bsdtar > /dev/null 2>&1; then
			bsdtar -xvf "$CNAME"
		elif command -v 7z > /dev/null 2>&1; then
			7z x "$CNAME"
		else
			echo "[ERROR] No unpacking utils for archive type \"zip\" were found."
			exit 3
		fi
	elif test `echo "$CNAME" | cut -d '.' -f2` = "tar"; then
		if tar -xvzf "$CNAME"; then
			echo "[INFO] This was a gzip archive"
		elif tar -xvJf "$CNAME"; then
			echo "[INFO] This was a lzma archive"
		elif tar -xvjf "$CNAME"; then
			echo "[INFO] This was a bzip2 archive"
		elif tar -xvf "$CNAME"; then
			echo "[INFO] This was a tar archive"
		elif bsdtar -xvf "$CNAME"; then
			echo "[INFO] This was a BSD tar archive"
		else
			echo "[ERROR] No unpacking utils could unpack this archive"
			exit 4
		fi
	else
		echo "[ERROR] Internal script error"
		exit 4
	fi
	rm -r -f "$CNAME"
	touch "$STAMPNAME"
done
echo "[INFO] Done"
touch ./.stamp
exit 0
