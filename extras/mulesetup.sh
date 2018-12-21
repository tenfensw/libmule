#!/bin/bash
# mulesetup.sh - libMule SDK Installer
# Copyright (c) Tim K 2018. Licensed under BSD license.

PREFIX=/opt/timkoisoft/libmule-sdk
CURRENTBRAND="libMule SDK for LEGO Mindstorms EV3"
CURRENTOS=undefined
CURRENTARCH=undefined
XMESSAGE=echo
ACCEPTLICENSE=no
PACKAGESELECTION=""
OPTIONALPACKAGES=yes
NOADDITIONALPROMPTS=no
REPO=""
BASEDIR=`dirname "$0"`
BASEDIR=`sh -c "cd \"$BASEDIR\" && pwd"`
if test `pwd` != "$BASEDIR"; then
	sh -c "cd \"$BASEDIR\" && $0 $*"
	exit $?
fi

for Arg in $*; do
	if test "$Arg" = "-skiplicense"; then
		ACCEPTLICENSE=yes
	elif test "$Arg" = "-skipoptional"; then
		OPTIONALPACKAGES=no
	elif test "$Arg" = "-quiet"; then
		ACCEPTLICENSE=yes
		OPTIONALPACKAGES=no
		NOADDITIONALPROMPTS=yes
		PACKAGESELECTION=""
	elif test "$Arg" = "-h" || test "$Arg" = "-help" || test "$Arg" = "--help"; then
		echo "Usage: $0 [-quiet/-skipoptional/-skiplicense]"
		exit 0
	else
		echo "WARNING!!! Unknown argument: $Arg"
	fi
done

if test `uname` = "Linux"; then
	CURRENTOS=linux
elif test `uname` = "SunOS"; then
	CURRENTOS=sunsolaris
elif test `uname` = "Darwin"; then
	CURRENTOS=macosx
elif test `uname` = "FreeBSD"; then
	CURRENTOS=freebsd
elif test `uname` = "OpenBSD"; then
	CURRENTOS=openbsd
elif test `uname` = "NetBSD"; then
	CURRENTOS=netbsd
elif uname | grep "BSD" > /dev/null 2>&1; then
	CURRENTOS=4.4bsd
elif test `uname` | grep "CYGWIN" > /dev/null 2>&1; then
	CURRENTOS=msys
	PREFIX=/cygdrive/timkoisoft/libmule-sdk
elif test `uname` | grep "MSYS" > /dev/null 2>&1; then
	CURRENTOS=msys
	PREFIX=/c/timkoisoft/libmule-sdk
elif test `uname` | grep "MINGW" > /dev/null 2>&1; then
	CURRENTOS=msys
	PREFIX=/c/timkoisoft/libmule-sdk
elif test `uname` = "BeOS" || test `uname` = "Haiku"; then
	CURRENTOS=beos
	PREFIX=/boot/home/timkoisoft/libmule-sdk
else
	CURRENTOS=undefined
fi

if test `uname -m` = "i86pc" || test `uname -m` = "i386" || test `uname -m` = "i486" || test `uname -m` = "i586" || test `uname -m` = "i686"; then
	CURRENTARCH=intel32
elif test `uname -m` = "x86_64" || test `uname -m` = "amd64" || test `uname -m` = "x86-64"; then
	CURRENTARCH=amd64
elif uname -m | grep "arm" > /dev/null 2>&1; then
	CURRENTARCH=arm32
elif uname -m | grep "aarch" > /dev/null 2>&1; then
	CURRENTARCH=arm64
elif uname -a | grep "sparc" > /dev/null 2>&1; then
	CURRENTARCH=sparc
elif uname -m | grep "ppc" > /dev/null 2>&1; then
	CURRENTARCH=powerpc
elif test `uname -m` = "m68k"; then
	CURRENTARCH=m68k
else
	CURRENTARCH=undefined
fi
XMESSAGE="$PWD/xmessage/libmule-xmessage-$CURRENTOS-$CURRENTARCH"

if test ! -e "$PWD/packages/$CURRENTOS/$CURRENTARCH/index.icfg" || test ! -e "$XMESSAGE"; then
	echo "Your computer running $CURRENTARCH version of $CURRENTOS is not supported by this build of $CURRENTBRAND"
	exit 1
else
	REPO="$PWD/packages/$CURRENTOS/$CURRENTARCH"
fi

