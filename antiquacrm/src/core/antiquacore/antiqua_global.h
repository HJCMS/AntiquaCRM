// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#ifndef ANTIQUA_GLOBAL_H
#define ANTIQUA_GLOBAL_H

#ifndef ANTIQUACRM_NAME
#define ANTIQUACRM_NAME "antiquacrm"
#endif

#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "Antiqua CRM"
#endif

#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "0.0.2"
#endif

#ifndef ANTIQUACRM_VERSION_STRING
#define ANTIQUACRM_VERSION_STRING "0.0.2"
#endif

#ifndef HJCMS_CONFIG_DOMAIN
#define HJCMS_CONFIG_DOMAIN "de.hjcms"
#endif

#ifndef ANTIQUACRM_CONNECTION_PREFIX
#define ANTIQUACRM_CONNECTION_PREFIX "de.hjcms."
#endif

#ifndef ANTIQUACRM_HOMEPAGE
#define ANTIQUACRM_HOMEPAGE "https://antiquacrm.hjcms.de"
#endif

#include <QtCore/QtGlobal>
#if defined(ANTIQUACORE_LIBRARAY)
# define ANTIQUACORE_EXPORT Q_DECL_EXPORT
#else
# define ANTIQUACORE_EXPORT Q_DECL_IMPORT
#endif

#endif // ANTIQUA_GLOBAL_H
