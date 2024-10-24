// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CHANGECUSTOMERFIND_H
#define ANTIQUACRM_PLUGIN_CHANGECUSTOMERFIND_H

#include <AntiquaWidgets>
#include <QComboBox>
#include <QLineEdit>
#include <QModelIndex>
#include <QPaintEvent>
#include <QTableWidget>

class ChangeCustomerTable final : public QTableWidget {
  Q_OBJECT

private:
  virtual void paintEvent(QPaintEvent*) override;

public:
  explicit ChangeCustomerTable(QWidget* parent = nullptr);
};

class ChangeCustomerFind final : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore* m_sql = nullptr;
  AntiquaCRM::AComboBox* m_comboBox;
  AntiquaCRM::TextLine* m_searchLine;
  ChangeCustomerTable* m_tableWidget;
  QLabel* labelCell(const QString& txt);

private Q_SLOTS:
  void customerSelected(const QModelIndex&);
  void searchClicked();

Q_SIGNALS:
  void sendCustomerSelected(qint64);
  void sendNotification(const QString&);

public:
  explicit ChangeCustomerFind(QWidget* parent = nullptr);
  ~ChangeCustomerFind();
};

#endif // ANTIQUACRM_PLUGIN_CHANGECUSTOMERFIND_H
