// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ABEBOOKS_PLUGIN_ORDER_MAIN_INFO_H
#define ABEBOOKS_PLUGIN_ORDER_MAIN_INFO_H

#include <AGlobal>
#include <QDomElement>
#include <QDomNode>
#include <QLabel>
#include <QTextEdit>
#include <QWidget>

class ANTIQUACRM_LIBRARY AbeBooksOrderMainInfo final : public QWidget {
  Q_OBJECT

private:
  QTextEdit *m_address;
  QLabel *m_orderId;
  QLabel *m_buyerId;
  QLabel *m_purchaseId;
  QLabel *m_email;

  void mailingAddress(const QDomNodeList &);
  void setMail(const QString &);

public:
  explicit AbeBooksOrderMainInfo(QWidget *parent = nullptr);
  void setContentData(const QDomElement &);
  void setOrderId(const QString &);
  void setOrderPurchaseId(const QString &);
};

#endif // ABEBOOKS_PLUGIN_ORDER_MAIN_INFO_H
