#!/bin/bash
if test -e "Makefile"; then
	make distclean
fi
rm -r -f .qmake.stash Makefile* debug/ release/ moc* *.exe mulestudio *.o object_script*
