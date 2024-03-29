// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ABEBOOKS_DIALOG_H
#define ANTIQUACRM_ABEBOOKS_DIALOG_H

#include <AntiquaCRMPlugin>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

/**
 * @brief The AbeBooksDialog class
 * @ingroup AntiquaCRMPlugin
 */
class ANTIQUACRM_LIBRARY AbeBooksDialog : public AntiquaCRM::UpdateDialog {
  Q_OBJECT

protected:
  QString p_order_id;
  QString p_seller_id;
  QLabel *m_info;
  QJsonObject p_system_data;
  QJsonObject p_post_data;

  bool prepareAction();

protected Q_SLOTS:
  void networkAction();

public Q_SLOTS:
  void statusMessage(const QString &){};

Q_SIGNALS:
  void sendPluginAction(const QJsonObject &);

public:
  explicit AbeBooksDialog(QWidget *parent = nullptr);
  int exec(const QString &orderId, const QJsonObject &data);
};

#endif // ANTIQUACRM_ABEBOOKS_DIALOG_H
