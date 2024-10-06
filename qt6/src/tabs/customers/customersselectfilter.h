// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSSELECTFILTER_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSSELECTFILTER_H

#include <AGlobal>
#include <QComboBox>
#include <QObject>
#include <QWidget>

class CustomersSelectFilter final : public QComboBox {
  Q_OBJECT

public:
  explicit CustomersSelectFilter(QWidget* parent = nullptr);
  const QString getFilter(int index = -1);
  const QString getToolTip(int index = -1);
};

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSSELECTFILTER_H
