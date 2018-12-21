#!/bin/bash
# mkinstaller.sh - create a libMule SDK installer in this directory
# Copyright (c) Tim K 2018. Licensed under GNU LGPLv2.1

safecopy() {
	echo "copy \"$1\" -> \"$2\""
	if cp -r "$1" "$2"; then
		echo "copied 1 file successfully"
		return 0
	else
		echo "failed to copy 1 file"
		return 1
	fi
}

getcorecount() {
	if test "$CURRENTOS" = "linux"; then
		echo `cat /proc/cpuinfo | grep processor | wc -l`
	elif test "$CURRENTOS" = "freebsd" || test "$CURRENTOS" = "openbsd" || test "$CURRENTOS" = "macosx"; then
		echo `sysctl hw.ncpu | cut -d ':' -f2 | sed 's/ //g'`
	else
		echo 2
	fi	
}

checkapp() {
	if command -v $1 > /dev/null 2>&1; then
		printf "$1 found ("
		printf `command -v $1`
		printf ")"
		echo ""
	else
		echo "$1 not found"
		return 1
	fi
	return 0
}

checksystem() {
	EVERYTHINGISINSTALLED=yes
	for Component in g++ make tar gunzip bunzip2 unxz git; do
		if checkapp $Component; then
			sleep $SLEEPTIME
		else
			EVERYTHINGISINSTALLED=no
		fi
	done
	if test "$EVERYTHINGISINSTALLED" = "no"; then
		echo "Not all required components are installed, please install them before continuing"
		exit 1
	fi
}

showhelp() {
	echo "Usage: $0 [-platform=<legoev3, pigpio_rpi or dummy>/-qt=<4 or 5>/-dl=<downloader command that dumps output to stdin>/-force-portablexmessage]"
	echo "Also, setting \$MULECC, \$MULECXX and \$MULEAR variables might be a good idea, especially if you are building libMule not for the dummy platform."
}

beautifulheader() {
	echo "This is mkinstaller.sh $TOOLVERSION"
}

TOOLVERSION=0.4
GIT_LIBMULE="https://gitlab.com/timkoi/libmule"
GIT_MULESTUDIO="https://gitlab.com/timkoi/mulestudio"
DL_QT4="http://mirrors.dotsrc.org/qtproject/archive/qt/4.8/4.8.7/qt-everywhere-opensource-src-4.8.7.tar.gz"
DL_QT5="http://mirrors.dotsrc.org/qtproject/archive/qt/5.6/5.6.3/submodules/qtbase-opensource-src-5.6.3.tar.xz"
DL_GCCRPI="https://dl.dropboxusercontent.com/s/t42laqka0ov1i1k/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu.tar.xz"
OPT_GCCRPIUNPACK="unxz -"
OPT_QT4UNPACK="gunzip -"
OPT_QT5UNPACK="unxz -"
DL_XMESSAGE="http://old-releases.ubuntu.com/ubuntu/pool/main/x/xmessage/xmessage_1.0.1.orig.tar.gz"
DL_PORTABLEXMESSAGE="https://gitlab.com/timkoi/libmule/raw/master/extras/portablexmessage"
OPT_XMESSAGEUNPACK="$OPT_QT4UNPACK"
DL_MAKESELF="http://old-releases.ubuntu.com/ubuntu/pool/universe/m/makeself/makeself_2.2.0.orig.tar.gz"
OPT_MAKESELFUNPACK="$OPT_XMESSAGEUNPACK"
CURRENTOS=undefined
CURRENTARCH=undefined
XORGCOMPATIBLE=no
USEQT=4
USEDDOWNLOADER="echo"
MULE_MICROCONTROLLER=dummy
SLEEPTIME=1
USEPXMESSAGE=no
PREFIX=/opt/timkoisoft/libmule-sdk
LAUNCHSTRING=`date +"%Y%m%d%H%M%S"`

beautifulheader
if echo "$*" | grep "\-help" > /dev/null 2>&1; then
	showhelp
	exit 0
fi

if test `uname` = "Linux"; then
	CURRENTOS=linux
	XORGCOMPATIBLE=yes
	SLEEPTIME="0.1"
