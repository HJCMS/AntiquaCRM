// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_SELLERS_REPAIR_FINDER_H
#define ANTIQUACRM_SELLERS_REPAIR_FINDER_H

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

class ImportRepairFinder final : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::TextLine* m_firstname;
  AntiquaCRM::TextLine* m_lastname;
  QTableWidget* m_table;
  QLabel* info(const QString& title);

private Q_SLOTS:
  void itemClicked(int row, int column);
  void prepareSearch();

Q_SIGNALS:
  void sendUseClause(qint64);
  void sendFindClause(const QString&);

public Q_SLOTS:
  void clear();

public:
  explicit ImportRepairFinder(QWidget* parent = nullptr);
  void addCustomer(const QJsonObject& customer);
};

#endif // ANTIQUACRM_SELLERS_REPAIR_FINDER_H
