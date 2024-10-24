// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DELIVERYDIALOG_H
#define ANTIQUACRM_UTILS_DELIVERYDIALOG_H

#include <AntiquaWidgets>
#include <QDialog>
#include <QDialogButtonBox>
#include <QStatusBar>

class DeliverServiceEdit;
class DeliverServiceList;

class DeliveryDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  QSplitter *m_splitter;
  DeliverServiceEdit *m_edit;
  DeliverServiceList *m_list;
  QDialogButtonBox *m_buttonBox;
  QPushButton *ac_create;
  QStatusBar *m_statusBar;

  void initItemsTreeView();

private Q_SLOTS:
  void messanger(const QString &);
  void deleteDeliveryPackage(const QJsonObject &obj);

protected:
  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

private Q_SLOTS:
  void saveCurrent();
  void queryDeliverServices(int);
  void queryServicePackage(int, const QJsonObject &);

public:
  explicit DeliveryDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_UTILS_DELIVERYDIALOG_H
