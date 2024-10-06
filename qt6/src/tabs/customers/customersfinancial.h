// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSFINANCIAL_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSFINANCIAL_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class CustomersTrustLevel;

/**
 * @brief Customers Financial data widget
 * @ingroup _customers
 */
class CustomersFinancial final : public QWidget {
  Q_OBJECT

public:
  AntiquaCRM::BoolBox* c_locked;
  CustomersTrustLevel* c_trusted;
  AntiquaCRM::TextLine* c_iban;
  AntiquaCRM::TextLine* c_swift_bic; /**< BIC Swift Code ISO 9362 */
  AntiquaCRM::TextLine* c_tax_id;
  AntiquaCRM::URILineEdit* c_shipping_email;
  AntiquaCRM::TextField* c_comments;
  explicit CustomersFinancial(QWidget* parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSFINANCIAL_H
