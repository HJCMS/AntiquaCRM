// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_DIALOG
#define ANTIQUACRM_REFUNDING_DIALOG

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

/**
 * @class RefundingDialog
 * @brief Orders refunding dialog
 *
 * @ingroup PluginOrders
 */
class RefundingDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  qint64 p_orderid;
  QPushButton *btn_commit;
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  QStackedWidget *m_stackedWidget;
  AntiquaCRM::ANavigationBar *m_navBar;

private Q_SLOTS:
  void commit();

public:
  explicit RefundingDialog(qint64 orderId, QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_REFUNDING_DIALOG
