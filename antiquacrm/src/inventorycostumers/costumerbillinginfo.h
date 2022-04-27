// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMERBILLINGINFO_H
#define COSTUMERBILLINGINFO_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

#include <Utils>

namespace HJCMS {
class SqlCore;
};

class CostumerBillingInfo : public QWidget {
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

  explicit CostumerBillingInfo(QWidget *parent = nullptr);
};

#endif // COSTUMERBILLINGINFO_H
