#!/bin/sh

if test "$1" = "-h" || test "$1" = "--help" || test "$1" = "-help" || test "$1" = ""; then
	echo "Usage: $0 --output=[path/to/muleconfig.h] [C++ compiler arguments]"
	exit 0
fi

OUTREDIR=muleconfig.h
OUTCONTENTS="// muleconfig.h - file generated with \"generate-muleconfig.sh\" on `date +"%d.%m.%Y"`"
OUTCONTENTS="$OUTCONTENTS\n// Part of libMule\n// Copyright (c) Tim K (timprogrammer@rambler.ru) 2018. Licensed under GNU GPLv3.\n#ifndef MULECONFIG_H\n#define MULECONFIG_H"
for Argument in $*; do
	if test `echo "$Argument" | cut -d '=' -f1` = "--output"; then
		OUTREDIR=`echo "$Argument" | cut -d '=' -f2`
	elif test `echo "$Argument" | sed 's/\(^..\).*/\1/'` = "-D"; then
		DEFINEWITHOUTHEADER=`echo "$Argument" | cut -c 3-`
		DEFINEOUT=""
		if echo "$DEFINEWITHOUTHEADER" | grep -q "="; then
			DEFINENAME=`echo "$DEFINEWITHOUTHEADER" | cut -d '=' -f1`
			DEFINEVAL=`echo "$DEFINEWITHOUTHEADER" | cut -d '=' -f2`
			DEFINEOUT="# ifndef $DEFINENAME\n# define $DEFINENAME $DEFINEVAL\n# endif"
		else
			DEFINENAME="$DEFINEWITHOUTHEADER"
			DEFINEOUT="# ifndef $DEFINENAME\n# define $DEFINENAME\n# endif"
		fi
		OUTCONTENTS="$OUTCONTENTS\n$DEFINEOUT"
	else
		echo "ignored $Argument"
	fi
done
OUTCONTENTS="$OUTCONTENTS\n#endif"
echo -e "$OUTCONTENTS" > "$OUTREDIR"
CPPVERSION=`echo "$OUTREDIR" | cut -d '.' -f1`
CPPVERSION="$CPPVERSION.cpp"
touch "$CPPVERSION"
exit 0
