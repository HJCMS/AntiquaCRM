// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMERCONTACT_H
#define COSTUMERCONTACT_H

#include <QObject>
#include <QWidget>

#include <Utils>

namespace HJCMS {
class SqlCore;
};

class CustomerContact : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  GenderBox *c_gender;      /**< Geschlecht */
  SalutationBox *c_title;   /**< Titelanrede */
  LineEdit *c_firstname;    /**< Vorname */
  LineEdit *c_lastname;     /**< Nachname */
  PostalCode *c_postalcode; /**< Postleitzahl */
  LineEdit *c_country;      /**< Land */
  EUCountryBox *c_country_bcp47; /**< IETF BCP 47 */
  LineEdit *c_location;     /**< Wohnort */
  LineEdit *c_street;       /**< Straße */
  PhoneEdit *c_phone_0;     /**< Telefon Nummer 1 */
  PhoneEdit *c_phone_1;     /**< Telefon Nummer 2 */
  PhoneEdit *c_mobil_0;     /**< Mobilerufnummer 1 */
  PhoneEdit *c_mobil_1;     /**< Mobilerufnummer 2 */
  PhoneEdit *c_fax_0;       /**< Faxnummer */
  EMailEdit *c_email_0;     /**< E-Mail Adresse */
  EMailEdit *c_email_1;     /**< E-Mail Adresse */
  LineEdit *c_website;      /**< Webseite */
  GroupBox *c_company;      /**< Firma/Institut oder Organisation ? */
  StrLineEdit *c_company_name; /**< Firmen Name */
  LineEdit *c_company_employer;  /**< Ansprechpartner */
  TextField *c_postal_address;   /**< Adresse */
  TextField *c_shipping_address; /**< Lieferadresse */

  explicit CustomerContact(QWidget *parent = nullptr);

private:
  HJCMS::SqlCore *m_sql;    /**< Wird für einige Unterabfragen benötigt! */

  /**
   * Mit Postleitzahl Wohnort und Bundesland ermitteln!
   * @note Wird von @ref postalCodeComplite() ausgelöst!
   */
  void fetchCountryFromPostal();

private Q_SLOTS:
  /**
   * Wenn @ref c_postalcode das Signal editingFinished
   * absetzt @ref fetchCountryFromPostal aufrufen!
   */
  void postalCodeComplite();

  /**
   * Wenn Knopf generieren geklickt wird.
   * Erstellt wir hier den Adressenkopf für den Kunden.
   * @note Es müssen alle Adress Datenfelder gesetzt sein.
   */
  void generateAddressBody();

  // Q_SIGNALS:
};

#endif // COSTUMERCONTACT_H