elif test `uname` = "FreeBSD"; then
	CURRENTOS=freebsd
	XORGCOMPATIBLE=yes
elif test `uname` = "OpenBSD"; then
	CURRENTOS=openbsd
	XORGCOMPATIBLE=yes
elif (uname | grep "BSD" > /dev/null 2>&1) || test `uname -s` = "DragonFly"; then
	CURRENTOS=4.4bsd
	XORGCOMPATIBLE=yes
elif test `uname` = "Darwin"; then
	CURRENTOS=macosx
	XORGCOMPATIBLE=no
elif uname | grep "MSYS" > /dev/null 2>&1; then
	CURRENTOS=msys
	XORGCOMPATIBLE=no
elif uname | grep "CYGWIN" > /dev/null 2>&1; then
	CURRENTOS=msys
	XORGCOMPATIBLE=no
elif uname | grep "MINGW" > /dev/null 2>&1; then
	CURRENTOS=msys
	XORGCOMPATIBLE=no
elif test `uname` = "SunOS"; then
	CURRENTOS=sunsolaris
	XORGCOMPATIBLE=yes
elif test `uname` = "BeOS" || test `uname` = "Haiku"; then
	CURRENTOS=beos
	XORGCOMPATIBLE=no
	SLEEPTIME="0.1"
else
	CURRENTOS=undefined
	XORGCOMPATIBLE=no
fi

if test `uname -m` = "i386" || test `uname -m` = "i486" || test `uname -m` = "i586" || test `uname -m` = "i686" || test `uname -m` = "i86pc"; then
	CURRENTARCH=intel32
elif test `uname -m` = "amd64" || test `uname -m` = "x86_64" || test `uname -m` = "x86-64"; then
	CURRENTARCH=amd64
elif uname -m | grep arm > /dev/null 2>&1; then
	CURRENTARCH=arm32
elif uname -m | grep aarch > /dev/null 2>&1; then
	CURRENTARCH=arm64
elif uname -m | grep ppc > /dev/null 2>&1; then
	CURRENTARCH=powerpc
elif test `uname -m` = "m68k"; then
	CURRENTARCH=m68k
elif uname -a | grep sparc > /dev/null 2>&1; then
	CURRENTARCH=sparc
else
	CURRENTARCH=undefined
fi

for Argument in $*; do
	HALFONE=`echo "$Argument" | cut -d '=' -f1`
	HALFTWO=`echo "$Argument" | cut -d '=' -f2`
	if test "$HALFONE" = "-platform" || test "$HALFONE" = "-target"; then
		MULE_MICROCONTROLLER="$HALFTWO"
	elif test "$HALFONE" = "-dl"; then
		USEDDOWNLOADER="$HALFTWO"
	elif test "$HALFONE" = "-qt"; then
		USEQT=$HALFTWO
	elif test "$HALFONE" = "-force-portablexmessage"; then
		USEPXMESSAGE=yes
	elif test "$HALFONE" = "-clean" || test "$HALFONE" = "clean"; then
		printf "Cleaning up, please wait... "
		rm -r -f "$PWD/packages" "$PWD/qtsdk" "$PWD/qtsrc" "$PWD/tmppkgdir" "$PWD/xmessage" "$PWD/xmessage-src" "$PWD/mulesrc" "$PWD/mulestudiosrc" "$PWD/linarogccunpacked" "$PWD/makeselfsrc" "$PWD/licenseinfo"
		if test "$?" = "0"; then
			echo "done!"
		else
			echo "done, but with some minor errors."
		fi
		exit 0
	else
		echo "Unknown argument: $HALFONE, will continue anyway, though"
	fi
done

FASTMAKE="make"
FASTMAKE="make -j`getcorecount`"
echo "You use $CURRENTARCH-$CURRENTOS"
if test "$CURRENTOS" = "undefined" || test "$CURRENTARCH" = "undefined"; then
	echo "Your OS or processor is not supported."
	exit 2
fi
checksystem
echo "Instead of regular make, \"$FASTMAKE\" will be invoked to speed up compile times"

