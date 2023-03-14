// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CUSTOMERSSEARCHLINEEDIT_H
#define ANTIQUACRM_CUSTOMERSSEARCHLINEEDIT_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Customers Search line edit
 * @ingroup _customers
 */
class CustomersSearchLineEdit : public SearchLineEdit {
  Q_OBJECT

public:
  explicit CustomersSearchLineEdit(QWidget *parent = nullptr);
  const QString getSearch() override;
};

#endif // ANTIQUACRM_CUSTOMERSSEARCHLINEEDIT_H
