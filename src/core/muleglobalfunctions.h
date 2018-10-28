#ifndef MULEGLOBALFUNCTIONS_H
#define MULEGLOBALFUNCTIONS_H

#include <iostream>
#include <sstream>
#include <exception>
#include <unistd.h>

static MULE_OTHER_STRINGTYPE muleyesno(const bool& in) {
    if (in == true)
        return "yes";
    else
        return "no";
}

static void muleprintf(MULE_OTHER_STRINGTYPE in) {
#ifdef MULE_FEATURES_SUPPORTSCOUT
    std::cout << in << std::endl;
#endif
    return;
}

static MULE_OTHER_STRINGTYPE muleinttostr(int in) {
    std::ostringstream strstream;
    strstream << in;
    return strstream.str();
}

static void muledebug(MULE_OTHER_STRINGTYPE debugout) {
#ifdef MULE_FEATURES_DEBUG
    muleprintf("[DEBUG] " + debugout);
#endif
    return;
}

static MULE_OTHER_STRINGTYPE mulegetcwd() {
    char wtw[1600];
    getcwd(wtw, 1600);
    return MULE_OTHER_STRINGTYPE(wtw);
}


static void muleexception(const int& errcode, const MULE_OTHER_STRINGTYPE& errtext, const bool& cancatch) {
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

namespace MuleApplicationWideData {
    void* appWideFirstInstance;
}

#endif // MULEGLOBALFUNCTIONS_H
