// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_FINALIZE_H
#define ANTIQUACRM_REFUNDING_FINALIZE_H

#include <AntiquaWidgets>
#include <QList>

#include "refundingpage.h"

class OrdersTableView;

class ANTIQUACRM_LIBRARY RefundFinalize final : public RefundingPage {
  Q_OBJECT

private:
  OrdersTableView *m_refundings;
  AntiquaCRM::PriceEdit *m_refundCost;

private Q_SLOTS:
  void refundsCostChanged();

public Q_SLOTS:
  void addArticles(const QList<qint64> &);

public:
  explicit RefundFinalize(QWidget *parent = nullptr);
  bool initPageData(AntiquaCRM::ASqlCore *m_sql, qint64 id) override;
};

#endif // ANTIQUACRM_REFUNDING_FINALIZE_H
