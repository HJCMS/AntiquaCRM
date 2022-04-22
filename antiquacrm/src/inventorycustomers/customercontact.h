// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CUSTOMERCONTACT_H
#define CUSTOMERCONTACT_H

#include <QtCore/QObject>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class SalutationBox;
class GenderBox;
class PhoneEdit;
class EMailEdit;
class StrLineEdit;

class CustomerContact : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  GenderBox *c_gender;     /**< Geschlecht */
  SalutationBox *c_title;  /**< Titelanrede */
  QLineEdit *c_firstname;  /**< Vorname */
  QLineEdit *c_lastname;   /**< Nachname */
  QGroupBox *c_company;    /**< Firma/Institut oder Organisation ? */
  PhoneEdit *c_phone_0;    /**< Telefon Nummer 1 */
  PhoneEdit *c_phone_1;    /**< Telefon Nummer 2 */
  PhoneEdit *c_mobil_0;    /**< Mobilerufnummer 1 */
  PhoneEdit *c_mobil_1;    /**< Mobilerufnummer 2 */
  PhoneEdit *c_fax_0;      /**< Faxnummer */
  EMailEdit *c_email_0;    /**< E-Mail Adresse */
  EMailEdit *c_email_1;    /**< E-Mail Adresse */
  StrLineEdit *c_website;  /**< Webseite */

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit CustomerContact(QWidget *parent = nullptr);
};

#endif // CUSTOMERCONTACT_H
