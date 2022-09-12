// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVOICE_PRINTING_H
#define INVOICE_PRINTING_H

#include <QList>
#include <QMetaType>
#include <QObject>
#include <QWidget>

#include "printing.h"

/**
 * @brief Rechnungs Erstellung
 * @ingroup Printing
 * @class Invoice
 */
class Invoice : public Printing {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

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
   * @brief Gesamtpreis
   */
  qreal p_fullPrice = 0.00;

  /**
   * @brief Versankosten
   */
  qreal p_packagePrice = 0.00;

  /**
   * @brief Standard Druckkopfbreich
   */
  void constructSubject();

  /**
   * @brief Anschrifft und Dokument Definition
   */
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
   * @brief Zahlungsbedingungen
   */
  void setPaymentTerms();

  /**
   * @brief Zusätzliche Texte einfügen
   */
  void setAdditionalInfo();

  /**
   * @brief mfg
   */
  void setRegards();

private Q_SLOTS:
  /**
   * @brief Drucker ausgabe generieren
   */
  bool generateDocument(QPrinter *printer);

  /**
   * @brief PDF in Datei schreiben
   */
  void generatePdf();

  /**
   * @brief Öffne Drucker Dialog
   */
  void openPrintDialog();

public:
  explicit Invoice(QWidget *parent = nullptr);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor @ref exec() gesetzt sein!
   */
  void setInvoice(int orderId,    /* Bestellnummer */
                  int customerId, /* Kundennummer */
                  int invoiceId,  /* Rechnungsnummer */
                  const QString &deliverNoteId);

  int exec(const QList<BillingInfo> &, bool paid = false);
};

#endif // INVOICE_PRINTING_H
