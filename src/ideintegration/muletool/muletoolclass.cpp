#include "muletoolclass.h"

MuleToolClass::MuleToolClass(int argc, char** argv) {
#ifdef _WIN32
	dirsepchar = '\\';
	try {
		userHomeDir = std::string(std::getenv("USERPROFILE"));
	}
	catch (...) {
		userHomeDir = "C:\\";
	}
#else
	dirsepchar = '/';
	try {
		userHomeDir = "/home/" + std::string(std::getenv("USER"));
#  if defined(__APPLE__) && defined(__MACH__)
		userHomeDir = "/Users/" + std::string(std::getenv("USER"));
#  elif defined(__BEOS__)
		userHomeDir = "/boot/home";
#  endif
	}
	catch (...) {
		userHomeDir = "/";
	}
#endif
	successfullyInitialized = init(argc, argv);
}

MuleToolClass::~MuleToolClass() {
}

bool MuleToolClass::init(int argc, char** argv) {
	// convert char** to vector
	for (int i = 1; i < argc; i++)
		cliArgs.push_back(std::string(argv[i]));
		
	currentProgName = std::string(argv[0]);
	
	if ((stringVectorContains(cliArgs, "-quiet") == false) && (stringVectorContains(cliArgs, "-q") == false)) {
		beQuiet = false;
		viewHeaderMessage();
	}
	else
		beQuiet = true;
	
	if (stringVectorContains(cliArgs, "-nolibmule") == false)
		doNotIncludeLibMule = false;
	else
		doNotIncludeLibMule = true;
		
		
	if (stringVectorContains(cliArgs, "-config") == true)
		haveToDetectConfig = false;
	else
		haveToDetectConfig = true;
	
	if ((stringVectorContains(cliArgs, "--help") == true) || (stringVectorContains(cliArgs, "-help") == true) || (stringVectorContains(cliArgs, "-h") == true)) {
		actionToRun = "printhelp";
		return true;	
	}
	else
		parseArguments();
		
	if (detectAndLoadConfig() == false)
		return false;
		
	return true;
}

bool MuleToolClass::detectAndLoadConfig() {
	if (haveToDetectConfig == true) {
		std::vector<std::string> possibleLocations = fillPossibleLocationsVector();
		configFilePath = FILENOTFOUND;
		for (int i = 0; i < possibleLocations.size(); i++) {
			if (fileExists(possibleLocations[i] + dirsepchar + "MuleTool.mcfg") == true) {
				configFilePath = possibleLocations[i] + dirsepchar + "MuleTool.mcfg";
				break;
			}
		}
		if (configFilePath == FILENOTFOUND) {
			verboseInfo("Configuration file not found, exiting with a critical error");
			return false;
		}
	}
	verboseInfo("Using configuration file located at " + configFilePath);
	return loadConfig(configFilePath);
}

bool MuleToolClass::loadConfig(const std::string& cfgname) {
	if (fileExists(cfgname) == false) {
		verboseInfo("MuleToolClass::loadConfig thinks that " + cfgname + " does not exist, exiting with a critical error");
		return false;
	}
	std::string filectnt = readFromFileToString(cfgname);
	std::vector<std::string> splitfilectnt = split(filectnt, std::string("\n").c_str());
	for (int i = 0; i < splitfilectnt.size(); i++) {
		if (splitfilectnt[i][0] != '#') {
			if (splitfilectnt[i].back() == ':')
				splitfilectnt[i] = splitfilectnt[i] + "  ";
			std::vector<std::string> splitline = split(splitfilectnt[i], std::string(":").c_str());
			std::string hone = splitline[0];
			std::string htwo = splitline[1];
			htwo = replaceContextAlternatives(hone, htwo);
			if (hone == "CC")
				compilerCC = htwo;
			else if (hone == "CXX")
				compilerCXX = htwo;
			else if (hone == "CFLAGS")
				compilerFlags_CC = htwo;
			else if (hone == "CXXFLAGS")
				compilerFlags_CXX = htwo;
			else if (hone == "LD")
				linkerLD = htwo;
			else if (hone == "LDFLAGS")
				linkerFlags_LD = htwo;
			else if (hone == "TARGET")
				libMuleTarget = htwo;
			else if (hone == "LIBMULE") {
				libMuleLib = htwo + dirsepchar + "lib" + dirsepchar + "libMule.a";
				libMuleInclude = htwo + dirsepchar + "include" + dirsepchar + "libmule";
			}
			else if (hone == "DEPLOY")
				deploy_copyFile = htwo;
			else if (hone == "RUN")
				deploy_runFile = htwo;
		}
	}
	return true;
}

