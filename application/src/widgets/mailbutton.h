// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_INPUT_MAILBUTTON_H
#define ANTIQUA_INPUT_MAILBUTTON_H

#include <QAction>
#include <QFlags>
#include <QMenu>
#include <QMetaObject>
#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @brief Vordefinierte E-Mail Aktionen aufrufen
 * @ingroup widgets
 */
class MailButton final : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(Sections sections READ getSections WRITE setSections NOTIFY sendSectionChanged)

private:
  QMenu *m_menu;
  bool createMailButtonActions();

private Q_SLOTS:
  void setMailAction();

Q_SIGNALS:
  /**
   * @brief Signal Änderungen bei den Flags
   */
  void sendSectionChanged();

  /**
   * @brief E-Mail Nachricht von Konstante senden ...
   * @see createMailButtonActions()
   * Werte werden aus PgSQL::ui_template_body entnommen:
   *  @li MAIL_ACTION_SIMPLE_MESSAGE
   *  @li MAIL_ACTION_SHIPPING_NOTICE
   *  @li MAIL_ACTION_INVOICE_BILLING
   *  @li MAIL_ACTION_ORDER_CANCELED
   */
  void sendMailAction(const QString &type);

public Q_SLOTS:
  /**
   * @brief Nur Anzeigen wenn eine E-Mail existiert!
   */
  void hasMailAddress(bool b = false);

public:
  /**
   * @brief Knopfanzeige
   */
  enum SectionFlag {
    NoButton = 0x0000, /**< Keine */
    Orders = 0x0001,   /**< Bestell/Auftragsnachrichten */
    Customers = 0x0002 /**< Kunden Nachricht */
  };
  Q_DECLARE_FLAGS(Sections, SectionFlag)
  Q_FLAG(Sections)

  /**
   * @brief PrinterButton
   */
  explicit MailButton(QWidget *parent = nullptr);

  /**
   * @brief Anzeige Einstellungen
   */
  void setSections(MailButton::Sections flags);

  /**
   * @brief Welche Flags sind gesetzt?
   */
  MailButton::Sections getSections();

private:
  MailButton::Sections sections;
};

#endif // ANTIQUA_INPUT_MAILBUTTON_H
