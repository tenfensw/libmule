#!/bin/bash
if test "$1" = "-help" || test "$1" = "--help" || test "$1" = "-h" || test "$1" = "-?" || test "$1" = "/?"; then
	echo "Just run this script to patch configure so that it would support MinGW32."
	exit 0
fi

if command -v patch > /dev/null 2>&1; then
	echo "patch command is available on your system, can continue"
else
	echo "Please install GNU patch first."
	exit 1
fi

BASECONFDIR=`dirname "$0"`
BASECONFDIR=`sh -c "cd \"$BASECONFDIR\" && pwd"`
echo "configure should be located in $BASECONFDIR"
cp "$BASECONFDIR/configure" "$BASECONFDIR/configure.beforepatch.backup"
for PatchFile in $BASECONFDIR/extras/mingwcrosscompile/*; do
	if sh -c "cd \"$BASECONFDIR\" && patch -p0 < \"$PatchFile\""; then
		echo "Applied patch $PatchFile"
	else
		echo "Patch $PatchFile was not applied properly, cannot continue"
		exit 2
	fi
done

echo "Successfully patched configure."
exit 0

