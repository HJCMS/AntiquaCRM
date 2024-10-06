// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_SELLERS_REPAIR_IMPORT
#define ANTIQUACRM_SELLERS_REPAIR_IMPORT

#include <AntiquaWidgets>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QJsonObject>
#include <QKeyEvent>
#include <QModelIndex>
#include <QObject>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>

class ImportRepairFinder;
class ImportRepairEdit;

class SellerImportRepair final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  const QString p_provider;
  const QString p_orderid;
  QJsonObject p_json;
  AntiquaCRM::ASqlCore* m_sql = nullptr;
  QStackedWidget* m_mainWidget;
  ImportRepairFinder* m_finder;
  ImportRepairEdit* m_cedit;
  QPushButton* btn_clear;
  AntiquaCRM::ANavigationBar* m_navbar;
  bool init();

private Q_SLOTS:
  void setStartPage();
  void setEditPage();
  void updateData();
  void findSystemCustomer(const QString&);
  void openSystemCustomer(qint64);

public:
  explicit SellerImportRepair(const QString& provider, const QString& order,
                              QWidget* parent = nullptr);
  int exec() override;
};

#endif
