// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_AFLAGS_H
#define ANTIQUACRM_AFLAGS_H

#include <QFlags>

namespace AntiquaCRM {

  /**
    * @brief Mailler button groups
    * @ingroup Flags
    */
  enum MailGroup {
    NO_MAIL_GROUP = 0x0,      /**< no button set */
    MAIL_ORDER_GROUP = 0x1,   /**< orders section */
    MAIL_CUSTOMER_GROUP = 0x2 /**< customers section */
  };
  Q_DECLARE_FLAGS(MailGroups, MailGroup);

  /**
    * @brief Printer buttons
    * @ingroup Flags
    */
  enum PrinterGroup {
    PRINT_NOBUTTON = 0x0, /**< no button set */
    PRINT_DELIVERY = 0x1, /**< enable delivery button */
    PRINT_INVOICE = 0x2,  /**< enable invoice button */
    PRINT_REMINDER = 0x4, /**< enable reminder button */
    PRINT_BOOKCARD = 0x8, /**< enable bookcard button */
    PRINT_REFUND = 0x16,   /**< enable refunding button */
    PRINT_ADMONITION = 0x32  /**< enable invoice warning */
  };
  Q_DECLARE_FLAGS(PrinterGroups, PrinterGroup);

} // namespace AntiquaCRM

Q_DECLARE_OPERATORS_FOR_FLAGS(AntiquaCRM::MailGroups);

Q_DECLARE_OPERATORS_FOR_FLAGS(AntiquaCRM::PrinterGroups);

#endif // ANTIQUACRM_AFLAGS_H
