// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_INPUT_PRINTERBUTTON_H
#define ANTIQUA_INPUT_PRINTERBUTTON_H

#include <QAction>
#include <QFlags>
#include <QMetaObject>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include <AntiquaCRM>

/**
 * @brief Knopf+Menü für das öffnen der Druckerdialoge.
 * @ingroup widgets
 */
class PrinterButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(Buttons buttons READ getButtons WRITE setButtons NOTIFY sendButtonsChanged)

private:
  QAction *ac_delivery;
  QAction *ac_invoice;
  QAction *ac_reminder;
  QAction *ac_bookcard;

Q_SIGNALS:
  /**
   * @brief Signal Änderungen bei den Flags
   */
  void sendButtonsChanged();

  /**
   * @brief Signal Lieferschein drucken
   */
  void sendPrintDelivery();

  /**
   * @brief Signal Rechnung drucken
   */
  void sendPrintInvoice();

  /**
   * @brief Signal Zahlungserinnerung drucken.
   */
  void sendPaymentReminder();

  /**
   * @brief Signal Buckarte drucken
   */
  void sendPrintBookcard();

public:
  /**
   * @brief Knopfanzeige
   */
  enum ButtonFlag {
    NoButton = 0x0000,
    Delivery = 0x0001,
    Invoice = 0x0002,
    Reminder = 0x0004,
    Bookcard = 0x0008
  };
  Q_DECLARE_FLAGS(Buttons, ButtonFlag)
  Q_FLAG(Buttons)

  /**
   * @brief PrinterButton
   */
  explicit PrinterButton(QWidget *parent = nullptr,
                         PrinterButton::Buttons flags = NoButton);

  /**
   * @brief Anzeige Einstellungen
   */
  void setButtons(PrinterButton::Buttons flags);

  /**
   * @brief Welche Flags sind gesetzt?
   */
  PrinterButton::Buttons getButtons();

private:
  PrinterButton::Buttons buttons;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PrinterButton::Buttons)

#endif // ANTIQUA_INPUT_PRINTERBUTTON_H
