// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CHANGEORDERCUSTOMERDIALOG_H
#define ANTIQUACRM_PLUGIN_CHANGEORDERCUSTOMERDIALOG_H

#include <AntiquaWidgets>
#include <QLabel>
#include <QStackedWidget>

class ChangeCustomerFind;

/**
 * @brief Change orders customers data dialog
 */
class ChangeOrderCustomerDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  qint64 origin_customer_id = -1;
  QStackedWidget* m_mainWidget;
  ChangeCustomerFind* m_findCustomer;
  AntiquaCRM::CustomersDataWidget* m_dataWidget;

  /**
   * @brief prevent invalid dialog calls
   */
  int exec() override;

  /**
   * @brief Label with some hints for start page
   */
  QLabel* getPage0() const;

private Q_SLOTS:
  /**
   * @brief __TODO__
   */
  void setPage1();

  /**
   * @brief __TODO__
   */
  void setPage2();

  /**
   * @brief __TODO__
   */
  void setPage3();

  /**
   * @brief Checking, save and quit this dialog.
   */
  void apply();

public:
  explicit ChangeOrderCustomerDialog(QWidget* parent = nullptr);

  /**
   * @brief open dialog
   * @param customerId
   * @note This is a overload function to exec()
   */
  QDialog::DialogCode start(qint64 customerId);
};

#endif // ANTIQUACRM_PLUGIN_CHANGEORDERCUSTOMERDIALOG_H
