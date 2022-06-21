// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVOICE_PRINTING_H
#define INVOICE_PRINTING_H

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
  QTextTable *m_billingTable;
  QString p_currency;
  int p_quantity_sum = 0;
  double p_fullPrice = 0.00;

  void constructSubject();
  void constructBody();

  /**
   * @brief Artikel in @ref m_articleTable einfügen.
   * @param articleid   - Artikel Nummer
   * @param designation - Bezeichnung
   * @param quantity    - Menge
   * @param sellPrice   - Preis
   */
  void insertBilling(const QString &articleid, const QString &designation,
                     int quantity, double sellPrice);

  void finalizeBillings();

private Q_SLOTS:
  bool generateDocument(QPrinter *printer);
  void openPrintDialog();

public:
  /**
   * @brief Container für das befüllen der Tabellenzellen.
   */
  struct BillingInfo {
    QString articleid;   /**< Article ID */
    QString designation; /**< Designation */
    int quantity;        /**< Quantity */
    double sellPrice;    /**< Preis */
  };

  explicit Invoice(QWidget *parent = nullptr);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor @ref exec() gesetzt sein!
   */
  void setInvoice(int orderId,    /* Bestellnummer */
                  int customerId, /* Kundennummer */
                  int invoiceId,  /* Rechnungsnummer */
                  const QString &deliverNoteId);

  int exec(const QList<BillingInfo> &);
};
Q_DECLARE_METATYPE(Invoice::BillingInfo);

#endif // INVOICE_PRINTING_H
