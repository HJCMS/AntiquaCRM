// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILBUTTON_UTILS_H
#define MAILBUTTON_UTILS_H

#include <QAction>
#include <QFlags>
#include <QMetaObject>
#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @brief Vordefinierte E-Mail Aktionen aufrufen
 * @class MailButton
 */
class MailButton final : public QPushButton {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(
      Buttons buttons READ getButtons WRITE setButtons NOTIFY buttonsChanged)

private:
  QAction *ac_invoice;
  QAction *ac_simple;

Q_SIGNALS:
  /**
   * @brief Signal Änderungen bei den Flags
   */
  void buttonsChanged();

  /**
   * @brief Rechnungs E-Mail
   */
  void sendMailInvoce();

  /**
   * @brief Einfache Nachricht
   */
  void sendMailSimple();

public:
  /**
   * @brief Knopfanzeige
   */
  enum ButtonFlag {
    NoButton = 0x0000, /**< Keine */
    Invoice = 0x0001,  /**< Rechnung/Vorkasse */
    Simple = 0x0002    /**< Einfache Nachricht */
  };
  Q_DECLARE_FLAGS(Buttons, ButtonFlag)
  Q_FLAG(Buttons)

  /**
   * @brief PrinterButton
   */
  explicit MailButton(QWidget *parent = nullptr,
                      MailButton::Buttons flags = NoButton);

  /**
   * @brief Anzeige Einstellungen
   */
  void setButtons(MailButton::Buttons flags);

  /**
   * @brief Welche Flags sind gesetzt?
   */
  MailButton::Buttons getButtons();

private:
  MailButton::Buttons buttons;
};

#endif // MAILBUTTON_UTILS_H
