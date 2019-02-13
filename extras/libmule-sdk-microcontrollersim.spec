%undefine _disable_source_fetch

Summary:	A library for creating portable applications for microcontrollers in C++ (Mule Microcontroller Simulator build).
Name:		libmule-sdk-microcontrollersim
Version:	0.4.0
Release:	rc1
Source0:	https://testmakerplusofficial.000webhostapp.com/opensource/libmule/v0.4/libmule-0.4.0rc1.tar.xz
License:	LGPLv2.1
Group:		Development
Url:		http://gitlab.com/timkoi/libmule
BuildRequires:	pkgconfig(x11)
BuildRequires:	pkgconfig(gtk+-3.0)
BuildRequires:	gcc
AutoReqProv:	no
Requires:	glibc
Requires:	gcc
Requires:	gcc-c++
Requires:	gtk3
Provides:	libmule-sdk-any

%description
A library for creating portable applications for microcontrollers in C++ (Mule Microcontroller Simulator build).

%files
/opt/libmule-sdk/0.4.0/microcontrollersim
/usr/share/applications/libmule-scite.desktop
/usr/share/pixmaps/libmule-scite.png
/usr/bin/libmule-scite

%prep
rm -r -f *
tar -xvJf ../SOURCES/libmule-0.4.0rc1.tar.xz
cd libmule-0.4.0rc1/
mkdir rpmbuilddir
cd rpmbuilddir
CC=gcc CXX=g++ LD=g++ AR=gcc-ar ../configure -prefix=/opt/libmule-sdk/0.4.0/microcontrollersim -scite=yes -platform=ms -nocrosscompile=yes

%build
cd libmule-0.4.0rc1/rpmbuilddir/
make -j3

%install
cd libmule-0.4.0rc1/rpmbuilddir/
RPMBUILDROOT="$RPM_BUILD_ROOT" DESTDIR="$RPM_BUILD_ROOT" make install
mkdir -v -p "$RPM_BUILD_ROOT/usr/share/pixmaps"
mkdir -v "$RPM_BUILD_ROOT/usr/share/applications"
mkdir -v "$RPM_BUILD_ROOT/usr/bin"
cp "$RPM_BUILD_ROOT/opt/libmule-sdk/0.4.0/microcontrollersim/share/pixmaps/Sci48M.png" "$RPM_BUILD_ROOT/usr/share/pixmaps/libmule-scite.png"
cp "$RPM_BUILD_ROOT/opt/libmule-sdk/0.4.0/microcontrollersim/share/applications/SciTE.desktop" "$RPM_BUILD_ROOT/usr/share/applications/libmule-scite.desktop"
cat "$RPM_BUILD_ROOT/usr/share/applications/libmule-scite.desktop" | sed 's/Exec=SciTE/Exec=libmule-scite/g' | sed 's/Icon=Sci48M/Icon=libmule-scite/g' > "$RPM_BUILD_ROOT/usr/share/applications/libmule-scite.desktop.new"
rm -r -f "$RPM_BUILD_ROOT/usr/share/applications/libmule-scite.desktop"
mv -v "$RPM_BUILD_ROOT/usr/share/applications/libmule-scite.desktop.new" "$RPM_BUILD_ROOT/usr/share/applications/libmule-scite.desktop"
ln -v -s "/opt/libmule-sdk/0.4.0/microcontrollersim/bin/SciTE" "$RPM_BUILD_ROOT/usr/bin/libmule-scite"
unlink "$RPM_BUILD_ROOT/opt/libmule-sdk/0.4.0/microcontrollersim/muletool"
ln -v -s "/opt/libmule-sdk/0.4.0/microcontrollersim/bin/muletool" "$RPM_BUILD_ROOT/opt/libmule-sdk/0.4.0/microcontrollersim/muletool"
