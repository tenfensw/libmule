#ifndef MULEFILE_H
#define MULEFILE_H

#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"
#include "core/muleapplication.h"
#include "platformsupport/common/mulecurrentplatform.h"

class MuleFile
{
public:
    MuleFile(const MULE_OTHER_STRINGTYPE& filename);
    ~MuleFile();
    MULE_OTHER_STRINGTYPE getParentDirectory();
    bool write(const MULE_OTHER_STRINGTYPE& val);
    bool append(const MULE_OTHER_STRINGTYPE& val);
    MULE_OTHER_STRINGTYPE read();
    bool delete();
    bool create();
    bool exists();

private:
    MULE_OTHER_STRINGTYPE fileName;
    MuleCurrentPlatform* mcpLocalClass;
    MULE_OTHER_STRINGTYPE readContents;

};

#endif // MULEFILE_H
