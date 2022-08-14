// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PAYMENTREMINDER_PRINTING_H
#define PAYMENTREMINDER_PRINTING_H

#include <QHash>
#include <QImage>
#include <QMetaType>
#include <QObject>
#include <QWidget>

#include <AntiquaCRM>

#include "printing.h"

/**
 * @brief Zahlungs Erinnerung Erstellung
 * @ingroup Printing
 * @class PaymentReminder
 */
class PaymentReminder : public Printing {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_titleText = tr("Payment Reminder");
  QString p_mainText;
  QString p_subText;
  /**
   * @brief Gesamtmenge
   */
  int p_quantity_sum = 0;

  /**
   * @brief Mehwertsteuer (inkl oder plus)
   */
  bool p_including_VAT;
  bool p_disable_VAT = false;

  /**
   * @brief Gesamtpreis
   */
  qreal p_fullPrice = 0.00;

  /**
   * @brief Versankosten
   */
  qreal p_packagePrice = 0.00;

  /**
   * @brief Artikelliste
   */
  QTextTable *m_billingTable;

protected:
  void constructSubject();
  void constructBody();

  /**
   * @brief Artikel in @ref m_articleTable einfügen.
   * @param articleid   - Artikel Nummer
   * @param designation - Bezeichnung
   * @param quantity    - Menge
   * @param sellPrice   - Preis
   */
  void insertBilling(BillingInfo billing);

  /**
   * @brief Versandkosten und Mehwertsteuer Anzeige
   */
  bool insertSummaryTable();

  /**
   * @brief Gesamtkosten
   */
  void finalizeBillings();

  /**
   * @brief Zusätzliche Texte einfügen
   */
  void setAdditionalInfo();

  /**
   * @brief mfg
   */
  void setRegards();

protected Q_SLOTS:
  bool generateDocument(QPrinter *printer);
  void generatePdf();
  void openPrintDialog();

public:
  explicit PaymentReminder(QWidget *parent = nullptr);

  void setTitleText(const QString &);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor @ref exec() gesetzt sein!
   */
  void setPaymentInfo(int orderId,    /* Bestellnummer */
                      int customerId, /* Kundennummer */
                      int invoiceId,  /* Rechnungsnummer */
                      const QString &deliverNoteId);

  /**
   * @brief Primäre Nachricht an den Kunden
   */
  void setMainText(const QString &);

  /**
   * @brief Abschließender Text
   */
  void setFinalText(const QString &);

  int exec(const QList<BillingInfo> &billing);
};

#endif // PAYMENTREMINDER_PRINTING_H
