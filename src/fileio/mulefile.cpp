#include "fileio/mulefile.h"

MuleFile::MuleFile(const MULE_OTHER_STRINGTYPE& filename) {
	muledebug("MuleFile(" + filename + ");");
	fileName = filename;
	mulestrmodreplace(fileName, "\\", "/");
	mulestrmodreplace(fileName, "./", mulegetcwd());
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

bool MuleFile::write(const MULE_OTHER_STRINGTYPE& val) {
	if (mcpLocalClass->writeToFile(fileName, val))
		return true;
	else
		return false;
}

bool MuleFile::append(const MULE_OTHER_STRINGTYPE& val) {
	if (write(readContents + "\n" + val)) {
		readContents = readContents + "\n" + val;
		return true;
	}
	else
		return false;
}

MULE_OTHER_STRINGTYPE MuleFile::getParentDirectory() {
	muleexception(2, "Unimplemented", true);
	return "";
}

bool MuleFile::remove() {
	return mcpLocalClass->deleteFile(fileName);
}

bool MuleFile::create() {
	return write("");
}
