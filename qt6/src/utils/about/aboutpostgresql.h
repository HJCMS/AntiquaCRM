// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ABOUTPOSTGRESQL_H
#define ANTIQUACRM_UTILS_ABOUTPOSTGRESQL_H

#include "infowidget.h"
#include <QWidget>

class AboutPostgreSQL : public InfoWidget {
  Q_OBJECT

public:
  explicit AboutPostgreSQL(QWidget *parent = nullptr);
  void setInfoWidget() override;
};

#endif // ANTIQUACRM_UTILS_ABOUTPOSTGRESQL_H
