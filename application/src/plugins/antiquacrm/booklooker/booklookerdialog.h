// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_BOOKLOOKER_DIALOG_H
#define ANTIQUACRM_BOOKLOOKER_DIALOG_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QStatusBar>
#include <QWidget>

/**
 * @brief The BooklookerDialog class
 * @ingroup BooklookerPlugin
 */
class ANTIQUACRM_LIBRARY BooklookerDialog
    : public AntiquaCRM::UpdateDialog {
  Q_OBJECT

private:
  QString p_order_id;
  QJsonObject p_system_data;
  QJsonObject p_post_data;

  QRadioButton *m_rb2;
  QRadioButton *m_rb3;
  QRadioButton *m_rb4;
  QRadioButton *m_rb5;
  QRadioButton *m_rb6;
  QRadioButton *m_rb7;
  QDialogButtonBox *m_buttonBar;
  QStatusBar *m_statusBar;

  bool prepareAction();

private Q_SLOTS:
  void networkAction();

public Q_SLOTS:
  void statusMessage(const QString &);

public:
  explicit BooklookerDialog(QWidget *parent = nullptr);
  int exec(const QString &orderId, const QJsonObject &data);
};

#endif // ANTIQUACRM_BOOKLOOKER_DIALOG_H