if test "$CURRENTOS" = "freebsd" && test "$USEDDOWNLOADER" = "echo"; then
	USEDDOWNLOADER="fetch -o -"
fi

if test "$USEDDOWNLOADER" = "echo"; then
	for Downloader in aria2c wget curl; do
		if checkapp $Downloader && sleep $SLEEPTIME; then
			if test "$Downloader" = "wget"; then
				USEDDOWNLOADER="wget -O-"
			elif test "$Downloader" = "aria2c"; then
				USEDDOWNLOADER="aria2c --output -"
			else
				USEDDOWNLOADER="$Downloader"
			fi
		fi
	done
	if test "$USEDDOWNLOADER" = "echo"; then
		echo "No available supported download managers were found, exiting"
		exit 3
	fi
fi

if test "$MULE_MICROCONTROLLER" = "rpi"; then
	MULE_MICROCONTROLLER=pigpio_rpi
fi

if test "$USEQT" != "5" && test "$USEQT" != "4"; then
	echo "Unknown \"-qt=$USEQT\" flag, will reset the \$USEQT value to the default one (Qt 4)"
	USEQT=4
fi

QT5FLAGS="-no-slog2 -no-pps -no-imf -no-lgmon -securetransport -system-proxies  -no-gstreamer -no-libinput -no-opengl -qpa $QTQPAPLATFORM -no-directfb -no-linuxfb -no-mirclient -no-kms -no-gbm -no-eglfs -no-dbus -no-ltcg -no-pch -strip -no-fontconfig -no-icu -no-evdev -no-tslib -no-cups -gui -widgets -no-openssl   -no-compile-examples -no-gtkstyle -no-alsa -no-pulseaudio -no-glib -qt-pcre -qt-harfbuzz -qt-zlib -qt-libjpeg -qt-libpng -qt-freetype -largefile -c++std c++98 -no-openssl -optimized-tools -confirm-license -opensource -static -prefix \"$PWD/qtsdk\" -no-avx2 -no-avx -no-sse4.2 -no-sse4.1 -no-ssse3 -no-sse3 -v"
QT4FLAGS="-no-glib -no-dbus -no-pch -iconv -no-cups -no-nis -optimized-qmake -qt-libmng -qt-libpng -qt-libjpeg -no-openssl -qt-libtiff -qt-zlib -no-gif -no-3dnow -no-avx -no-neon -no-sse4.2 -no-sse4.1 -no-ssse3 -no-sse3 -no-declarative -no-declarative-debug -no-script -no-scripttools -no-webkit -svg -no-phonon-backend -no-phonon -no-audio-backend -multimedia -no-xmlpatterns -no-qt3support -system-proxies -exceptions -fast -static -opensource -confirm-license -prefix \"$PWD/qtsdk\" -v"

QTDLURL="$DL_QT4"
QTUNPACK="$OPT_QT4UNPACK"
QTFLAGS="$QT4FLAGS"
if test "$USEQT" = "5"; then
	QTDLURL="$DL_QT5"
	QTUNPACK="$OPT_QT5UNPACK"
	QTFLAGS="$QT5FLAGS"
fi

QTQPAPLATFORM=xcb
QTQPAFLAGS="-xcb -xcb-xlib -no-xinput2 -system-xcb "
QT4PLFLAGS="-xkb -no-xinput -no-fontconfig -no-mitshm -no-xrandr -no-xcursor -no-xinerama -no-sm -no-openvg -no-opengl -no-gtkstyle"
if test "$XORGCOMPATIBLE" = "no"; then
	QTQPAFLAGS="-no-xcb-xlib -no-xinput2 -no-xcb"
	QT4PLFLAGS="-no-xkb -no-xinput -no-fontconfig -no-mitshm -no-xrandr -no-xcursor -no-xinerama -no-sm -no-openvg -no-opengl -no-gtkstyle"
	if test "$CURRENTOS" = "macosx"; then
		QTQPAPLATFORM=cocoa
	elif test "$CURRENTOS" = "msys"; then
		QTQPAPLATFORM=windows
	elif test "$CURRENTOS" = "beos"; then
		QTQPAPLATFORM=haiku
	else
		echo "Your OS is not supported by Qt5"
		exit 4
	fi
