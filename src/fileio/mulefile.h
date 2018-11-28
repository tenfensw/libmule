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

#ifndef MULEFILE_H
#define MULEFILE_H

#include "core/muleglobalfunctions.h"
#include "core/muleconfig.h"
#include "core/muleapplication.h"
#include "platformsupport/common/mulecurrentplatform.h"

class MuleFile
{
public:
    MuleFile(MULE_OTHER_STRINGTYPE filename);
    ~MuleFile();
    MULE_OTHER_STRINGTYPE getParentDirectory();
    MULE_OTHER_STRINGTYPE getFileName();
    bool write(MULE_OTHER_STRINGTYPE val);
    bool append(MULE_OTHER_STRINGTYPE val);
    MULE_OTHER_STRINGTYPE read();
    bool remove();
    bool create();
    bool exists();

private:
    MULE_OTHER_STRINGTYPE fileName;
    MuleCurrentPlatform* mcpLocalClass;
    MULE_OTHER_STRINGTYPE readContents;

};

#endif // MULEFILE_H
