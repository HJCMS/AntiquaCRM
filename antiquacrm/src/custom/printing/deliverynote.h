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

#include "printing.h"

class ApplSettings;

/**
 * @brief Lieferschein Erstellung
 * @ingroup Printing
 * @class DeliveryNote
 */
class DeliveryNote : public Printing {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Bestellnummer
   */
  QString p_orderId;

  /**
   * @brief Kundennummer
   */
  QString p_customerId;

  /**
   * @brief Lieferschein Nummer
   */
  QString p_deliveryId;

  /**
   * @brief Kunden Anschrift
   */
  QString p_customerAddress;

  /**
   * @brief Einlesen der Einstellungen
   */
  ApplSettings *config;

  /**
   * @brief Wird von @ref readConfiguration() befüllt.
   */
  QHash<QString, QString> companyData;

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
   * @brief Firmen Einstellungen lesen
   */
  void readConfiguration();

  /**
   * @brief Standard PDF Ausgabe Verzeichnis
   */
  const QString outputDirectory();

  /**
   * @brief Briefkopf erstellen
   */
  void constructHeader();

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
   * @brief Adresse und Finanz Informationen
   */
  void constructFooter();

  /**
   * @brief Artikel in @ref m_articleTable einfügen.
   * @param articleid   - Artikel Nummer
   * @param designation - Bezeichnung
   * @param quantity    - Menge
   */
  void insertArticle(const QString &articleid, const QString &designation,
                     const QString &quantity);

  /**
   * @group HTML Ausgaben für Printer
   * @{
   */
  const QString getHeaderHTML();

  const QString getBodyHTML();

  const QString getFooterHTML();
  /** @} */

  /**
   * @brief Wenn vorhanden ...
   * Wasserzeichen obne Links einfügen.
   */
  const QImage getWatermark();

private Q_SLOTS:
  void printToFile(QPrinter *printer){/* not needed */};

  /**
   * @brief Erstelle Druckdokument mit QPainter
   */
  void printDocument(QPrinter *printer);

  /**
   * @brief Drucker Dialog aufrufen
   */
  void openPrintDialog();

public Q_SLOTS:
  /**
   * @brief Warnung ausgeben wenn ...
   * Wichtige Parameter nicht gesetzt sind!
   */
  int warningMessageBox(const QString &);

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
   * @brief Konvetrierte Lieferschein Nummer
   */
  const QString deliveryNumber();

  /**
   * @brief Add Customer Address
   * @note Muss vor @ref exec() gesetzt sein!
   */
  void setCustomerAddress(const QString &);

  /**
   * @brief Wichtige Nummern angeben
   * @note Muss vor @ref exec() gesetzt sein!
   */
  void setDelivery(int &orderId, int &customerId);

  /**
   * @brief Erstelle Inhalt und öffne Dialog.
   */
  int exec(const QList<Delivery> &);
};
Q_DECLARE_METATYPE(DeliveryNote::Delivery);

#endif // DELIVERYNOTE_PRINTING_H
