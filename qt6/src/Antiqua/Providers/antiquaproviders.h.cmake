 // -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PROVIDERS_PLUGIN_H
#define ANTIQUACRM_PROVIDERS_PLUGIN_H

#ifndef ANTIQUACRM_PROVIDER_INTERFACE
#define ANTIQUACRM_PROVIDER_INTERFACE "@CMAKE_ANTIQUA_PROVIDER_INTERFACE@"
#endif

// AntiquaCRM::AGlobal
#ifndef ANTIQUACRM_GLOBAL_H
#include "aglobal.h"
#endif

// AntiquaCRM::PluginConfigWidget
#ifndef ANTIQUACRM_PLUGIN_CONFIGWIDGET_H
// Note: This class is a part of AntiquaWidgets library.
#include "apluginconfigwidget.h"
#endif

// AntiquaCRM::ProviderInterface
#include "profiderinterface.h"

// AntiquaCRM::ProvidersLoader
#include "providersloader.h"

// AntiquaCRM::ProviderActionDialog
#include "provideractiondialog.h"

#endif // ANTIQUACRM_PROVIDERS_PLUGIN_H
