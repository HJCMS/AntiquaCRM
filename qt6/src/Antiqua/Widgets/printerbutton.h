// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRINTER_BUTTON_H
#define ANTIQUACRM_WIDGETS_PRINTER_BUTTON_H

#include <AGlobal>
#include <QObject>
#include <QPushButton>

namespace AntiquaCRM {

/**
 * @class PrinterButton
 * @brief The PrinterButton class
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY PrinterButton final : public QPushButton {
  Q_OBJECT

private:
  AntiquaCRM::PrinterGroups settings;
  QAction *ac_delivery;
  QAction *ac_invoice;
  QAction *ac_reminder;
  QAction *ac_bookcard;

Q_SIGNALS:
  /**
   * @brief Signal changes in the flags
   */
  void sendButtonsChanged();

  /**
   * @brief Print delivery note signal
   */
  void sendPrintDelivery();

  /**
   * @brief Print signal invoice
   */
  void sendPrintInvoice();

  /**
   * @brief Print signal payment reminder
   */
  void sendPaymentReminder();

  /**
   * @brief Print signal bookcard
   */
  void sendPrintBookcard();

public:
  /**
   * @brief PrinterButton
   */
  explicit PrinterButton(QWidget *parent = nullptr,
                         AntiquaCRM::PrinterGroups flags = PRINT_NOBUTTON);

  /**
   * @brief Display settings
   */
  void setButtons(AntiquaCRM::PrinterGroups flags);

  /**
   * @brief Which flags are set?
   */
  AntiquaCRM::PrinterGroups getButtons();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINTER_BUTTON_H
