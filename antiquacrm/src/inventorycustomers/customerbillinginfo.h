// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CUSTOMERBILLINGINFO_H
#define CUSTOMERBILLINGINFO_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

#include <Utils>

namespace HJCMS {
class SqlCore;
};

class CustomerBillingInfo : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  BoolBox *c_locked;     /**< Gesperrt */
  TrustBox *c_trusted;   /**< Vetrauenswürdigkeit */
  LineEdit *c_iban;      /**< IBAN Nummer */
  LineEdit *c_swift_bic; /**< BIC Swift Code ISO 9362 */
  LineEdit *c_tax_id;    /**< Umsatzsteuer-Identifikationsnummer */
  TextField *c_comments; /**< Anmerkungen */

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit CustomerBillingInfo(QWidget *parent = nullptr);
};

#endif // CUSTOMERBILLINGINFO_H