fi
if test "$USEQT" = "5"; then
	QTFLAGS="$QTFLAGS $QTQPAFLAGS -qpa $QTQPAPLATFORM"
else
	QTFLAGS="$QTFLAGS $QT4PLFLAGS"
fi

for DirToRemove in qtsrc xmessage-src mulesrc mulestudiosrc packages linarogccunpacked makeselfsrc licenseinfo; do
	if test -e "$PWD/$DirToRemove"; then
		rm -r -f "$PWD/$DirToRemove"
		echo "removed directory \"$PWD/$DirToRemove\""
	fi
done

mkdir qtsrc
if (test -e "$PWD/qtsdk") || ($USEDDOWNLOADER "$QTDLURL" | $QTUNPACK | tar --strip-components=1 -C ./qtsrc -xf -); then
	echo "Downloaded and unpacked Qt $USEQT"
else
	echo "Did not download or unpack Qt $USEQT"
	exit 5
fi

printf "Configuring Qt $USEQT... "
if (test -e "$PWD/qtsdk") || (sh -c "cd \"$PWD/qtsrc\" && ./configure $QTFLAGS" > ./qtconf.log 2>&1); then
	echo "done!"
else
	echo "failed, here is the log:"
	cat ./qtconf.log
	rm -r -f ./qtconf.log
	echo "Qt $USEQT failed to configure"
	exit 5
fi
rm -r -f ./qtconf.log

printf "Building Qt $USEQT, this might take a while... "
if (test -e "$PWD/qtsdk") || (sh -c "cd \"$PWD/qtsrc\" && $FASTMAKE && make install" > ./qtmake.log 2>&1); then
	echo "done!"
else
	echo "failed, here is the log:"
	cat ./qtmake.log
	rm -r -f ./qtmake.log
	echo "Qt $USEQT failed to build"
	exit 6
fi
rm -r -f ./qtmake.log

if mkdir -p "$PWD/packages/$CURRENTOS/$CURRENTARCH"; then
	echo "created dummy repository \"$PWD/packages/$CURRENTOS/$CURRENTARCH\""
else
	echo "failed to create dummy repository \"$PWD/packages/$CURRENTOS/$CURRENTARCH\""
	exit 7
fi

touch "$PWD/packages/$CURRENTOS/$CURRENTARCH/index.icfg"
echo "touch \"$PWD/packages/$CURRENTOS/$CURRENTARCH/index.icfg\""
REPOINDEX="$PWD/packages/$CURRENTOS/$CURRENTARCH/index.icfg"


mkdir xmessage-src
mkdir xmessage
if test ! -e "$PWD/xmessage/libmule-xmessage-$CURRENTOS-$CURRENTARCH"; then
	if test "$XORGCOMPATIBLE" = "yes" && test "$USEPXMESSAGE" = "no"; then
		if $USEDDOWNLOADER "$DL_XMESSAGE" | $OPT_XMESSAGEUNPACK | tar --strip-components=1 -C "$PWD/xmessage-src" -xvf -; then
			echo "Downloaded XMessage sources"
		else
			echo "Failed to download XMessage sources"
			exit 8
		fi
		if sh -c "cd \"$PWD/xmessage-src\" && ./configure --disable-shared --enable-static --with-release-version=mulesetup && $FASTMAKE" > xmessagebl.log 2>&1; then
			echo "Built XMessage"
		else
			echo "Failed to build XMessage, here is the log:"
			cat xmessagebl.log
			rm -r -f xmessagebl.log
			exit 9
		fi
		rm -r -f xmessagebl.log
		cp "$PWD/xmessage-src/xmessage" "$PWD/xmessage/libmule-xmessage-$CURRENTOS-$CURRENTARCH"
	else
		if $USEDDOWNLOADER "$DL_PORTABLEXMESSAGE" > "$PWD/xmessage/libmule-xmessage-$CURRENTOS-$CURRENTARCH"; then
			echo "Downloaded Portable-XMessage"
		else
			echo "Failed to download Portable-XMessage"
			exit 10
		fi
		chmod 777 "$PWD/xmessage/libmule-xmessage-$CURRENTOS-$CURRENTARCH"
	fi
