// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CHANGECUSTOMERWIDGET_H
#define ANTIQUACRM_PLUGIN_CHANGECUSTOMERWIDGET_H

#include <AntiquaWidgets>
#include <QLineEdit>
#include <QTextEdit>
#include <QWidget>

class ChangeCustomerWidget final : public QWidget {
  Q_OBJECT
  Q_PROPERTY(qint64 customerId READ getCustomerId WRITE setCustomerId NOTIFY sendCustomerChanged)

private:
  qint64 customerId = -1;
  AntiquaCRM::ASqlCore* m_sql = nullptr;
  AntiquaCRM::SerialId* m_customerId;
  AntiquaCRM::TextLine* m_fullName;
  AntiquaCRM::TextField* m_invoice;
  AntiquaCRM::TextField* m_delivery;

Q_SIGNALS:
  void sendCustomerChanged();

public Q_SLOTS:
  void setCustomerId(qint64);

public:
  explicit ChangeCustomerWidget(QWidget* parent = nullptr);
  ~ChangeCustomerWidget();

  qint64 getCustomerId();

  /**
   * @brief load customer data with id
   * @param cid - customer id
   *
   * This will create a sql connection from current environment.
   * If \i cid is null, the \b customerId property is used.
   * When \b customerId is lower then 1 nothing will done.
   *
   * This function returning \b false, when \b customerId property is not set
   * or no result exists!
   */
  bool loadCostumerData(qint64 cid = 0);
};

#endif // ANTIQUACRM_PLUGIN_CHANGECUSTOMERWIDGET_H
