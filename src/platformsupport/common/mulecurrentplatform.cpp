#include "mulecurrentplatform.h"

MuleCurrentPlatform::MuleCurrentPlatform()
{
    muledebug("Current platform: " + MULE_INTERNAL_CURRENTPLATFORMNAME);
    platformClass = new MULE_INTERNAL_CURRENTPLATFORMCLASS();
    if (platformClass->initialize() == false)
        muleexception(3, "MuleCurrentPlatform can't get the specified platform (" + MULE_INTERNAL_CURRENTPLATFORMNAME + ") to initialize.", false);
}
