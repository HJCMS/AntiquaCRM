// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PROVIDERPURCHASEHEADER_H
#define ANTIQUACRM_PROVIDERPURCHASEHEADER_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QWidget>

/**
 * @brief The ProviderPurchaseHeader class
 * @ingroup _providers
 */
class ProviderPurchaseHeader final : public QFrame {
  Q_OBJECT

private:
  QLabel *m_cid;
  QLabel *m_buyer;

public:
  qint64 CustomerId = -1;
  explicit ProviderPurchaseHeader(QWidget *parent = nullptr);
  void setHeader(const QString &name, qint64 customerId = -1);
};

#endif // ANTIQUACRM_PROVIDERPURCHASEHEADER_H
