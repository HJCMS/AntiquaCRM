// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSSELECTFILTER_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSSELECTFILTER_H

#include <AGlobal>
#include <QComboBox>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY CustomersSelectFilter final : public QComboBox {
  Q_OBJECT

public:
  explicit CustomersSelectFilter(QWidget *parent = nullptr);
  const QString currentFilter(int index = -1);
  const QString currentToolTip(int index = -1);
};

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSSELECTFILTER_H
