#!/bin/bash
# download-3rdparty-components.sh - This bash script will download third-party components
# This file is a part of libMule - Microcontroller-Universal 
# Library (that is extendable)
# Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#  
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
# 02110-1301  USA

if test "$1" = "clean"; then
	rm -r -f ./.stamp* ./.stamp
	rm -r -f ./*/
	exit 0
elif test "$1" = "list"; then
	echo "all"
	echo "crucial"
	exit 0
fi

TARGET="$1"
if test "$TARGET" = ""; then
	TARGET=crucial
fi
if test "$TARGET" != "crucial" && test "$TARGET" != "all"; then
	echo "Unknown target: $TARGET. Run \"$0 list\" to get a list of supported targets."
	exit 1
fi
OPTIONALONLY="ev3duder avrdude"
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

COMPONENTS="ev3api=https://github.com/c4ev3/EV3-API pigpio=https://github.com/joan2937/pigpio ArduinoCoreSlim=https://dl.dropboxusercontent.com/s/xvdtr0vkwsmhtae/ArduinoCore-avr-slim-29112018-release2.tar.gz ev3duder=https://dl.dropboxusercontent.com/s/hzrun77ak51scz2/ev3duder-snapshot-04_01_2019.tar.gz avrdude=https://dl.dropboxusercontent.com/s/4hqpnpw6h7367g4/avrdude-6.3.1mule.tgz"
if test -e "./.stamp"; then
	echo "[INFO] Everything was already downloaded"
	exit 0
fi
for Component in $COMPONENTS; do
	CNAME=`echo "$Component" | cut -d '=' -f1`
	DLURL=`echo "$Component" | cut -d '=' -f2`
	DOSKIP=no
	for OptComp in $OPTIONALONLY; do
		if test "$OptComp" = "$CNAME"; then
			DOSKIP=yes
			break
		fi
	done
	if test "$DOSKIP" = "yes" && test "$TARGET" != "all"; then
		echo "[INFO] Skip $CNAME"
	else
		STAMPNAME=`pwd`
		STAMPNAME="$STAMPNAME/.stamp_$CNAME"
		echo "[INFO] Downloading component \"$CNAME\" from \"$DLURL\""
		CNAME=`basename "$DLURL"`
		if echo "$DLURL" | grep -q "github.com"; then
			if command -v git > /dev/null 2>&1; then
				if git clone "$DLURL" && mv -v `basename "$DLURL"` `basename "$DLURL"`-master/; then
					echo "[INFO] git clone finished successfully"
				else
					echo "[ERROR] Download failed!"
					exit 2
				fi
			else
				echo "[ERROR] Git is not installed."
				exit 5
			fi
		else
			if $DOWNLOAD_CMD "$CNAME" "$DLURL"; then
				echo "[INFO] Download finished, trying to unpack it"
			else
				echo "[ERROR] Download failed!"
				exit 2
			fi
			if echo "$CNAME" | grep -q ".zip"; then
				if command -v bsdtar > /dev/null 2>&1; then
					bsdtar -xvf "$CNAME"
				elif command -v unzip > /dev/null 2>&1; then
					unzip "$CNAME"
				elif command -v 7z > /dev/null 2>&1; then
					7z x "$CNAME"
				else
					echo "[ERROR] No unpacking utils for archive type \"zip\" were found."
					exit 3
				fi
			elif echo "$CNAME" | grep -q ".tar"; then
				tar -xvf "$CNAME" || tar -xvjf "$CNAME" || tar -xvzf "$CNAME" || tar -xvJf "$CNAME" || bsdtar -xvf "$CNAME" || exit 5
			elif echo "$CNAME" | grep -q ".tgz"; then
				tar -xvzf "$CNAME" || bsdtar -xvzf "$CNAME" || exit 98
			else
				echo "[ERROR] Internal script error"
				exit 4
			fi 
			rm -r -f "$CNAME" 
		fi 
		touch "$STAMPNAME"
	fi
done
echo "[INFO] Done"
touch ./.stamp
exit 0
