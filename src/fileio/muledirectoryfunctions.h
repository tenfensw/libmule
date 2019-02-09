//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef MULEDIRECTORYFUNCTIONS_H
#define MULEDIRECTORYFUNCTIONS_H
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include "fileio/mulefile.h"

std::vector<MuleFile> mulels(MULE_OTHER_STRINGTYPE indir) {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (indir.c_str())) != NULL) {
		std::vector<MuleFile> result;
		while ((ent = readdir (dir)) != NULL)
			result.push_back(MuleFile(MULE_OTHER_STRINGTYPE(ent->d_name)));
		closedir (dir);
		return result;
	}
	return std::vector<MuleFile>();
}

bool mulemkdir(MULE_OTHER_STRINGTYPE indir) {
	MULE_OTHER_STRINGTYPE cmd;
#ifdef MULE_OS_UNIX
	cmd = MULE_OTHER_STRINGTYPE("mkdir -p " + indir);
#else
	cmd = MULE_OTHER_STRINGTYPE("mkdir " + indir);
#endif
	if (std::system(cmd.c_str()) == 0)
		return true;
	else
		return false;
}

bool mulermdir(MULE_OTHER_STRINGTYPE indir) {
	MULE_OTHER_STRINGTYPE cmd;
#ifdef MULE_OS_UNIX
	cmd = MULE_OTHER_STRINGTYPE("rm -r -f " + indir);
#else
	cmd = "exit 1";
#endif
	if (std::system(cmd.c_str()) == 0)
		return true;
	else
		return false;
}
#endif
