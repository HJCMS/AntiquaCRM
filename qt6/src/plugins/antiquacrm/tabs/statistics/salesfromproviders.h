// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H
#define ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H

#include <AntiquaCRM>
#include <AntiquaStatistics>
#include <QChart>
#include <QFont>
#include <QObject>
#include <QSplitter>

class SoldCountChart;
class SalesVolumeChart;

class ANTIQUACRM_LIBRARY SalesFromProviders final : public QSplitter {
  Q_OBJECT

private:
  SoldCountChart *m_soldCountChart;
  SalesVolumeChart *m_salesVolumeChart;

public:
  explicit SalesFromProviders(QWidget *parent = nullptr);
  bool initView(int year);
};

#endif // ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H
