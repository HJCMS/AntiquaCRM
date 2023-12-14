// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SELECTER_H
#define ANTIQUACRM_STATISTICS_SELECTER_H

#include <AntiquaWidgets>
#include <QToolBar>

class ANTIQUACRM_LIBRARY StatisticsSelecter : public QToolBar {
  Q_OBJECT

private:
  AntiquaCRM::ASpinBox *m_date;
  AntiquaCRM::AComboBox *m_box;

private Q_SLOTS:
  void chartSelected(int);

Q_SIGNALS:
  void sendChartSelected(qint64, const QString &);

public:
  explicit StatisticsSelecter(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_STATISTICS_SELECTER_H
