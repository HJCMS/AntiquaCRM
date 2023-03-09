// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_PRINTING_H
#define PRINTING_PRINTING_H

#include <AntiquaCRM>
#include <QComboBox>
#include <QDialog>
#include <QFont>
#include <QPen>
#include <QMarginsF>
#include <QObject>
#include <QPageLayout>
#include <QPageSize>
#include <QPrintDialog>
#include <QPrinter>
#include <QPushButton>
#include <QStatusBar>
#include <QTextCharFormat>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QWidget>

class TextEditor;

/**
 * @ingroup utils
 * @brief Container für das befüllen der Tabellenzellen.
 */
struct BillingInfo {
  /**
   * @brief Artikel Nummer
   */
  QString articleid;

  /**
   * @brief Artikel Beschreibung
   */
  QString designation;

  /**
   * @brief Artikel Menge/Anzahl
   */
  int quantity;

  /**
   * @brief Artikel Preis
   */
  double sellPrice;

  /**
   * @brief Es fällt keine Mehwertsteuer an!
   */
  bool vatDisabled;

  /**
   * @brief Mehwertsteuersatz
   * @note Wird Gesamt behandelt!
   */
  int vatValue;

  /**
   * @brief Abrechnungs type
   * @see global.h
   */
  AntiquaCRM::SalesTax vatSet;

  /**
   * @brief Versankosten hinzufügen
   */
  qreal packagePrice;
};
Q_DECLARE_METATYPE(BillingInfo);

/**
 * @ingroup utils
 * @brief Printing Dialogs
 * @class Printing
 * Das Dokument wird aus drei Textkörpern erstellt.
 * Mit einem Kopfbereich, Inhaltsbereich und Fußzeilenbereich.
 * Anders ist es nicht möglich die Fußzeile sauber zu setzen.
 */
class Printing : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Schriften
   */
  QFont headerFont;
  QFont addressFont;
  QFont subjectFont;
  QFont normalFont;
  QFont footerFont;
  QFont smallFont;

protected:
  /**
   * @brief Ausgewählter Drucker
   * @note Wenn keiner Ausgwählt, dann PDF export!
   */
  QString p_printerName = QString();

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
  AntiquaCRM::ASettings *config;

  /**
   * @brief Zahlungsfrist
   */
  QString p_grace_period;

  /**
   * @brief Standard Mehrwertsteuersatz
   */
  int p_tax_value;

  /**
   * @brief Rechter Rand vom Dokument in Pixel!
   */
  QMarginsF p_margins;

  /**
   * @brief Gesamtpreis
   */
  qreal p_totalPrice = 0.00;

  /**
   * @brief Deckkraft Wasserzeichen
   */
  qreal p_watermark_opacity = 1.0;

  /**
   * @brief Versankosten
   */
  qreal p_deliveryCost = 0.00;

  /**
   * @brief Artikel Mehrwertsteuersatz speicher
   * QMap<ArticleID,VAT>
   */
  QMap<QString, int> p_articleCalc;

  /**
   * @brief Wird von @ref readConfiguration() befüllt.
   */
  QHash<QString, QString> companyData;

  /**
   * @brief Text Editor
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
   * @brief Position vom Adressenkopf
   */
  int p_subjectPosition = 120;
  int p_bodyPosition = 15;
  qreal p_recipientTopPadding = 5.0;

  /**
   * @brief Inhalts Editor
   */
  TextEditor *body;

  /**
   * @brief Fußzeilen Editor
   */
  TextEditor *footer;

  /**
   * @brief Drucken/PDF Aktionsknöpfe
   */
  QComboBox *selectPrinter;
  QPushButton *pdfButton;
  QPushButton *printButton;
  QPushButton *quitButton;

  /**
   * @brief StatusBar
   */
  QStatusBar *m_statusBar;

  /**
   * @brief Rahmenrand einstellungen
   */
  enum Border { NoBorder = 0, Top = 1, Bottom = 2 };

  /**
   * @brief Firmen Einstellungen lesen
   */
  void readConfiguration();

  /**
   * @brief Linien für falten und lochen!
   */
  const QPen penStyle() const;

  /**
   * @brief Textformat für den Briefkopf
   */
  const QTextCharFormat charFormat(const QFont &f, bool bolded = false);

  /**
   * @brief Text Block align right
   */
  const QTextBlockFormat alignRight();

  /**
   * @brief Text Block align Center
   */
  const QTextBlockFormat alignCenter();

  /**
   * @brief Tabellen Format
   */
  const QTextTableFormat tableFormat();
  const QTextTableFormat inlineTableFormat();

  /**
   * @brief Tabellenzellen Formate
   */
  const QTextTableCellFormat recipientCellFormat();
  const QTextTableCellFormat cellFormat(Printing::Border);

  /**
   * @brief Tabellenrandfarbe
   */
  const QBrush borderBrush();

  /**
   * @brief Body Rahmen.
   */
  QTextFrame *bodyFrame();

  /**
   * @brief Erstelle Briefkopf
   */
  virtual void constructHeader();

  /**
   * @brief Tabelle Adressenkopf für Rechnung, Erinnerung und Mahnung!
   */
  QTextTable *constructSubjectTable(const QString &subject);

  /**
   * @brief In Tabellenkopf die Felder einfügen
   * @see constructSubjectTable(subject)
   */
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
   * @brief mfg
   */
  void setRegards(const QStringList &);

  /**
   * @brief Standard PDF Ausgabe Verzeichnis
   */
  const QString outputDirectory(const QString &target);

  /**
   * @brief HTML Ausgaben für Printer
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

  /**
   * @brief Einen Artikel zur Tabelle hinzufügen.
   */
  bool addArticleRow(QTextTable *, BillingInfo);

  /**
   * @brief Drucker in @ref selectPrinter einfügen!
   */
  void addPrinters();

  /**
   * @brief PDF Erstellen
   * @param section - Konfigurations Sektion
   */
  bool createPDF(const QString &section);

  /**
   * @brief Mehrwertsteuer berechnen
   * @param price - Basis Preis
   * @param vat   - Umsatzssteuer
   */
  inline qreal addVat(qreal price, qreal vat) const {
    AntiquaCRM::ATaxCalculator calc(price);
    return calc.toAdd(vat);
  };

  inline qreal inclVat(qreal price, qreal vat) const {
    AntiquaCRM::ATaxCalculator calc(price);
    return calc.getIncl(vat);
  };

