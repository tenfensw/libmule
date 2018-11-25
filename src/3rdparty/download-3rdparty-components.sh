
#!/bin/bash
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

COMPONENTS="ev3api=https://github.com/c4ev3/EV3-API pigpio=https://github.com/joan2937/pigpio ArduinoSTL=https://dl.dropboxusercontent.com/s/iisdyd8so9zhl30/ArduinoSTL-1.0.3-workswithlibmule.zip"
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
		if test `echo "$CNAME" | cut -d '.' -f2` = "zip"; then
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
		else
			echo "[ERROR] Internal script error"
			exit 4
		fi 
		rm -r -f "$CNAME" 
	fi 
	touch "$STAMPNAME"
done
echo "[INFO] Done"
touch ./.stamp
exit 0
