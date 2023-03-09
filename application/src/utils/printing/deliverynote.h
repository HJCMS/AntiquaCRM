// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERYNOTE_PRINTING_H
#define DELIVERYNOTE_PRINTING_H

#include <QHash>
#include <QImage>
#include <QMetaType>
#include <QObject>
#include <QWidget>

#ifndef PRINTING_PRINTING_H
#include "printing.h"
#endif

/**
 * @ingroup _printing
 * @brief Printing delivery notes
 */
class DeliveryNote final : public Printing {
  Q_OBJECT

private:
  /**
   * @brief Hier werden die Bestellartikel eingefügt.
   */
  QTextTable *m_articleTable;

  /**
   * @brief Optionaler TextBlock für Bemerkungen
   *  unterhalb der Bestellartikel.
   */
  QTextTableCell p_infoCell;

  /**
   * @brief Anschrift und Lieferschein
   * Spalte zwischen Briefkopf und Artikelliste
   * Hier werden Kundenanschrift und Lieferschein
   * Informationen eingefügt.
   */
  void constructSubject() override;

  /**
   * @brief Artikelliste
   */
  void constructBody() override;

  /**
   * @brief Artikel in @ref m_articleTable einfügen.
   * @param articleid   - Artikel Nummer
   * @param designation - Bezeichnung
   * @param quantity    - Menge
   */
  void insertArticle(const QString &articleid, const QString &designation,
                     const QString &quantity);

private Q_SLOTS:
  bool generateDocument(QPrinter *printer) override;
  void generatePdf() override;
  void openPrintDialog() override;

public:
  /**
   * @brief Container für das befüllen der Tabellenzellen.
   */
  explicit DeliveryNote(QWidget *parent = nullptr);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor exec() gesetzt sein!
   */
  void setDelivery(qint64 orderId, qint64 customerId,
                   const QString &deliverNoteId);

  /**
   * @brief Erstelle Inhalt und öffne Dialog.
   */
  int exec() override;
  int exec(const QList<BillingInfo> &);
};

#endif // DELIVERYNOTE_PRINTING_H
