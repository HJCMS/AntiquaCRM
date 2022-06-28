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
 * @brief Container für das befüllen der Tabellenzellen.
 */
struct BillingInfo {
  QString articleid;   /**< Article ID */
  QString designation; /**< Designation */
  int quantity;        /**< Quantity */
  double sellPrice;    /**< Preis */
  bool includeVat;     /**< inkl. MwSt */
  int taxValue;        /**< Steuerwert */
  qreal packagePrice;  /**< Paket Extras */
};
Q_DECLARE_METATYPE(BillingInfo);

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
  int p_quantity_sum = 0;
  bool p_including_VAT;
  qreal p_fullPrice = 0.00;
  qreal p_packagePrice = 0.00;

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
  bool insertSummaryTable();
  void finalizeBillings();
  void setComment(const QString &);

private Q_SLOTS:
  bool generateDocument(QPrinter *printer);
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

  int exec(const QList<BillingInfo> &, const QString &comment = QString());
};

#endif // INVOICE_PRINTING_H
