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

#include "mulecurrentplatform.h"

MuleCurrentPlatform::MuleCurrentPlatform()
{
    muledebug("Current platform: " + MULE_INTERNAL_CURRENTPLATFORMNAME);
    platformClass = new MULE_INTERNAL_CURRENTPLATFORMCLASS();
    if (platformClass->initialize() == false)
        muleexception(3, "MuleCurrentPlatform can't get the specified platform (" + MULE_INTERNAL_CURRENTPLATFORMNAME + ") to initialize.", false);
}
