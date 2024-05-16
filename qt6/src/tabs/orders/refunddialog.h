// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
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
  AntiquaCRM::ASqlCore *m_sql = nullptr;

  /**
   * @brief current order id
   *
   * Order Id will set in constructor and can not changed.
   */
  const qint64 p_orderid;

  /**
   * @brief Introduction to the process
   */
  RefundIntroduction *m_intro;

  /**
   * @brief Select return item from order.
   */
  RefundSelecter *m_select;

  /**
   * @brief Set and send cancellation costs.
   */
  RefundFinalize *m_final;

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
  void pageChanged(int);

  /**
   * @brief Page status update
   */
  void statusFromPage(bool);

  /**
   * @brief execute refunding action
   *
   * @todo final process description
   */
  void saveAndQuit();

public:
  /**
   * @param orderId - Id from order, which will be refunded.
   * @param parent - parent object
   */
  explicit RefundingDialog(qint64 orderId, QWidget *parent = nullptr);

  /**
   * @brief Call Refundings after accept
   */
  const QList<AntiquaCRM::OrderArticleItems> refundArticles();

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
