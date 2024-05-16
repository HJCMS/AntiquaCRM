// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_PURCHASEHEADER_H
#define ANTIQUACRM_PLUGIN_SELLERS_PURCHASEHEADER_H

#include <AntiquaWidgets>
#include <QLabel>

class ANTIQUACRM_LIBRARY PurchaseHeader final : public QWidget {
  Q_OBJECT

private:
  qint64 p_customer_id = -1;
  QLabel *m_buyer;
  QLabel *m_cid;

public:
  explicit PurchaseHeader(QWidget *parent = nullptr);
  qint64 getCustomerId();
  void setHeader(const QString &name, qint64 cid = -1);
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_PURCHASEHEADER_H
