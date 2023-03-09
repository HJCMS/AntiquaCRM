// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSBILLINGS_H
#define ANTIQUACRM_CUSTOMERSBILLINGS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Customers data billings widget
 * @ingroup _customers
 */
class CustomersBillings final : public QWidget {
  Q_OBJECT

public:
  BoolBox *c_locked;           /**< Gesperrt */
  TrustedBox *c_trusted;       /**< Vetrauenswürdigkeit */
  LineEdit *c_iban;            /**< IBAN Nummer */
  LineEdit *c_swift_bic;       /**< BIC Swift Code ISO 9362 */
  LineEdit *c_tax_id;          /**< Umsatzsteuer-Identifikationsnummer */
  EMailEdit *c_shipping_email; /**< E-Mail z.b: für PayPal */
  TextField *c_comments;       /**< Anmerkungen */

  explicit CustomersBillings(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_CUSTOMERSBILLINGS_H
