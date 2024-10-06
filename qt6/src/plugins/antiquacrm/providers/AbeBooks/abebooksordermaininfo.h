// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ABEBOOKS_PLUGIN_ORDER_MAIN_INFO_H
#define ABEBOOKS_PLUGIN_ORDER_MAIN_INFO_H

#include "abebooksconfig.h"
#include <AGlobal>
#include <QDomElement>
#include <QDomNode>
#include <QLabel>
#include <QTextEdit>
#include <QUrl>
#include <QWidget>

class ABEBOOKS_PLUGIN AbeBooksOrderMainInfo final : public QWidget {
  Q_OBJECT

private:
  QTextEdit *m_address;
  QLabel *m_orderId;
  QLabel *m_buyerId;
  QLabel *m_purchaseId;
  QLabel *m_email;
  QLabel *m_buyerInfo;

  void mailingAddress(const QDomNodeList &);
  void setMail(const QString &);

public Q_SLOTS:
  void setOrderLink(const QUrl &url);

public:
  explicit AbeBooksOrderMainInfo(QWidget *parent = nullptr);
  void setContentData(const QDomElement &);
  void setOrderId(const QString &);
  void setOrderPurchaseId(const QString &);
};

#endif // ABEBOOKS_PLUGIN_ORDER_MAIN_INFO_H
