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

#ifndef PRINTING_PRINTING_H
#include "printing.h"
#endif

/**
 * @ingroup utils
 * @brief Printing Payment reminders
 */
class PaymentReminder final : public Printing {
  Q_OBJECT

private:
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
  void constructSubject() override;
  void constructBody() override;

  /**
   * @brief Versandkosten
   */
  bool insertDeliveryCost();

  /**
   * @brief Gesamtkosten
   */
  void finalizeBillings();

  /**
   * @brief Zusätzliche Texte einfügen
   */
  void setAdditionalInfo();

protected Q_SLOTS:
  bool generateDocument(QPrinter *printer) override;
  void generatePdf() override;
  void openPrintDialog() override;

public:
  explicit PaymentReminder(QWidget *parent = nullptr);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor exec() gesetzt sein!
   */
  void setPaymentInfo(qint64 orderId,     /* Bestellnummer */
                      qint64 customerId,  /* Kundennummer */
                      qint64 invoiceId,   /* Rechnungsnummer */
                      qreal deliverprice, /* Paketsender Preis */
                      const QString &deliverNoteId);

  /**
   * @brief Primäre Nachricht an den Kunden
   */
  void setMainText(const QString &);

  /**
   * @brief Abschließender Text
   */
  void setFinalText(const QString &);

  int exec() override;
  int exec(const QList<BillingInfo> &);
};

#endif // PAYMENTREMINDER_PRINTING_H
