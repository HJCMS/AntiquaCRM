// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVOICE_PRINTING_H
#define INVOICE_PRINTING_H

#include <QList>
#include <QMetaType>
#include <QObject>
#include <QWidget>

#ifndef PRINTING_PRINTING_H
#include "printing.h"
#endif

/**
 * @ingroup AntiquaPrinting
 * @brief Printing invoices
 */
class Invoice final : public Printing {
  Q_OBJECT

private:
  /**
   * @brief Artikelliste
   */
  QTextTable *m_billingTable;

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
   * @brief Bereits beszahlt
   */
  bool already_paid = false;

  /**
   * @brief Standard Druckkopfbreich
   */
  void constructSubject() override;

  /**
   * @brief Anschrifft und Dokument Definition
   */
  void constructBody() override;

  /**
   * @brief Versandkosten anzeige
   */
  bool insertDeliveryCost();

  /**
   * @brief Gesamtkosten
   */
  void finalizeBillings();

  /**
   * @brief Zahlungsbedingungen
   */
  void setPaymentTerms();

  /**
   * @brief Zusätzliche Texte einfügen
   */
  void setAdditionalInfo();

private Q_SLOTS:
  /**
   * @brief Drucker ausgabe generieren
   */
  bool generateDocument(QPrinter *printer) override;

  /**
   * @brief PDF in Datei schreiben
   */
  void generatePdf() override;

  /**
   * @brief Öffne Drucker Dialog
   */
  void openPrintDialog() override;

public:
  explicit Invoice(QWidget *parent = nullptr);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor exec() gesetzt sein!
   */
  void setInvoice(qint64 orderId,    /* Bestellnummer */
                  qint64 customerId, /* Kundennummer */
                  qint64 invoiceId,  /* Rechnungsnummer */
                  qreal deliverprice, /* Paketsender Preis */
                  const QString &deliverNoteId);

  int exec() override;
  int exec(const QList<BillingInfo> &, bool paid = false);
};

#endif // INVOICE_PRINTING_H