else
	echo "Skip rebuilding XMessage"
fi

if git clone --recursive "$GIT_LIBMULE" mulesrc && git clone --recursive "$GIT_MULESTUDIO" mulestudiosrc; then
	echo "git cloned libmule and mulestudio"
else
	echo "failed to git clone libmule and mulestudio"
	exit 11
fi

POSSIBLEMULECC=cc
POSSIBLEMULECXX=c++
POSSIBLEMULEAR=ar
if test "$MULECC" != ""; then
	POSSIBLEMULECC=$MULECC
fi
if test "$MULECXX" != ""; then
	POSSIBLEMULECXX=$MULECXX
fi
if test "$MULEAR" != ""; then
	POSSIBLEMULEAR=$MULEAR
fi

REPOBD=`dirname "$REPOINDEX"`
MULEADDITIONALCONF=""
if test -e "$PWD/tmppkgdir"; then rm -r -f "$PWD/tmppkgdir"; fi
if test "$MULE_MICROCONTROLLER" = "pigpio_rpi" || test "$DEBUG" != ""; then
	mkdir "$PWD/tmppkgdir"
	mkdir "$PWD/tmppkgdir/tools"
	if $USEDDOWNLOADER "$DL_GCCRPI" | $OPT_GCCRPIUNPACK | tar -C "$PWD/tmppkgdir" -xf -; then
		echo "Downloaded Linaro GCC for Raspberry Pi 3"
	else
		echo "Failed to download Linaro GCC for Raspberry Pi 3"
		exit 17
	fi
	LINAROGCC_PKGSTRING="linarogcc-7.3.1-$LAUNCHSTRING-$MULE_MICROCONTROLLER-$CURRENTOS-$CURRENTARCH.tgz"
	if sh -c "cd \"$PWD/tmppkgdir\" && mv -v ./gcc* ./linarogccrpi && mv -v ./linarogccrpi ./tools/linarogccrpi && tar -czf \"$REPOBD/$LINAROGCC_PKGSTRING\" ./*"; then
		echo "packaged Linaro GCC for Raspberry Pi 3"
	else
		echo "failed to package Linaro GCC for Raspberry Pi 3"
		exit 18
	fi
	echo "linarogcc:required=$LINAROGCC_PKGSTRING" > "$REPOINDEX"
	mv -v "$PWD/tmppkgdir" "$PWD/linarogccunpacked"
	POSSIBLEMULECC="$PWD/linarogccunpacked/tools/linarogccrpi/bin/aarch64-linux-gnu-gcc"
	POSSIBLEMULECXX="$PWD/linarogccunpacked/tools/linarogccrpi/bin/aarch64-linux-gnu-g++"
	POSSIBLEMULEAR="$PWD/linarogccunpacked/tools/linarogccrpi/bin/aarch64-linux-gnu-ar"
fi

if sh -c "cd \"$PWD/mulesrc\" && if test -e \"./bd\"; then rm -r -f ./bd; fi && mkdir bd && cd bd && ../configure -prefix=\"$PREFIX/$TOOLVERSION/$MULE_MICROCONTROLLER\" -platform=$MULE_MICROCONTROLLER -platformcc=\"$POSSIBLEMULECC\" -platformcxx=\"$POSSIBLEMULECXX\" -platformar=\"$POSSIBLEMULEAR\"" > ./mulecfg.log 2>&1; then
	echo "Configured libMule"
else
	echo "Failed to configure libMule for $MULE_MICROCONTROLLER. Here is the log:"
	cat mulecfg.log
	rm -r -f mulecfg.log
	exit 12
fi
rm -r -f mulecfg.log

if sh -c "cd \"$PWD/mulesrc/bd\" && $FASTMAKE && if make RPMBUILDROOT=\"$PWD/tmppkgdir\" install; then exit 0; else exit 0; fi" > mulebl.log 2>&1; then
	echo "Built libMule"