protected Q_SLOTS:
  /**
   * @brief Ein Drucker wurde ausgewählt.
   */
  void printerSelected(int index);

  /**
   * @brief Ausdruck erstellen
   */
  virtual bool generateDocument(QPrinter *printer) = 0;

  /**
   * @brief Inhalt erstellen
   */
  virtual void generatePdf() = 0;

  /**
   * @brief Drucker Dialog öffnen
   */
  virtual void openPrintDialog() = 0;

Q_SIGNALS:
  void printerChanged(bool);
  void statusMessage(const QString &);

public Q_SLOTS:
  /**
   * @brief Warnung ausgeben wenn ...
   * Wichtige Parameter nicht gesetzt sind!
   */
  int warningMessageBox(const QString &);

public:
  explicit Printing(QWidget *parent = nullptr);

  /**
   * @brief Preis in text format
   */
  const QString money(double) const;

  /**
   * @brief Vordefinierte Seitengröße
   */
  const QPageSize pageSize() const;

  /**
   * @brief Drucker Layout
   */
  const QPageLayout pageLayout() const;

  /**
   * @brief Suche Schrift in der Datenbank
   */
  static bool fontFamilyExists(const QString &family);

  /**
   * @brief Briefkopfschrift
   */
  const QFont getHeaderFont();

  /**
   * @brief Briefkopf-Anschrift
   */
  const QFont getAddressFont();

  /**
   * @brief Standard schrift ändern
   */
  const QFont getNormalFont();

  /**
   * @brief Fußzeilen schrift ändern
   */
  const QFont getFooterFont();

  /**
   * @brief Fußnoten schrift ändern
   */
  const QFont getSmallFont();

  /**
   * @brief Add Customer Address
   * @note Muss vor exec() gesetzt sein!
   */
  void setCustomerAddress(const QString &);

  /**
   * @brief Druckervorschau aktivieren
   */
  void enablePrintPreview(bool b = false);

  static const QByteArray defaultCodec();
};

#endif // PRINTING_PRINTING_H
