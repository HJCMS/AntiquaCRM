/** @COPYRIGHT_HOLDER@ */

#include "../version.h"
#include "applsettings.h"

ApplSettings::ApplSettings()
    : QSettings(HJCMSFQDN,ANTIQUACRM_NAME)
{

}
