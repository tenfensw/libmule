#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include "fileio/mulefile.h"

std::vector<MuleFile> mulels(const MULE_OTHER_STRINGTYPE& indir) {
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

bool mulemkdir(const MULE_OTHER_STRINGTYPE& indir) {
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

bool mulermdir(const MULE_OTHER_STRINGTYPE& indir) {
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
