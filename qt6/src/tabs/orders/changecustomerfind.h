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
#include <QTableWidget>

class ANTIQUACRM_LIBRARY ChangeCustomerFind final : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore* m_sql = nullptr;
  QComboBox* m_comboBox;
  QLineEdit* m_searchLine;
  QTableWidget* m_tableWidget;

  QLabel* labelCell(const QString& txt);

private Q_SLOTS:
  void searchClicked();

public:
  explicit ChangeCustomerFind(QWidget* parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_CHANGECUSTOMERFIND_H
