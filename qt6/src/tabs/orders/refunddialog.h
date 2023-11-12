// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_DIALOG
#define ANTIQUACRM_REFUNDING_DIALOG

#include <AntiquaWidgets>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

#ifndef ANTIQUACRM_REFUNDING_PAGES
#include "refundingpages.h"
#endif

/**
 * @class RefundingDialog
 * @brief Refunding order dialog
 *
 * A step-by-step dialog that goes through some important questions about
 * submitting a refund.
 *
 * @ingroup PluginOrders
 */
class RefundingDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  /**
   * @brief current order id
   *
   * Will set in constructor and can not changed.
   */
  const qint64 p_orderid;

  RefundIntroduction *m_intro;
  RefundStepOne *m_page1;
  RefundStepTwo *m_page2;

  /**
   * @brief submit button
   */
  QPushButton *btn_commit;

  /**
   * @brief Dialog follow context widget
   */
  QStackedWidget *m_stackedWidget;

  /**
   * @brief Navigation bar, not dialog button box!
   */
  AntiquaCRM::ANavigationBar *m_navBar;

private Q_SLOTS:
  void goForward();
  void goBackward();

  /**
   * @brief execute refunding action
   *
   * @todo final process description
   */
  void commit();

public:
  /**
   * @param orderId - Id from order, which will be refunded.
   * @param parent - parent object
   */
  explicit RefundingDialog(qint64 orderId, QWidget *parent = nullptr);

  /**
   * @brief opens this dialog with some checks
   *
   * @li inital PgSQL connection
   * @li validates given dataset
   * @li fills most important formula data
   *
   * @return QDialog::DialogCode;
   */
  int exec() override;
};

#endif // ANTIQUACRM_REFUNDING_DIALOG
