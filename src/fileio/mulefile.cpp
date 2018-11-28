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

#include "fileio/mulefile.h"

MuleFile::MuleFile(MULE_OTHER_STRINGTYPE filename) {
	muledebug("MuleFile(" + filename + ");");
	fileName = filename;
	mulestrmodreplace(fileName, "\\", "/");
	mulestrmodreplace(fileName, "./", MULE_OTHER_STRINGTYPE(mulegetcwd() + "/"));
	muledebug("Filename after replacements is " + fileName);
	mcpLocalClass = MuleApplication::getRunningInstance()->getPlatformClass();
	if (mcpLocalClass->fileExists("./" + fileName))
		fileName = mulegetcwd() + fileName;

	if (exists())
		readContents = mcpLocalClass->readFromFile(fileName);
}

MuleFile::~MuleFile() {
	muledebug("~MuleFile() called, goodbye");
}

MULE_OTHER_STRINGTYPE MuleFile::read() {
	return readContents;
}

bool MuleFile::exists() {
	return mcpLocalClass->fileExists(fileName);
}

bool MuleFile::write(MULE_OTHER_STRINGTYPE val) {
	muledebug("Called MuleFile::write(" + val + ")");
	if (mcpLocalClass->writeToFile(fileName, val)) {
		readContents = val;
		return true;
	}
	else
		return false;
}

bool MuleFile::append(MULE_OTHER_STRINGTYPE val) {
	if (write(readContents + "\n" + val)) {
		readContents = readContents + "\n" + val;
		return true;
	}
	else
		return false;
}

MULE_OTHER_STRINGTYPE MuleFile::getParentDirectory() {
	return fileName.substr(0, fileName.find_last_of("/"));
}

bool MuleFile::remove() {
	return mcpLocalClass->deleteFile(fileName);
}

bool MuleFile::create() {
	return write("");
}

MULE_OTHER_STRINGTYPE MuleFile::getFileName() {
	return fileName;
}
