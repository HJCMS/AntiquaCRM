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
      Sections sections READ getSections WRITE setSections NOTIFY sectionChanged)

private:
  QAction *ac_message;
  QAction *ac_status;
  QAction *ac_invoice;
  QAction *ac_canceld;

private Q_SLOTS:
  void setSimpleMail();
  void setStatusMail();
  void setInvoceMail();
  void setCancelMail();

Q_SIGNALS:
  /**
   * @brief Signal Änderungen bei den Flags
   */
  void sectionChanged();

  /**
   * @brief E-Mail Nachricht von Konstante senden ...
   * @list Werte werden aus PgSQL::ui_template_body entnommen:
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
    NoButton = 0x0000,  /**< Keine */
    Orders = 0x0001,    /**< Bestell/Auftragsnachrichten */
    Customers = 0x0002  /**< Kunden Nachricht */
  };
  Q_DECLARE_FLAGS(Sections, SectionFlag)
  Q_FLAG(Sections)

  /**
   * @brief PrinterButton
   */
  explicit MailButton(QWidget *parent = nullptr,
                      MailButton::Sections flags = NoButton);

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

#endif // MAILBUTTON_UTILS_H
