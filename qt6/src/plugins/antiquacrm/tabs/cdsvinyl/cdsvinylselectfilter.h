// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYLSELECTFILTER_H
#define ANTIQUACRM_PLUGIN_CDSVINYLSELECTFILTER_H

#include "cdsvinylconfig.h"
#include <AGlobal>
#include <QComboBox>
#include <QWidget>

class ANTIQUACRM_CDSVINYL_PLUGIN CDsVinylSelectFilter final : public QComboBox {
  Q_OBJECT

public:
  explicit CDsVinylSelectFilter(QWidget* parent = nullptr);
  const QString currentFilter(int index = -1);
  const QString currentToolTip(int index = -1);
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLSELECTFILTER_H
