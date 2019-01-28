#!/bin/sh
CXX="c++ -o muletool"
SHAREDDEFINES="-DMULETOOL_WORKAROUND_PWDASPREFIX"
if command -v g++ > /dev/null 2>&1; then
	CXX="g++ -o muletool -std=gnu++0x -g -Os"
elif command -v clang++ > /dev/null 2>&1; then
	CXX="clang++ -o muletool -std=c++0x -g -Os"
elif command -v CC > /dev/null 2>&1; then
	CXX="CC -o muletool"
fi

if test -f muletool; then
	rm -r -f muletool
fi

if $CXX $SHAREDDEFINES muletoolclass.cpp main.cpp; then
	echo "Built muletool successfully"
else
	echo "Did not build muletool successfully"
	exit 1
fi
