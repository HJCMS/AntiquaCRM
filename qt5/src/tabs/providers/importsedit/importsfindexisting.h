// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PROVIDER_IMPORTS_FIND_EXISTING_H
#define PROVIDER_IMPORTS_FIND_EXISTING_H

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

class ImportsFindExisting final : public QWidget {
  Q_OBJECT

private:
  LineEdit *m_firstname;
  LineEdit *m_lastname;
  QTableWidget *m_table;
  QLabel *info(const QString &title);

private Q_SLOTS:
  void itemClicked(int row, int column);
  void prepareSearch();

Q_SIGNALS:
  void sendUseClause(const QString &);
  void sendFindClause(const QString &);

public Q_SLOTS:
  void clear();

public:
  explicit ImportsFindExisting(QWidget *parent = nullptr);
  void addCustomer(const QJsonObject &customer);
};

#endif // PROVIDER_IMPORTS_FIND_EXISTING_H