if test `whoami` != "root" && test "$CURRENTOS" != "msys" && test "$CURRENTOS" != "beos"; then
	FULLPATH=`dirname "$0"`
	FULLPATH=`sh -c "cd \"$FULLPATH\" && pwd"`
	FULLPATH=`printf "$FULLPATH/" && basename "$0"` 
	if command -v pfexec > /dev/null 2>&1; then
		pfexec bash $FULLPATH $*
		exit $?
	elif command -v gksu > /dev/null 2>&1; then
		gksu -u root "bash $FULLPATH $*"
		exit $?
	elif command -v gksudo > /dev/null 2>&1; then
		gksudo bash $FULLPATH $*
		exit $?
	elif command -v konsole > /dev/null 2>&1; then
		echo '#!/bin/sh' > /tmp/runmulesetup.sh
		echo "if su -c \"bash $FULLPATH $*\"; then" >> /tmp/runmulesetup.sh
		echo "      rm -r -f \$0 ; exit 0" >> /tmp/runmulesetup.sh
		echo "else" >> /tmp/runmulesetup.sh
		echo "      rm -r -f \$0 ; exit 1" >> /tmp/runmulesetup.sh
		echo "fi" >> /tmp/runmulesetup.sh
		chmod 777 /tmp/runmulesetup.sh
		konsole -e /tmp/runmulesetup.sh
		exit $?
	elif command -v gnome-terminal > /dev/null 2>&1; then
		gnome-terminal -e "su -c \"bash $FULLPATH $*\""
		exit $?
	elif command -v xfce4-terminal > /dev/null 2>&1; then
		xfce4-terminal -e "su -c \"bash $FULLPATH $*\""
		exit $?
	elif command -v lxterminal > /dev/null 2>&1; then
		lxterminal -e "su -c \"bash $FULLPATH $*\""
		exit $?
	elif command -v sakura > /dev/null 2>&1; then
		sakura -e "su -c \"bash $FULLPATH $*\""
		exit $?
	elif command -v qterminal > /dev/null 2>&1; then
		qterminal -e "su -c \"bash $FULLPATH $*\""
		exit $?
	elif command -v xvt > /dev/null 2>&1; then
		xvt -e "su -c \"bash $FULLPATH $*\""
		exit $?
	elif command -v xdg-terminal > /dev/null 2>&1; then
		echo '#!/bin/sh' > /tmp/runmulesetup.sh
                echo "if su -c \"bash $FULLPATH $*\"; then" >> /tmp/runmulesetup.sh
                echo "      rm -r -f \$0 ; exit 0" >> /tmp/runmulesetup.sh
                echo "else" >> /tmp/runmulesetup.sh
                echo "      rm -r -f \$0 ; exit 1" >> /tmp/runmulesetup.sh
                echo "fi" >> /tmp/runmulesetup.sh
		xdg-terminal /tmp/runmulesetup.sh
		exit $?
	else
		$XMESSAGE -center "Please re-run this installer as root."
		exit 8
	fi
fi

if command -v tar > /dev/null 2>&1; then
	echo "tar found"
else
	echo "tar not found"
	$XMESSAGE -center "GNU tar is not installed. Please install it and re-run this installer."
	exit 5
fi
if command -v gzip > /dev/null 2>&1; then
	echo "gzip found"
else
	echo "gzip not found"
	$XMESSAGE -center "GNU gzip is not installed. Please install it and re-run this installer."
	exit 6
fi

if test "$ACCEPTLICENSE" = "no"; then
	if test -e "$PWD/licenseinfo/beta.txt"; then
		$XMESSAGE -center -file "$PWD/licenseinfo/beta.txt"
	fi
	if $XMESSAGE -center -buttons agree:0,disagree:1 -file "$PWD/licenseinfo/lgpl.txt"; then
		ACCEPTLICENSE=yes
	fi
	if test "$ACCEPTLICENSE" = "no"; then
		$XMESSAGE -center "If you disagree with the licensing terms, then you may not install $CURRENTBRAND on your computer. Installation aborted."
		exit 2
	fi
else
	echo "Skip the license part"
fi

INSTALLEDPKGFILES=""
while read fileline; do
	echo "$fileline"
	HALFONE=`echo "$fileline" | cut -d '=' -f1`
	if test "$fileline" | grep "#" > /dev/null 2>&1; then
		echo "this is a comment, skipping"
	else
		HALFTWO=`echo "$fileline" | cut -d '=' -f2`
		PKGNAME=`echo "$HALFONE" | cut -d ':' -f1`
		PKGIMPORTANCE=`echo "$HALFONE" | cut -d ':' -f2`
		if test "$PKGIMPORTANCE" = "optional"; then
			if test "$OPTIONALPACKAGES" = "yes"; then
				if $XMESSAGE -center -buttons install:0,skip:1 "Would you like to install $PKGNAME?"; then
					INSTALLEDPKGFILES="$INSTALLEDPKGFILES $HALFTWO"
				fi
			else
				echo "Skip $PKGNAME, because it is optional"
			fi
		else
			INSTALLEDPKGFILES="$INSTALLEDPKGFILES $HALFTWO"
		fi
	fi
done < "$REPO/index.icfg"


for PendingInstallFile in $INSTALLEDPKGFILES; do
	if test ! -e "$REPO/$PendingInstallFile"; then
		$XMESSAGE -center "Corrupt installation package - file $PendingInstallFile is not found in $REPO"
		exit 3
	fi
done

if test "$NOADDITIONALPROMPTS" = "yes" || $XMESSAGE -center -buttons yes:0,no:1 "So, the setup is ready to install $CURRENTBRAND into $PREFIX. This might take a while, depending on the package size and the speed of your hard drive. During the installation, you might be prompted to enter the admin password. Continue?"; then
	echo "user said yes"
else
	echo "user said no"
	exit 4
fi

if test ! -e "$PREFIX"; then
	mkdir -p "$PREFIX"
fi

for InstallFile in $INSTALLEDPKGFILES; do
	echo "package $InstallFile"
	INFILEBASENAME="$InstallFile"
	if cp "$REPO/$InstallFile" "/tmp/$INFILEBASENAME" && tar -C "$PREFIX" -xzf "/tmp/$INFILEBASENAME" && rm -r -f "/tmp/$INFILEBASENAME"; then
		echo "install successful"
		if test -e "$PREFIX/postinstall.sh"; then
			sh "$PREFIX/postinstall.sh"
			rm -r -f "$PREFIX/postinstall.sh"
		fi
	else
		echo "install failed"
		$XMESSAGE -center "Installation failed, check if you have free space left on the drive."
		exit 7
	fi

done

$XMESSAGE -center "Installation complete, thank you for installing $CURRENTBRAND!"
exit 0
