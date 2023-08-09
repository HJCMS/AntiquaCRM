// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_AENUMERATIONS_H
#define ANTIQUACRM_AENUMERATIONS_H

#include <QObject>

namespace AntiquaCRM {
  Q_NAMESPACE

  /**
    * @brief Type specification for the network request interface!
    * @ingroup Enumerations
    */
  enum NetworkQueryType { JSON_QUERY = 0, XML_QUERY = 1 };
  Q_ENUM_NS(AntiquaCRM::NetworkQueryType)

  /**
    * @brief Order processing status
    * @ingroup Enumerations
    */
  enum OrderStatus {
    OPEN = 0,      /**< open */
    STARTED = 1,   /**< order accepted */
    FETCHET = 2,   /**< ready for pickup */
    DELIVERY = 3,  /**< delivery is on the way */
    DELIVERED = 4, /**< package has been delivered */
    COMPLETED = 5, /**< @deprecated delivery completed */
    CANCELED = 6   /**< canceled */
  };
  Q_ENUM_NS(AntiquaCRM::OrderStatus)

  /**
    * @brief Order payment status
    * @ingroup Enumerations
    */
  enum OrderPayment {
    NOTPAID = 0,  /**< waiting for payment */
    PAYED = 1,    /**< paid */
    REMIND = 2,   /**< remindet */
    ADMONISH = 3, /**< admonish */
    RETURN = 4,   /**< returns */
    COLLPROC = 5  /**< collection procedures */
  };
  Q_ENUM_NS(AntiquaCRM::OrderPayment)

  /**
    * @brief Order status of the service provider
    * @note Used in the order system!
    * @ingroup Enumerations
    */
  enum ProviderPaymentStatus {
    STATUS_NOT_SET = 0,           /**< No status set */
    WAIT_FOR_PAYMENT = 1,         /**< Warte auf Zahlung */
    SHIPMENT_CREATED = 2,         /**< Fertig zum Versand */
    SHIPPED_WAIT_FOR_PAYMENT = 3, /**< Geliefert warte auf Zahlung */
    SHIPPED_AND_PAID = 4,         /**< Bezahlt und versendet */
    BUYER_NO_REACTION = 5,        /**< Keine Reaktion des Käufers */
    ORDER_CANCELED = 6            /**< Käufer hat die Bestellung storniert */
  };
  Q_ENUM_NS(AntiquaCRM::ProviderPaymentStatus)

  /**
    * @brief Payment method of the order
    *
    * Used in the order system to show the payment method.
    * The implementation of this specification is up to the plugin integration!
    *
    * @ingroup Enumerations
    */
  enum PaymentMethod {
    PAYMENT_NOT_SET = 0,              /**< unspecified */
    BANK_PREPAYMENT = 1,              /**< bank transfer (prepayment) */
    DELIVER_WITH_INVOICE = 2,         /**< delivery with open invoice */
    DIRECT_DEBIT_PREPAYMENT = 3,      /**< direct debit payment in advance */
    CREDIT_CARD_PREPAYMENT = 4,       /**< Credit card in advance */
    CASH_ON_DELIVERY = 5,             /**< Cash on delivery payment */
    PAYPAL_PREPAYMENT = 6,            /**< PayPal in advance */
    SKRILL_PREPAYMENT = 7,            /**< Skrill in advance */
    GIROPAY_PREPAYMENT = 8,           /**< GiroPay in advance */
    GOOGLEPAY_PREPAYMENT = 9,         /**< Google Pay in advance */
    UNKNOWN_PREPAYMENT = 10,          /**< Payment in advance from an unknown */
    LOCAL_PICKUP_CASH_PAYMENT = 11,   /**< Pickup - cash payment */
    INSTANT_BANK_TRANSFER = 12,       /**< Instant bank transfer */
    INVOICE_PREPAYMENT_RESERVED = 13, /**< open - Advance payment reserved */
    CHECK_PREPAYMENT = 14,            /**< Check - payment in advance */
  };
  Q_ENUM_NS(AntiquaCRM::PaymentMethod)

  /**
    * @brief Set customer trust level!
    * @ingroup Enumerations
    */
  enum CustomerTrustLevel {
    NO_TRUST_LEVEL = 0, /**< unspecified */
    ON_TIME = 1,        /**< Pays on time */
    RELIABLE = 2,       /**< Pays reliably */
    WITH_DELAY = 3,     /**< Pay late */
    PREPAYMENT = 4,     /**< Only with prepayment */
    NO_DELIVERY = 5     /**< No delivery */
  };
  Q_ENUM_NS(AntiquaCRM::CustomerTrustLevel)

  /**
    * @brief gender definition
    * @ingroup Enumerations
    */
  enum Gender {
    NO_GENDER = 0, /**< unspecified */
    MALE = 1,      /**< male */
    FEMALE = 2,    /**< female */
    VARIOUS = 3    /**< various */
  };
  Q_ENUM_NS(AntiquaCRM::Gender)

  /**
    * @brief Inventory groups
    *
    * Implementation for service categories.
    *
    * @ingroup Enumerations
    */
  enum ArticleType {
    UNKNOWN = 0, /**< unspecified */
    BOOK = 1,    /**< books */
    MEDIA = 2,   /**< media - CD,Vinyl,DVD */
    PRINTS = 3,  /**< prints and stitches */
    OTHER = 4    /**< other or various */
  };
  Q_ENUM_NS(AntiquaCRM::ArticleType)

  /**
    * @brief TAX type
    *
    * Invoicing options for sales tax information (UStG §14)
    *
    * @ingroup Enumerations
    */
  enum SalesTax {
    TAX_NOT = 0,  /**< without sales tax */
    TAX_INCL = 1, /**< including sales tax */
    TAX_WITH = 2  /**< with sales tax  */
  };
  Q_ENUM_NS(AntiquaCRM::SalesTax)

  /**
    * @brief Media type
    *
    * Implementation of media type information
    *
    * @ingroup Enumerations
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
    * @brief Item condition description
    * @ingroup Enumerations
    */
  enum Condition {
    NO_CONDITION = 0, /**< unspecified */
    FINE = 1,         /**< Very good, almost new! */
    GOOD = 2,         /**< Slight signs of wear */
    SATISFYING = 3,   /**< Significant signs of use */
    SUFFICIENT = 4    /**< Heavily worn */
  };
  Q_ENUM_NS(AntiquaCRM::Condition)

  /**
    * @brief Enumeration to know, how to store configs.
    * @ingroup Enumerations
    */
  enum ConfigType {
    CONFIG_SYSTEM = 0,  /**< store in user config */
    CONFIG_DATABASE = 1 /**< store in database */
  };
  Q_ENUM_NS(AntiquaCRM::ConfigType);

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_AENUMERATIONS_H
