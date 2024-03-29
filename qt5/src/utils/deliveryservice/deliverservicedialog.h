// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DELIVERSERVICEDIALOG_H
#define DELIVERSERVICEDIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QJsonObject>
#include <QKeyEvent>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QWidget>

class DeliverServiceEdit;
class DeliverServiceList;

/**
 * @ingroup _deliveryservice
 * @brief Edit and fetch Delivery Services
 */
class DeliverServiceDialog : public QDialog {
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
  explicit DeliverServiceDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // DELIVERSERVICEDIALOG_H