std::vector<std::string> MuleToolClass::split(const std::string& origstr, const char* delim) {
	std::vector<std::string> result;
	char* strToSplit = new char[origstr.size() + 1];
	std::copy(origstr.begin(), origstr.end(), strToSplit);
	strToSplit[origstr.size()] = '\0';

	char* strtokedstring = strtok(strToSplit, delim);
	result.push_back(std::string(strtokedstring));

	while (true) {
		strtokedstring = strtok(NULL, delim);
		if (!strtokedstring)
			break;
		result.push_back(std::string(strtokedstring));
	}

	return result;
}

bool MuleToolClass::fileExists(const std::string& fname) {
	std::ifstream istream(fname.c_str());
	return istream.good();
}

std::string MuleToolClass::readFromFileToString(const std::string& fname) {
	std::ifstream t(fname);
	return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

std::vector<std::string> MuleToolClass::fillPossibleLocationsVector() {
	std::vector<std::string> possibleLocations;
	possibleLocations.push_back(TOOLPREFIX + dirsepchar + "etc");
	possibleLocations.push_back(TOOLPREFIX);
	possibleLocations.push_back(getCurrentDirectory() + dirsepchar + "etc");
	possibleLocations.push_back(getCurrentDirectory());
	possibleLocations.push_back(userHomeDir + dirsepchar + ".timkoisoft" + dirsepchar + "mule");
	possibleLocations.push_back(userHomeDir + dirsepchar + ".timkoisoft" + dirsepchar + "libmule");
	possibleLocations.push_back(userHomeDir + dirsepchar + ".config" + dirsepchar + "timkoisoft" + dirsepchar + "mule");
	possibleLocations.push_back(userHomeDir + dirsepchar + ".config" + dirsepchar + "timkoisoft" + dirsepchar + "libmule");
#ifndef _WIN32
	possibleLocations.push_back("/etc");
	possibleLocations.push_back("/etc/timkoisoft/mule");
	possibleLocations.push_back("/etc/timkoisoft/libmule");
	possibleLocations.push_back("/opt/timkoisoft/mule/etc");
	possibleLocations.push_back("/opt/timkoisoft/libmule/etc");
	possibleLocations.push_back("/usr/share/timkoisoft/libmule");
	possibleLocations.push_back("/usr/share/timkoisoft/mule");
#else
	possibleLocations.push_back(std::string(std::getenv("APPDATA")) + "\\Timkoisoft\\Mule\\etc");
	possibleLocations.push_back(std::string(std::getenv("APPDATA")) + "\\Timkoisoft\\Mule");
	possibleLocations.push_back(std::string(std::getenv("APPDATA")) + "\\Timkoisoft\\libMule\\etc");
	possibleLocations.push_back(std::string(std::getenv("APPDATA")) + "\\Timkoisoft\\libMule");
	possibleLocations.push_back(std::string(std::getenv("ALLUSERSPROFILE")) + "\\Timkoisoft\\Mule\\etc");
	possibleLocations.push_back(std::string(std::getenv("ALLUSERSPROFILE")) + "\\Timkoisoft\\Mule");
	possibleLocations.push_back(std::string(std::getenv("ALLUSERSPROFILE")) + "\\Timkoisoft\\libMule\\etc");
	possibleLocations.push_back(std::string(std::getenv("ALLUSERSPROFILE")) + "\\Timkoisoft\libMule");
	possibleLocations.push_back(std::string(std::getenv("ProgramFiles")) + "\\Timkoisoft\\Mule");
	possibleLocations.push_back(std::string(std::getenv("ProgramFiles")) + "\\Timkoisoft\\Mule");
#endif
	return possibleLocations;
}

std::string MuleToolClass::getCurrentDirectory() {
#ifndef _WIN32
	char cwd[1400];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return std::string(cwd);
#else
	TCHAR cwd[1400];
	if (GetCurrentDirectory(1400, cwd) != NULL)
		return std::string(cwd);
#endif
	return userHomeDir;
}

bool MuleToolClass::stringVectorContains(const std::vector<std::string>& invec, const std::string& initem) {
	int vecsize = invec.size();
	for (int i = 0; i < vecsize; i++) {
		if (invec[i] == initem)
			return true;
	}
	return false;
}

int MuleToolClass::run() {
	if (successfullyInitialized == false)
		criticalError(-1, "Application failed to initialize because the configuration could not be loaded properly");
	
	//verboseInfo("MuleToolClass::run -> " + actionToRun);
	if (actionToRun == "printhelp") {
		viewHelp();
		return 0;
	}
	else if (actionToRun == "compile")
		return compileFiles();
	else if (actionToRun == "link")
		return linkProgram();
	else if (actionToRun == "deploy")
		return deployProgram();
	return -1;
}

void MuleToolClass::criticalError(const int& ercode, const std::string& ermessage) {
	std::cout << "[ERROR №" << ercode << "] " << ermessage.c_str() << std::endl;
	std::exit(ercode);
}

void MuleToolClass::verboseInfo(const std::string& vinfo) {
	if (beQuiet == false)
		std::cout << "[INFO] " << vinfo << std::endl;
}

void MuleToolClass::viewHeaderMessage() {
	std::cout << "muletool " << TOOLVERSION << " - a libMule application deployment and building utility" << std::endl;
	std::cout << "Copyright (c) 2018 Tim K <timprogrammer@rambler.ru>. Licensed under GNU LGPLv2.1." << std::endl;
	std::cout << "Original target platform is " << TOOLTARGET << std::endl;
	std::cout << "" << std::endl;
}

void MuleToolClass::viewHelp() {
	std::cout << "Usage: " << currentProgName << " [some arguments here]" << std::endl;
	std::cout << "     -deploy [binary name]           Deploy file [binary name] to target" << std::endl;
	std::cout << "                                     device and run it." << std::endl;
	std::cout << "" << std::endl;
	std::cout << "     -compile [optionally filename]  If a filename is specified after   " << std::endl;
	std::cout << "                                     -compile, then it will be compiled " << std::endl;
	std::cout << "                                     into an object file."  << std::endl;
	std::cout << "" << std::endl;
	std::cout << "     -link [optionally object files] Link all object files specified on" << std::endl;
	std::cout << "                                     the command-line (if none are spe-" << std::endl;
	std::cout << "                                     cified, then all object files in  " << std::endl;
	std::cout << "                                     $PWD will be automatically selected)" << std::endl;
	std::cout << "                                     into a binary file that could be ran" << std::endl;
	std::cout << "                                     on the target device." << std::endl;
	std::cout << "" << std::endl;
	std::cout << "     -o [filename]                   A useful option that specifies out-" << std::endl;
	std::cout << "                                     put filename for -compile and -link" << std::endl;
	std::cout << "                                     options." << std::endl;
	std::cout << "" << std::endl;
	std::cout << "    -config [filename]              Read muletool configuration from " << std::endl;
	std::cout << "                                    [filename]." << std::endl;
	std::cout << "" << std::endl;
	std::cout << "     -quiet                          Be as quiet as possible." << std::endl;
	std::cout << "" << std::endl;
	std::cout << "     -help                           Print this help message and exit." << std::endl;
	std::cout << "" << std::endl;
	std::cout << "All bugs should be reported to " << TOOLBUGURL << std::endl;
	
}

std::string MuleToolClass::replaceContextAlternatives(const std::string& vname, const std::string& vval) {
	std::string calt = "";
	if (vname == "CC" || vname == "LD")
		calt = compilerCXX;
	else if (vname == "CXX")
		calt = compilerCC;
	else if (vname == "LDFLAGS" || vname == "CFLAGS")
		calt = compilerFlags_CXX;
	else if (vname == "CXXFLAGS")
		calt = compilerFlags_CC;
	else if (vname == "DEPLOY" || vname == "RUN")
		calt = deploy_binName;
	
	std::string result = "";
	for (int i = 0; i < vval.size(); i++) {
		if (vval[i] != '%')
			result = result + vval[i];
		else {
			if ((i < (vval.size() - 1)) && (vval[i + 1] == 'A')) {
				i = i + 1;
				result = result + calt;
			}
		}
	}
	return result;
}

void MuleToolClass::parseArguments() {
	actionToRun = "nmtd"; // this means "not much to do"
	actionArgs.clear();
	for (int i = 0; i < cliArgs.size(); i++) {
		if (cliArgs[i] == "-compile") {
			actionToRun = "compile";
			if ((i + 1) < (cliArgs.size() - 1)) {
				actionArgs.push_back(cliArgs[i + 1]);
				i = i + 1;
			}
		}
		else if (cliArgs[i] == "-link") {
			actionToRun = "link";
			if ((i + 1) < (cliArgs.size() - 1)) {
				actionArgs.push_back(cliArgs[i + 1]);
				i = i + 1;
			}
		}
		else if (cliArgs[i] == "-deploy") {
			actionToRun = "deploy";
			deploy_binName = cliArgs[i + 1];
			i = i + 1;
		}
		else if (cliArgs[i] == "-config") {
			if ((i + 1) < (cliArgs.size() - 1) && (fileExists(cliArgs[i + 1]))) {
				haveToDetectConfig = false;
				configFilePath = cliArgs[i + 1];
			}
			else
				haveToDetectConfig = true;
			i = i + 1;
			
		}
		else if (cliArgs[i] == "-o") {
			if ((i + 1) < (cliArgs.size() - 1)) {
				actionArgs.push_back(cliArgs[i]);
				actionArgs.push_back(cliArgs[i + 1]);
				i = i + 1;
			}
		}
		else if (cliArgs[i] == "-quiet")
			beQuiet = true;
		else {
			if ((fileExists(cliArgs[i])) || (cliArgs[i][0] == '-' && cliArgs[i][1] == 'D') || (cliArgs[i][0] == '-' && cliArgs[i][1] == 'I') || (cliArgs[i][0] == '-' && cliArgs[i][1] == 's'))
				actionArgs.push_back(cliArgs[i]);
		}
	}
	
	if (actionToRun == "nmtd")
		criticalError(6, "Nothing to do, exiting");
	
}

bool MuleToolClass::directoryExists(const std::string& dname) {
	struct stat dirinfo;
	if (stat(dname.c_str(), &dirinfo) != 0)
		return false;
	if (dirinfo.st_mode & S_IFDIR)
		return true;
	return false;
}

int MuleToolClass::compileFiles() {
	std::vector<std::string> filesToCompile;
	std::string cflags = compilerFlags_CC;
	std::string cxxflags = compilerFlags_CXX;
	for (int i = 0; i < actionArgs.size(); i++) {
		if (fileExists(actionArgs[i]))
			filesToCompile.push_back(actionArgs[i]);
		else if (directoryExists(actionArgs[i]))
			criticalError(11, "You can only compile specific files, not a whole directory.");
		else {
			cflags = cflags + " " + actionArgs[i];
			cxxflags = cxxflags + " " + actionArgs[i];
		}
	}
	if (doNotIncludeLibMule == false) {
		cflags = cflags + " -I" + libMuleInclude;
		cxxflags = cxxflags + " -I" + libMuleInclude;
	}
	for (int j = 0; j < filesToCompile.size(); j++) {
		std::string extension = split(filesToCompile[j], ".").back();
		std::string buildcmd;
		if (extension == "cxx" || extension == "cpp" || extension == "c++" || extension == "cc" || extension == "C")
			buildcmd = compilerCXX + " " + cxxflags;
		else if (extension == "c")
			buildcmd = compilerCC + " " + cflags;
		else
			buildcmd = "echo \"skip " + filesToCompile[j] + "\"";
		std::string outobjectname = filesToCompile[j];
		// create a proper object filename
		std::vector<std::string> veconame = split(outobjectname, ".");
		outobjectname = veconame[0];
		for (int i = 1; i < (veconame.size() - 1); i++)
			outobjectname = outobjectname + "." + veconame[i];
		outobjectname = outobjectname + ".o";
		buildcmd = buildcmd + " -o \"" + outobjectname + "\" \"" + filesToCompile[j] + "\"";
		if (fileExists(outobjectname)) {
#ifdef _WIN32
			std::cout << "del /q " << outobjectname << std::endl;
			std::system(std::string("del /q \"" + outobjectname + "\"").c_str());
#else
			std::cout << "rm -r -f " << outobjectname << std::endl;
			std::system(std::string("rm -r -f \"" + outobjectname + "\"").c_str());
#endif
		}
		std::cout << buildcmd << std::endl;
		if (std::system(buildcmd.c_str()) != 0)
			criticalError(10, "Failed to compile \"" + filesToCompile[j] + "\"");
	}
	std::cout << "Everything was compiled successfully" << std::endl;
	return 0;
}

int MuleToolClass::linkProgram() {
	std::vector<std::string> filesToCompile;
	std::string ldflags = linkerFlags_LD;
	bool haveOutput = false;
	for (int i = 0; i < actionArgs.size(); i++) {
		if (fileExists(actionArgs[i]))
			filesToCompile.push_back(actionArgs[i]);
		else if (directoryExists(actionArgs[i]))
			criticalError(11, "You can only compile specific files, not a whole directory.");
		else {
			if (actionArgs[i] == "-o")
				haveOutput = true;
			ldflags = ldflags + " " + actionArgs[i];
		}
	}
	if (haveOutput == false)
		ldflags = ldflags + " -o a.out";
	if (doNotIncludeLibMule == false)
		ldflags = ldflags + " " + libMuleLib;
	
	std::string buildcmd = linkerLD + " " + ldflags;
	for (int j = 0; j < filesToCompile.size(); j++)
		buildcmd = buildcmd + " " + filesToCompile[j];
	std::cout << buildcmd << std::endl;
	if (std::system(buildcmd.c_str()) != 0)
		return 12;
	std::cout << "Everything linked successfully" << std::endl;
	return 0;
}

int MuleToolClass::deployProgram() {
	std::vector<std::string> filesToCompile;
	verboseInfo("Deploying program \"" + deploy_binName + "\"");
	std::cout << deploy_copyFile << std::endl;
	if (std::system(deploy_copyFile.c_str()) != 0)
		return 14;
	std::cout << deploy_runFile << std::endl;
	if (std::system(deploy_runFile.c_str()) != 0)
		return 15;
	std::cout << "Your program was successfully deployed" << std::endl;
	return 0;
}

std::vector<std::string> MuleToolClass::getDirectoryContents(const std::string& dname) {
	if (directoryExists(dname) == false)
		return std::vector<std::string>();
	
	std::vector<std::string> result;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (dname.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL)
			result.push_back(std::string(ent->d_name));
		closedir (dir);
	}
	return std::vector<std::string>();
}

