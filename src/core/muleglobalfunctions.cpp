#include "core/muleglobalfunctions.h"

void mulestrmodreplace(MULE_OTHER_STRINGTYPE& subject, const MULE_OTHER_STRINGTYPE& search,
                          const MULE_OTHER_STRINGTYPE& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != MULE_OTHER_STRINGTYPE::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}
MULE_OTHER_STRINGTYPE muleyesno(const bool& in) {
    if (in == true)
        return "yes";
    else
        return "no";
}

void muleprintf(MULE_OTHER_STRINGTYPE in) {
#ifdef MULE_FEATURES_SUPPORTSCOUT
    std::cout << in << std::endl;
#endif
#ifdef MULE_PLATFORM_LEGOEV3
    LcdPrintf('0', in.c_str());
    Wait(1000);
#endif
    return;
}

MULE_OTHER_STRINGTYPE muleinttostr(int in) {
    std::ostringstream strstream;
    strstream << in;
    return strstream.str();
}

void muledebug(MULE_OTHER_STRINGTYPE debugout) {
#ifdef MULE_FEATURES_DEBUG
    muleprintf("[DEBUG] " + debugout);
#endif
    return;
}

MULE_OTHER_STRINGTYPE mulegetcwd() {
    char wtw[1600];
    getcwd(wtw, 1600);
    return MULE_OTHER_STRINGTYPE(wtw);
}


void muleexception(const int& errcode, const MULE_OTHER_STRINGTYPE& errtext, const bool& cancatch) {
    muleprintf("libMule Application Exception");
    muleprintf("An error occured while running the code of current application.");
    muleprintf("");
    muleprintf("Error code: " + muleinttostr(errcode));
    muleprintf("Message: " + errtext);
    muleprintf("Critical: " + muleyesno(cancatch));
    muleprintf("");
    if (cancatch == true) {
        muleprintf("This program will continue to run if the algorithm catches this exception.");
        std::exception();
    }
    else {
        muleprintf("This program will now close because this exception is critical.");
        exit(errcode + 500);
    }
    return;
}


void mulesleep(const double& seconds) {
#ifdef MULE_PLATFORM_LEGOEV3
	Wait((int)(seconds * 1000));
#elif defined(MULE_OS_UNIX)
	std::system(MULE_OTHER_STRINGTYPE("sleep " + muleinttostr((int)(ceil(seconds)))).c_str());
#else
	muleprintf("[libMule/WARNING] mulesleep is not supported on this platform");
	return;
#endif
}
