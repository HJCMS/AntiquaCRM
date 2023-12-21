// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_TABLEVIEW_H
#define ANTIQUACRM_REPORTS_TABLEVIEW_H

#include <AntiquaWidgets>
#include <QObject>
#include <QTableView>
#include <QWidget>

class ReportsTableModel;

class ANTIQUACRM_LIBRARY ReportsTableView final : public QTableView {
  Q_OBJECT

private:
  int calc_section = -1;
  int refunds_section = -1;
  ReportsTableModel *m_model;
  QHeaderView *m_tableHeader;

Q_SIGNALS:
  void sendFinished();

public Q_SLOTS:
  void setQuery(const QString &query);

public:
  explicit ReportsTableView(QWidget *parent = nullptr);
  const QString headerName(const QString &key);
  const QString dataHeader(const QChar &delimiter = QChar::Tabulation);
  const QStringList dataRows(const QChar &delimiter = QChar::Tabulation);
  const QString salesVolume();
};

#endif // ANTIQUACRM_REPORTS_TABLEVIEW_H
