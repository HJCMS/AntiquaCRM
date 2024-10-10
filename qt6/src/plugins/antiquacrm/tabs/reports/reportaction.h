// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_ACTION_H
#define ANTIQUACRM_REPORTS_ACTION_H

#include "reportsconfig.h"
#include <AntiquaCRM>
#include <QAction>
#include <QDate>
#include <QObject>
#include <QString>

class ANTIQUACRM_REPORTS_PLUGIN ReportAction final : public QAction {
  Q_OBJECT

private:
  const QDate p_date;

private Q_SLOTS:
  void clicked();

Q_SIGNALS:
  void signalClicked(const QDate&);

public:
  explicit ReportAction(const QDate& date, const QString& title, QObject* parent = nullptr);
};

#endif // ANTIQUACRM_REPORTS_ACTION_H
