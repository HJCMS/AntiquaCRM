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
 * @brief Lieferschein Erstellung
 * @ingroup Printing
 * @class DeliveryNote
 */
class DeliveryNote : public Printing {
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
  void constructSubject();

  /**
   * @brief Artikelliste
   */
  void constructBody();

  /**
   * @brief Artikel in @ref m_articleTable einfügen.
   * @param articleid   - Artikel Nummer
   * @param designation - Bezeichnung
   * @param quantity    - Menge
   */
  void insertArticle(const QString &articleid, const QString &designation,
                     const QString &quantity);

private Q_SLOTS:
  bool generateDocument(QPrinter *printer);
  void generatePdf();
  void openPrintDialog();

public:
  /**
   * @brief Container für das befüllen der Tabellenzellen.
   */
  struct Delivery {
    QString articleid;   /**< Article ID */
    QString designation; /**< Designation */
    QString quantity;    /**< Quantity */
  };
  explicit DeliveryNote(QWidget *parent = nullptr);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor @ref exec() gesetzt sein!
   */
  void setDelivery(int orderId, int customerId, const QString &deliverNoteId);

  /**
   * @brief Erstelle Inhalt und öffne Dialog.
   */
  int exec(const QList<Delivery> &);
};
Q_DECLARE_METATYPE(DeliveryNote::Delivery);

#endif // DELIVERYNOTE_PRINTING_H
