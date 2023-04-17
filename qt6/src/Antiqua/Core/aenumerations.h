// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_AENUMERATIONS_H
#define ANTIQUACRM_AENUMERATIONS_H

#include <QObject>

namespace AntiquaCRM {
  Q_NAMESPACE

  /**
   * @ingroup Enumerations
   * @brief Nachrichtentyp
   * Definiert die Dringlichkeit einer Nachricht an das Meldungssystem!
   */
  enum Message {
    NORMAL = 0,  /**< Standard Info */
    WARNING = 1, /**< Warnung wird ausgegeben! */
    FATAL = 2,   /**< Schwehrwiegender Fehler ist aufgetreten! */
    LOGGING = 3  /**< Nur für das Protokollieren vorgesehen! */
  };
  Q_ENUM_NS(AntiquaCRM::Message)

  /**
   * @ingroup Enumerations
   * @brief Typenangabe für die Netzwerkanfragen Schnittstelle!
   */
  enum NetworkQueryType { JSON_QUERY = 0, XML_QUERY = 1 };
  Q_ENUM_NS(AntiquaCRM::NetworkQueryType)

  /**
   * @ingroup Enumerations
   * @brief Auftrag-Bearbeitungsstatus
   */
  enum OrderStatus {
    OPEN = 0,      /**< Offen */
    STARTED = 1,   /**< Auftrag angenommen */
    FETCHET = 2,   /**< Bereit zur Abholung */
    DELIVERY = 3,  /**< Lieferung ist Unterwegs */
    DELIVERED = 4, /**< Paket wurde geliefert */
    COMPLETED = 5, /**< @deprecated Lieferung abgeschlossen */
    CANCELED = 6   /**< Storniert */
  };
  Q_ENUM_NS(AntiquaCRM::OrderStatus)

  /**
   * @ingroup Enumerations
   * @brief Auftrag-Zahlungsstatus
   */
  enum OrderPayment {
    NOTPAID = 0,  /**< Warte auf Zahlung */
    PAYED = 1,    /**< Bezahlt */
    REMIND = 2,   /**< Erinnert */
    ADMONISH = 3, /**< Mahnen */
    RETURN = 4,   /**< Retour */
    COLLPROC = 5  /**< Collection procedures/Inkassoverfahren */
  };
  Q_ENUM_NS(AntiquaCRM::OrderPayment)

  /**
   * @ingroup Enumerations
   * @brief Bestellstatus des Dienstleisters
   * Wird im Auftragssystem verwendet!
   */
  enum ProviderPaymentStatus {
    STATUS_NOT_SET = 0,           /**< Kein Status fesgelegt */
    WAIT_FOR_PAYMENT = 1,         /**< Warte auf Zahlung */
    SHIPMENT_CREATED = 2,         /**< Fertig zum Versand */
    SHIPPED_WAIT_FOR_PAYMENT = 3, /**< Geliefert warte auf Zahlung */
    SHIPPED_AND_PAID = 4,         /**< Bezahlt und versendet */
    BUYER_NO_REACTION = 5,        /**< Keine Reaktion des Käufers */
    ORDER_CANCELED = 6            /**< Käufer hat die Bestellung storniert */
  };
  Q_ENUM_NS(AntiquaCRM::ProviderPaymentStatus)

  /**
   * @ingroup Enumerations
   * @brief Zahlungsart der Bestellung
   * Wird im Auftragssystem verwendet um die Zahlungsart zu Zeigen.
   * Die Umsetzung dieser Vorgabe liegt bei der Plugin Integration!
   */
  enum PaymentMethod {
    PAYMENT_NOT_SET = 0,            /**< Keine Auswahl vorhanden */
    BANK_PREPAYMENT = 1,            /**< Banküberweisung Vorkasse */
    DELIVER_WITH_INVOICE = 2,       /**< Lieferung mit offener Rechnung */
    DIRECT_DEBIT_PREPAYMENT = 3,    /**< Lastschrift Vorkasse */
    CREDIT_CARD_PREPAYMENT = 4,     /**< Kreditkarte Vorkasse */
    CASH_ON_DELIVERY = 5,           /**< Zahlung mit Nachnahme */
    PAYPAL_PREPAYMENT = 6,          /**< PayPal Vorkasse */
    SKRILL_PREPAYMENT = 7,          /**< Skrill Vorkasse */
    GIROPAY_PREPAYMENT = 8,         /**< GiroPay Vorkasse */
    GOOGLEPAY_PREPAYMENT = 9,       /**< GooglePay Vorkasse */
    UNKNOWN_PREPAYMENT = 10,        /**< Vorkasse unbekannter Anbieter */
    LOCAL_PICKUP_CASH_PAYMENT = 11, /**< Selbstabholung - Barzahlung */
    INSTANT_BANK_TRANSFER = 12,     /**< Sofortüberweisung */
    INVOICE_PREPAYMENT_RESERVED =
        13,                /**< Offene Rechnung - Vorkasse vorbehalten */
    CHECK_PREPAYMENT = 14, /**< Scheck - Vorkasse */
  };
  Q_ENUM_NS(AntiquaCRM::PaymentMethod)

