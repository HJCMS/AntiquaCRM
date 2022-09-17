// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CUSTOMER_BILLING_INFO_H
#define CUSTOMER_BILLING_INFO_H

#include <QObject>
#include <QWidget>

#include <Utils>

class CustomerBillingInfo final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  BoolBox *c_locked;     /**< Gesperrt */
  TrustBox *c_trusted;   /**< Vetrauenswürdigkeit */
  LineEdit *c_iban;      /**< IBAN Nummer */
  LineEdit *c_swift_bic; /**< BIC Swift Code ISO 9362 */
  LineEdit *c_tax_id;    /**< Umsatzsteuer-Identifikationsnummer */
  EMailEdit *c_shipping_email; /**< E-Mail z.b: für PayPal */
  TextField *c_comments; /**< Anmerkungen */

  explicit CustomerBillingInfo(QWidget *parent = nullptr);
};

#endif // CUSTOMER_BILLING_INFO_H
