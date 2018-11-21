#ifndef MULETOOLCLASS_H
#define MULETOOLCLASS_H
#include <iostream>
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
#define TOOLVERSION std::string("14112018-working")
#define TOOLTARGET std::string("any")
#ifdef MULETOOL_WORKAROUND_PWDASPREFIX
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
	  char lastStringChar(const std::string& stritself);
	  
	  // private class-wide variables
	  bool successfullyInitialized;
	  bool beQuiet;
	  bool haveToDetectConfig;
	  bool needToDumpConfigFileName;
	  bool doNotIncludeLibMule;
	  bool useShortObjectFileNames;
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
