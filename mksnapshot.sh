#!/bin/bash
safeexit() {
	rm -r -f "$PWD/$SNAPSHOTNAME" "$PWD/$SNAPSHOTNAME.tar.gz" "$PWD/$SNAPSHOTNAME.tar"
	exit $1
}

safecp() {
	if cp -r "$1" "$2"; then
		return 0
	else
		echo "safecp: Failed to copy \"$1\" into \"$2\"."
		safeexit 2
		return 2
	fi
}

SNAPSHOTNAME="libmule-snapshot-`date +"%d_%m_%Y"`"
if test "$1" = "-help" || test "$1" = "-h" || test "$1" = "--help"; then
	echo "Usage: $0 [output version]"
	exit 0
elif test "$1" != ""; then
	SNAPSHOTNAME="$1"
fi


if command -v gzip > /dev/null 2>&1 && command -v tar > /dev/null 2>&1; then
	echo "All necessary commands are available."
else
	echo "Not all necessary commands are available. Install GNU tar and GNU gzip."
	exit 1
fi

if test -d "$PWD/$SNAPSHOTNAME"; then
	rm -r -f "$PWD/$SNAPSHOTNAME"
fi

if test -f "$PWD/$SNAPSHOTNAME.tar.gz"; then
	rm -r -f "$PWD/$SNAPSHOTNAME.tar.gz"
fi

mkdir "$PWD/$SNAPSHOTNAME"
for DirectoryToCopy in extras src LICENSE README configure; do
	safecp "$DirectoryToCopy" "$PWD/$SNAPSHOTNAME/$DirectoryToCopy"
done
if sh -c "cd \"$PWD/$SNAPSHOTNAME/src/3rdparty\" && ./download-3rdparty-components.sh clean && ./download-3rdparty-components.sh all github-nogitclone"; then
	echo "download-3rdparty-components.sh worked just fine"
else
	echo "download-3rdparty-components.sh just crashed"
	safeexit 3
fi

for DirectoryToRemove in `find "$PWD/$SNAPSHOTNAME" -type d -name ".git"` `find "$PWD/$SNAPSHOTNAME" -type f -name ".stamp*"`; do
	rm -r -f "$DirectoryToRemove"
done

echo '#!/bin/sh' > "$PWD/$SNAPSHOTNAME/src/3rdparty/download-3rdparty-components.sh"
echo 'exit 0' >> "$PWD/$SNAPSHOTNAME/src/3rdparty/download-3rdparty-components.sh"
if tar -cf "$PWD/$SNAPSHOTNAME.tar" ./$SNAPSHOTNAME && gzip -9 "$PWD/$SNAPSHOTNAME.tar"; then
	echo "Created \"$PWD/$SNAPSHOTNAME.tar.gz\""
else
	echo "Compression failed"
	safeexit 4
fi
rm -r -f "$PWD/$SNAPSHOTNAME"
exit 0
