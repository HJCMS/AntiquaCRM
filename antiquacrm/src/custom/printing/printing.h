// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_PRINTING_H
#define PRINTING_PRINTING_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFont>
#include <QMarginsF>
#include <QObject>
#include <QPageLayout>
#include <QPageSize>
#include <QPrinter>
#include <QPushButton>
#include <QTextCharFormat>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QWidget>

class TextEditor;
class ApplSettings;

/**
 * @brief Drucker Dialog Klasse
 * @class Printing
 * @group Printing
 * Das Dokument wird aus drei Textkörpern erstellt.
 * Mit einem Kopfbereich, Inhaltsbereich und Fußzeilenbereich.
 * Anders ist es nicht möglich die Fußzeile sauber zu setzen.
 */
class Printing : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(QFont headerFont READ getHeaderFont WRITE setHeaderFont NOTIFY headerFontChanged)
  Q_PROPERTY(QFont normalFont READ getNormalFont WRITE setNormalFont NOTIFY normalFontChanged)
  Q_PROPERTY(QFont footerFont READ getFooterFont WRITE setFooterFont NOTIFY footerFontChanged)
  Q_PROPERTY(QFont smallFont READ getSmallFont WRITE setSmallFont NOTIFY smallFontChanged)

private:
  /**
   * @brief Schriften
   */
  QFont headerFont;
  QFont normalFont;
  QFont footerFont;
  QFont smallFont;

protected:
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
   * @brief Rechnungs Nummer
   */
  QString p_invoiceId;

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
   * @brief Seitenränder der Druckerausgabe
   */
  QMarginsF page_margins;

  /**
   * @brief Seitengröße
   * @note Standard ist DIN A4
   */
  QPageSize page_size;

  /**
   * @brief Text Editor
   * @group printArea
   * @{
   * Das Widget @ref printArea enthält
   * das Layout für die Text Editoren.
   * @b Besonderheiten:
   *  @li Es besitzt eine feste Größe.
   *  @li Die größe ist auf den Druckerbreich eingeschränkt.
   *  @li Es hat keine Ränder css{border:none;}
   *  @li Das Dimensions verhalten der Editoren ist mit
   *      setStretchFactor() in Prozent eingeschränkt.
   *      Aktuelle angaben: Header(30%) Body(40%) Footer(20%)
   *  @li Die Editoren "header" und "footer" besitzen keinen Scrollbalken.
   */
  QWidget *printArea;

  /**
   * @brief Kopzeilen Editor
   */
  TextEditor *header;

  /**
   * @brief Inhalts Editor
   */
  TextEditor *body;

  /**
   * @brief Fußzeilen Editor
   */
  TextEditor *footer;
  /** @} */

  /**
   * @brief Dialog Knopfleiste
   */
  QDialogButtonBox *buttonBox;

  /**
   * @brief Drucken/PDF Aktionsknöpfe
   */
  QPushButton *printButton;

  /**
   * @brief Firmen Einstellungen lesen
   */
  void readConfiguration();

  /**
   * @brief Textformat für den Briefkopf
   */
  const QTextCharFormat headerFormat();

  /**
   * @brief Standard Textformat
   */
  const QTextCharFormat normalFormat();

  /**
   * @brief Textformat für die Fußnote
   */
  const QTextCharFormat footerFormat();

  /**
   * @brief Textformat für die Kleine Schrift
   */
  const QTextCharFormat smallFormat();

  /**
   * @brief Tabellen Format
   */
  const QTextTableFormat tableFormat();

  /**
   * @brief Erstelle Briefkopf
   */
  virtual void constructHeader();

  virtual void constructSubject() = 0;

  /**
   * @brief Inhalt erstellen
   */
  virtual void constructBody() = 0;

  /**
   * @brief Fußnote erstellen
   */
  virtual void constructFooter();

  /**
   * @brief Standard PDF Ausgabe Verzeichnis
   */
  const QString outputDirectory(const QString &target = QString("deliverynotes"));

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
   * Wasserzeichen oben Links einfügen.
   */
  const QImage getWatermark();

protected Q_SLOTS:
  /**
   * @brief In Datei Speichern
   */
  virtual void printToFile(QPrinter *printer) = 0;

  /**
   * @brief Ausdruck erstellen
   */
  virtual void printDocument(QPrinter *printer) = 0;

  /**
   * @brief Drucker Dialog öffnen
   */
  virtual void openPrintDialog() = 0;

Q_SIGNALS:
  void headerFontChanged();
  void normalFontChanged();
  void footerFontChanged();
  void smallFontChanged();

public Q_SLOTS:
  /**
   * @brief Warnung ausgeben wenn ...
   * Wichtige Parameter nicht gesetzt sind!
   */
  int warningMessageBox(const QString &);

public:
  explicit Printing(QWidget *parent = nullptr);

  /**
   * @brief Vordefinierte Seitengröße
   */
  const QPageSize pageSize();

  /**
   * @brief Drucker Layout
   */
  const QPageLayout pageLayout();

  /**
   * @brief Suche Schrift in der Datenbank
   */
  static bool fontFamilyExists(const QString &family);

  /**
   * @brief Briefkopf schrift ändern
   */
  void setHeaderFont(const QFont &font);
  const QFont getHeaderFont();

  /**
   * @brief Standard schrift ändern
   */
  void setNormalFont(const QFont &font);
  const QFont getNormalFont();

  /**
   * @brief Fußzeilen schrift ändern
   */
  void setFooterFont(const QFont &font);
  const QFont getFooterFont();

  /**
   * @brief Fußnoten schrift ändern
   */
  void setSmallFont(const QFont &font);
  const QFont getSmallFont();

  /**
   * @brief Add Customer Address
   * @note Muss vor @ref exec() gesetzt sein!
   */
  void setCustomerAddress(const QString &);

  static const QByteArray defaultCodec();
};

#endif // PRINTING_PRINTING_H