else
	echo "Failed to build libMule. Here is the log:"
	cat mulebl.log
	rm -r -f mulebl.log
	exit 13
fi
rm -r -f mulebl.log
rm -r -f "$PWD/tmppkgdir/include/libmule/*"

MULE_SOURCEDIR="$PWD/mulesrc/src"
for HeaderMuleFile in `find "$MULE_SOURCEDIR" -type f -name "*.h" && find "$MULE_SOURCEDIR" -type f -name "*.hpp"`; do
	RealHeaderMuleFile=`echo "$HeaderMuleFile" | sed "s+$MULE_SOURCEDIR+$PWD/tmppkgdir/$PREFIX/$TOOLVERSION/$MULE_MICROCONTROLLER/include/libmule+g"`
	BaseDirMuleFile=`dirname "$RealHeaderMuleFile"`
	if test ! -e "$BaseDirMuleFile"; then
		mkdir -p "$BaseDirMuleFile"
	fi
	if safecopy "$HeaderMuleFile" "$RealHeaderMuleFile"; then
		sleep $SLEEPTIME
	else
		exit 14
	fi
done
if test "$MULE_MICROCONTROLLER" = "pigpio_rpi"; then
	MULETOOL_MCFG_PATH="$PWD/tmppkgdir/$PREFIX/$TOOLVERSION/$MULE_MICROCONTROLLER/etc/MuleTool.mcfg"
	mv "$MULETOOL_MCFG_PATH" "$MULETOOL_MCFG_PATH.old"
	cat "$MULETOOL_MCFG_PATH.old" | sed "s+$PWD/linarogccunpacked+$PREFIX+g" > "$MULETOOL_MCFG_PATH"
	rm -r -f "$MULETOOL_MCFG_PATH.old"
fi

MULE_PKGSTRING="libmule-$TOOLVERSION-$LAUNCHSTRING-$MULE_MICROCONTROLLER-$CURRENTOS-$CURRENTARCH.tgz"
echo "libmule:required=$MULE_PKGSTRING" >> "$REPOINDEX"
if sh -c "cd \"$PWD/tmppkgdir/$PREFIX\" && tar -czf \"$REPOBD/$MULE_PKGSTRING\" ./*"; then
	echo "packaged $MULE_PKGSTRING"
else
	echo "packaging libmule failed"
	exit 15
fi
rm -r -f "$PWD/tmppkgdir"

mkdir tmppkgdir
if sh -c "cd \"$PWD/mulestudiosrc\" && \"$PWD/qtsdk/bin/qmake\" -r && $FASTMAKE" > ./mulestudiobl.log 2>&1; then
	echo "built MuleStudio"
else
	echo "failed to build MuleStudio, here is the log:"
	cat mulestudiobl.log
	rm -r -f mulestudiobl.log
	exit 16
fi
rm -r -f mulestudiobl.log


mkdir -p tmppkgdir/tools/mulestudio/bin tmppkgdir/tools/mulestudio/share/icons
if safecopy ./mulestudiosrc/mulestudio ./tmppkgdir/tools/mulestudio/bin/mulestudio && safecopy ./mulestudiosrc/resources/icons/mulestudiologo.png ./tmppkgdir/tools/mulestudio/share/icons/mulestudiologo.png; then
	echo "All MuleStudio assets were copied successfully"
else
	echo "Copying the assets of MuleStudio failed"
	exit 17
fi
echo '#!/bin/sh' > "$PWD/tmppkgdir/postinstall.sh"
echo "SHORTCUT=\"/usr/share/applications/ru.timkoisoft.mulestudio.desktop\"" >> "$PWD/tmppkgdir/postinstall.sh"
echo "if test -e \"/usr/local/share/applications/kde4\"; then" >> "$PWD/tmppkgdir/postinstall.sh"
echo "    SHORTCUT=\"/usr/local/share/applications/kde4/ru.timkoisoft.mulestudio.desktop\"" >> "$PWD/tmppkgdir/postinstall.sh"
echo "fi" >> "$PWD/tmppkgdir/postinstall.sh"
echo "if test -e \"\$SHORTCUT\"; then rm -r -f \"\$SHORTCUT\"; fi" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"[Desktop Entry]\" > \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"Name=MuleStudio Integrated Development Environment\" >> \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"GenericName=IDE for microcontollers\" >> \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"Comment=An open-source IDE that allows easy creation and deployment of libMule-based C++ programs\" >> \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"Exec=$PREFIX/tools/mulestudio/bin/mulestudio\" >> \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"Terminal=false\" >> \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"Type=Application\" >> \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "echo \"Categories=Qt;Development;IDE;KDE;\" >> \$SHORTCUT" >> "$PWD/tmppkgdir/postinstall.sh"
echo "exit 0" >> "$PWD/tmppkgdir/postinstall.sh"
chmod 777 "$PWD/tmppkgdir/postinstall.sh"

