// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_SELLERS_REPAIR_IMPORT
#define ANTIQUACRM_SELLERS_REPAIR_IMPORT

#include <AntiquaCRM>
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
class ImportRepairNavBar;

class SellerImportRepair final : public QDialog {
  Q_OBJECT

private:
  const QString p_provider;
  const QString p_orderid;
  QJsonObject p_json;
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  QStackedWidget *m_mainWidget;
  ImportRepairFinder *m_finder;
  ImportRepairEdit *m_cedit;
  ImportRepairNavBar *m_navbar;
  QDialogButtonBox *m_buttonsBar;
  QStatusBar *m_statusBar;

  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

private Q_SLOTS:
  void setStartPage();
  void setEditPage();
  void updateData();
  void findSystemCustomer(const QString &);

public:
  explicit SellerImportRepair(const QString &provider, const QString &order,
                              QWidget *parent = nullptr);
  int exec() override;
};

#endif
