// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_STEPONE_H
#define ANTIQUACRM_REFUNDING_STEPONE_H

#include "refundingpage.h"

#include <QSqlRecord>

class OrdersTableView;

class ANTIQUACRM_LIBRARY RefundStepOne final : public RefundingPage {
  Q_OBJECT

private:
  AntiquaCRM::SerialId *m_oid; /**< @brief Order Id */
  AntiquaCRM::SerialId *m_iid; /**< @brief Invoice Id */
  AntiquaCRM::SerialId *m_cid; /**< @brief Customer Id */
  AntiquaCRM::TextLine *m_did; /**< @brief Delivery Id */
  AntiquaCRM::TextField *m_address;

  OrdersTableView *m_table;

public:
  explicit RefundStepOne(QWidget *parent = nullptr);
  bool addDataRecord(qint64 id) override;
};

#endif // ANTIQUACRM_REFUNDING_STEPONE_H
