#!/bin/sh
if test "$PREFIX" = ""; then
	PREFIX=/opt/timkoisoft/mule
fi

if test `whoami` != "root"; then
	echo "[ERROR] Not running as root, cannot continue, attempting to relaunch using su/pfexec/sudo"
	su -c "$0 $*" || sudo sh -c "$0 $*" || pfexec sh -c "$0 $*" || exit 1
	exit 0
fi

for DirStruct in $PREFIX/include $PREFIX/include/libmule $PREFIX/include/libmule/core $PREFIX/include/libmule/platformsupport/common $PREFIX/include/libmule/platformsupport $PREFIX/include/libmule/platformsupport/legoev3 $PREFIX/include/libmule/platformsupport/dummy $PREFIX/include/libmule/3rdparty  $PREFIX/include/libmule/3rdparty/EV3-API-master/API $PREFIX/lib; do
	if test ! -e "$DirStruct"; then
		printf "creating directory $DirStruct... "
		if mkdir -p "$DirStruct"; then
			printf "done!\n"
		else
			printf "failed!\n"
			exit 2
		fi
	else
		echo "directory $DirStruct already exists"
	fi
done

SRCDIR=`pwd`
SRCDIR="$SRCDIR/src/"
for File in $(find "$SRCDIR" -type f -name "*.h"); do
	SHORTFILE=`echo "$File" | sed "s+$SRCDIR++g"`
	cp "$File" $PREFIX/include/libmule/$SHORTFILE
done

cp libMule.a "$PREFIX/lib/libMule.a"