MULESTUDIO_PKGSTRING="mulestudio-$TOOLVERSION-$LAUNCHSTRING-$MULE_MICROCONTROLLER-$CURRENTOS-$CURRENTARCH.tgz"
echo "mulestudio:optional=$MULESTUDIO_PKGSTRING" >> "$REPOINDEX"
if sh -c "cd \"$PWD/tmppkgdir\" && tar -czf \"$REPOBD/$MULESTUDIO_PKGSTRING\" ./*"; then
	echo "packaged $MULESTUDIO_PKGSTRING"
else
	echo "packaging mulestudio failed"
	exit 16
fi
rm -r -f "$PWD/tmppkgdir"

mkdir -p "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER"
echo '#!/bin/sh' > "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "DIR=\"$PREFIX/$TOOLVERSION/$MULE_MICROCONTROLLER\"" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "rm -r -f /usr/share/applications/ru.timkoisoft.mulestudio.desktop /usr/local/share/applications/kde4/ru.timkoisoft.mulestudio.desktop" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "if rm -r -f \"\$DIR\"; then" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "   echo \"uninstall complete\"" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "else" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "   echo \"uninstall failed\"" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "   exit 1" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "fi" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
echo "exit 0" >> "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"
chmod 777 "$PWD/tmppkgdir/$TOOLVERSION/$MULE_MICROCONTROLLER/uninstall.sh"

UNINSTALLER_PKGSTRING="uninstallscripts-$TOOLVERSION-$LAUNCHSTRING-$MULE_MICROCONTROLLER-$CURRENTOS-$CURRENTARCH.tgz"
echo "uninstallscripts:required=$UNINSTALLER_PKGSTRING" >> "$REPOINDEX"
if sh -c "cd \"$PWD/tmppkgdir\" && tar -czf \"$REPOBD/$UNINSTALLER_PKGSTRING\" ./*"; then
	echo "packaged $UNINSTALLER_PKGSTRING"
else
	echo "packaging uninstaller scripts failed"
	exit 20
fi
rm -r -f "$PWD/tmppkgdir"

mkdir "$PWD/makeselfsrc"
if $USEDDOWNLOADER "$DL_MAKESELF" | $OPT_MAKESELFUNPACK | tar --strip-components=1 -C "$PWD/makeselfsrc" -xf -; then
	echo "Downloaded MakeSelf"
else
	echo "Failed to download MakeSelf"
	exit 21
fi

safecopy "$PWD/mulesrc/extras/mulesetup.sh" ./setup.sh
mkdir "$PWD/licenseinfo"
safecopy "$PWD/mulesrc/LICENSE" "$PWD/licenseinfo/lgpl.txt"
for CleanDir in mulestudiosrc mulesrc qtsrc xmessage-src linarogccunpacked qtsdk; do
	rm -r -f "$PWD/$CleanDir"
	echo "removed directory \"$PWD/$CleanDir\""
done
if ./makeselfsrc/makeself.sh "$PWD" libmulesdk.run "libMule SDK $TOOLVERSION for $CURRENTARCH-$CURRENTOS $MULE_MICROCONTROLLER"  ./setup.sh; then
	echo "Your installer was built successfully"
else
	echo "Failed to build the installer, you should start all over"
	rm -r -f "$PWD/makeselfsrc"
	exit 99
fi
rm -r -f "$PWD/makeselfsrc"
exit 0
