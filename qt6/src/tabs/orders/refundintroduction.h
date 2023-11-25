// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_INTRODUCTION_H
#define ANTIQUACRM_REFUNDING_INTRODUCTION_H

#include "refundingpage.h"
#include <QPushButton>

/**
 * @class RefundIntroduction
 * @brief Introduction to the refund process.
 *
 * @ingroup PluginOrders
 */
class ANTIQUACRM_LIBRARY RefundIntroduction final : public RefundingPage {
  Q_OBJECT

private:
  QLabel *m_label;
  QPushButton *btn_ready;

public:
  /**
   * @param parent - pointer to parent object
   */
  explicit RefundIntroduction(QWidget *parent = nullptr);
  bool initPageData(AntiquaCRM::ASqlCore *con, qint64 id) override;
};

#endif // ANTIQUACRM_REFUNDING_INTRODUCTION_H
