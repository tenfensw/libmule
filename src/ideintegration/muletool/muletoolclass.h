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

#ifndef MULETOOLCLASS_H
#define MULETOOLCLASS_H
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#ifdef _WIN32
#include <windows.h> // for _getcwd
#else
#include <unistd.h>
#endif

#define FILENOTFOUND std::string("nf")
#define TOOLBUGURL std::string("timprogrammer@rambler.ru")
#define TOOLVERSION std::string("0.4.0-rc")
#ifdef MULETOOL_WORKAROUND_PWDASPREFIX
#define TOOLTARGET std::string("any")
#define TOOLPREFIX std::string(".")
#endif

class MuleToolClass {
	public:
	  MuleToolClass(int argc, char** argv);
	  ~MuleToolClass();
	  int run();
	  std::string getVariableValue(const std::string& vname);
	
	private:
	  bool init(int argc, char** argv);
	  std::vector<std::string> split(const std::string& origstr, const char* delim);
	  std::vector<std::string> getDirectoryContents(const std::string& dname);
	  bool loadConfig(const std::string& cfgname);
	  bool stringVectorContains(const std::vector<std::string>& invec, const std::string& initem);
	  void criticalError(const int& ercode, const std::string& ermessage);
	  void verboseInfo(const std::string& vinfo);
	  bool fileExists(const std::string& fname);
	  bool directoryExists(const std::string& dname);
	  void parseArguments();
	  void viewHeaderMessage();
	  void viewHelp();
	  void printInternalValues();
	  bool detectAndLoadConfig();
	  int compileFiles();
	  int linkProgram();
	  int deployProgram();
	  std::vector<std::string> fillPossibleLocationsVector();
	  std::string readFromFileToString(const std::string& fname);
	  std::string getCurrentDirectory();
	  std::string replaceContextAlternatives(const std::string& vname, const std::string& vval);
	  std::string replaceSubstring(const std::string& origstring, const std::string& tofind, const std::string& toreplace);
	  int stringLength(const std::string& origstring);
	  char lastStringChar(const std::string& stritself);
	  
	  // private class-wide variables
	  bool successfullyInitialized;
	  bool beQuiet;
	  bool haveToDetectConfig;
	  bool needToDumpConfigFileName;
	  bool doNotIncludeLibMule;
	  bool useShortObjectFileNames;
	  bool printCommandsOnScreen;
	  std::vector<std::string> cliArgs;
	  std::string actionToRun;
	  std::vector<std::string> actionArgs;
	  std::string currentProgName;
	  char dirsepchar;
	  std::string userHomeDir;
	  std::string configFilePath;
	  
	  // variables read from config
	  std::string compilerCC;
	  std::string compilerCXX;
	  std::string compilerFlags_CC;
	  std::string compilerFlags_CXX;
	  std::string linkerLD;
	  std::string linkerFlags_LD;
	  std::string deploy_copyFile;
	  std::string deploy_runFile;
	  std::string deploy_binName;
	  std::string libMuleLib;
	  std::string libMuleInclude;
	  std::string libMuleTarget;
	  
};
#endif
