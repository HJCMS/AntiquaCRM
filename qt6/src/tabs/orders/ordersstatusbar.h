// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSSTATUSBAR_H
#define ANTIQUACRM_PLUGIN_ORDERSSTATUSBAR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QMenu>
#include <QPushButton>
#include <QSignalMapper>

class ANTIQUACRM_LIBRARY OrdersStatusBar final
    : public AntiquaCRM::TabsStatusBar {
  Q_OBJECT

private:
  /**
   * @brief Internal selection handling
   */
  enum FilterQuery {
    QF_DEFAULT = 0,
    QF_NOT_PAID = 1,
    QF_DELIVERED_NOT_PAID = 2,
    QF_COMPLETED = 3,
    QF_CANCELED = 4,
    QF_REFUNDS = 5
  };

  /**
   * @brief History Button
   */
  QPushButton *btn_history;

  /**
   * @brief History button actions
   */
  QAction *m_ac;

  /**
   * @brief Get year from OrdersSearchBar and build sql statement.
   */
  const QString inPastRange() const;

  void setHistoryActionMenu(QPushButton *parent) override;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override{/* unused */};

public:
  explicit OrdersStatusBar(QWidget *parent = nullptr);

  /**
   * @brief Create order button is disabled
   * @note A new Order creation needs a CustomerID!
   */
  bool isCreateButtonEnabled() override { return false; };
};

#endif // ANTIQUACRM_PLUGIN_ORDERSSTATUSBAR_H
