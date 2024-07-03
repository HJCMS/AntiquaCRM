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
class ChangeCustomerWidget;

/**
 * @brief Change orders customers data dialog
 */
class ChangeOrderCustomerDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  QStackedWidget* m_mainWidget;
  ChangeCustomerFind* m_findCustomer;
  ChangeCustomerWidget* m_dataWidget;

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
   * @brief Search customer for change order
   */
  void setPage1();

  /**
   * @brief View customer data and confirm.
   */
  void setPage2(qint64);

  /**
   * @brief Check current selection, then commit sql and quit on success.
   */
  void apply();
  void close();

public:
  explicit ChangeOrderCustomerDialog(QWidget* parent = nullptr);

  /**
   * @brief open dialog
   * @param cid - Customer Id
   * @note This is a overload function to exec()
   */
  qint64 start(qint64 cid);
};

#endif // ANTIQUACRM_PLUGIN_CHANGEORDERCUSTOMERDIALOG_H
