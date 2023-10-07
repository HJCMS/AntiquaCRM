// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PROVIDER_IMPORTS_EDIT_H
#define PROVIDER_IMPORTS_EDIT_H

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

class ImportsFindExisting;
class ImportCustomerEdit;
class ImportsNavBar;

/**
 * @ingroup _importsedit
 * @brief Select and modify Provider Import json data
 */
class ImportsEdit final : public QDialog {
  Q_OBJECT

private:
  const QString p_provider;
  const QString p_orderid;
  QJsonObject p_json;
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  QStackedWidget *m_mainWidget;
  ImportsFindExisting *m_finder;
  ImportCustomerEdit *m_cedit;
  ImportsNavBar *m_navbar;
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
  explicit ImportsEdit(const QString &provider, const QString &order,
                       QWidget *parent = nullptr);
  int exec() override;
};

#endif // PROVIDER_IMPORTS_EDIT_H