  /**
   * @ingroup Enumerations
   * @brief Kunden Vertrauensebene festlegen!
   */
  enum CustomerTrustLevel {
    NO_TRUST_LEVEL = 0, /**< Keine Vertrauensebene festgelegt */
    ON_TIME = 1,        /**< Zahlt Pünktlich */
    RELIABLE = 2,       /**< Zahlt Zuverlässig */
    WITH_DELAY = 3,     /**< Zahlt mit Verspätung */
    PREPAYMENT = 4,     /**< Nur mit Vorauszahlung */
    NO_DELIVERY = 5     /**< Keine Lieferung */
  };
  Q_ENUM_NS(AntiquaCRM::CustomerTrustLevel)

  /**
   * @ingroup Enumerations
   * @brief Geschlechter Definition
   */
  enum Gender {
    NO_GENDER = 0, /**< Ohne Angabe */
    MALE = 1,      /**< Männlich */
    FEMALE = 2,    /**< Weiblich */
    VARIOUS = 3    /**< Diverse */
  };
  Q_ENUM_NS(AntiquaCRM::Gender)

  /**
   * @ingroup Enumerations
   * @brief Inventory Gruppen
   * Umsetzung für Dienstleister kategorien.
   */
  enum ArticleType {
    UNKNOWN = 0, /**< nicht festgelegt */
    BOOK = 1,    /**< Bücher */
    MEDIA = 2,   /**< Film & Tonträger */
    PRINTS = 3,  /**< Drucke & Stiche */
    OTHER = 4    /**< Diverse */
  };
  Q_ENUM_NS(AntiquaCRM::ArticleType)

  /**
   * @ingroup Enumerations
   * @brief SalesTax
   * Rechnungsmöglichkeiten bei der Umsatzsteuerangabe (UStG §14)
   */
  enum SalesTax {
    TAX_NOT = 0,  /**< without sales tax */
    TAX_INCL = 1, /**< including sales tax */
    TAX_WITH = 2  /**< with sales tax  */
  };
  Q_ENUM_NS(AntiquaCRM::SalesTax)

  /**
   * @ingroup Enumerations
   * @brief MediaType
   * Umsetzung Musik Medien
   */
  enum MediaType {
    MEDIA_NO_TYPE = 0,            /**< nicht festgelegt */
    MEDIA_DISC_COMPACT = 1,       /**< Compact Disc */
    MEDIA_DISC_MINI = 2,          /**< Mini Compact Disc */
    MEDIA_DISC_VIDEO = 3,         /**< Video Compact Disc */
    MEDIA_VINYL_SINGLE = 4,       /**< Vinyl single record */
    MEDIA_VINYL_LONGPLAY = 5,     /**< Vinyl longplay record */
    MEDIA_VINYL_MULTI_GROOVE = 6, /**< Vinyl Multi Groove */
    MEDIA_VINYL_COLORED = 7,      /**< Vinyl colored record */
    MEDIA_VINYL_VIDEO = 8,        /**< Vinyl video record */
    MEDIA_VINYL_DISC = 9          /**< Vinyl record and DISC */
  };
  Q_ENUM_NS(AntiquaCRM::MediaType)

  /**
   * @ingroup Enumerations
   * @brief Zustandsbeschreibung für Artikel
   */
  enum Condition {
    NO_CONDITION = 0, /**< Ohne Angabe */
    FINE = 1,         /**< Sehr gut, fast neuwertig! */
    GOOD = 2,         /**< Leichte Gebrauchsspuren */
    SATISFYING = 3,   /**< Deutliche Gebrauchsspuren */
    SUFFICIENT = 4    /**< Stark abgenutzt */
  };
  Q_ENUM_NS(AntiquaCRM::Condition)

  /**
   * @brief Mailler buttons
   */
  enum MailGroup {
    NO_MAIL_GROUP = 0x0,      /**< @brief No buttons */
    MAIL_ORDER_GROUP = 0x1,   /**< @brief Order System Button */
    MAIL_CUSTOMER_GROUP = 0x2 /**< @brief Customers Section */
  };
  Q_DECLARE_FLAGS(MailGroups, MailGroup);

  /**
   * @brief Printer buttons
   */
  enum PrinterGroup {
    PRINT_NOBUTTON = 0x0, /**< @brief No buttons */
    PRINT_DELIVERY = 0x1, /**< @brief Enable Delivery button */
    PRINT_INVOICE = 0x2,  /**< @brief Enable Invoice button */
    PRINT_REMINDER = 0x4, /**< @brief Enable Reminder button */
    PRINT_BOOKCARD = 0x8  /**< @brief Enable Bookcard button */
  };
  Q_DECLARE_FLAGS(PrinterGroups, PrinterGroup);

}; // namespace AntiquaCRM

Q_DECLARE_OPERATORS_FOR_FLAGS(AntiquaCRM::MailGroups);

Q_DECLARE_OPERATORS_FOR_FLAGS(AntiquaCRM::PrinterGroups);

#endif // ANTIQUACRM_AENUMERATIONS_H
