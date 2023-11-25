// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_SELECTER_H
#define ANTIQUACRM_REFUNDING_SELECTER_H

#include "refundingpage.h"

#include <QLineEdit>
#include <QList>
#include <QRegularExpression>
#include <QSqlRecord>

class OrdersTableView;

class ANTIQUACRM_LIBRARY RefundSelecter final : public RefundingPage {
  Q_OBJECT

private:
  const QRegularExpression p_id_pattern;
  AntiquaCRM::SerialId *m_oid;      /**< @brief Order Id */
  AntiquaCRM::SerialId *m_iid;      /**< @brief Invoice Id */
  AntiquaCRM::SerialId *m_cid;      /**< @brief Customer Id */
  AntiquaCRM::TextLine *m_did;      /**< @brief Delivery Id */
  AntiquaCRM::TextField *m_address; /**< @brief Invoice Address */
  OrdersTableView *m_table;         /**< @brief Article orders */
  QLineEdit *m_refundsIds;          /**< @brief Selected Payment Ids */

private Q_SLOTS:
  /**
   * @brief Validate content and emit sendStatus
   */
  void refundParser(const QString &);

  /**
   * @brief add payment id with double clicked from table
   */
  void addPaymentRefund(qint64);

public:
  explicit RefundSelecter(QWidget *parent = nullptr);
  bool initPageData(AntiquaCRM::ASqlCore *con, qint64 id) override;
  const QList<qint64> selectedRefunds();
};

#endif // ANTIQUACRM_REFUNDING_SELECTER_H
