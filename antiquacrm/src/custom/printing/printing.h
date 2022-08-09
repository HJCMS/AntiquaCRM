// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_PRINTING_H
#define PRINTING_PRINTING_H

#include <QComboBox>
#include <QDialog>
#include <QFont>
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
class ApplSettings;

#include <AntiquaCRM>

/**
 * @brief Container für das befüllen der Tabellenzellen.
 * @class BillingInfo
 * @ingroup Printing
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
   * @brief Mehwertsteuerangabe
   * @li true  = "inkl. MwSt"
   * @li false = "+ MwSt"
   */
  bool includeVat;

  /**
   * @brief Es fällt keine Mehwertsteuer an!
   */
  bool disableVat;

  /**
   * @brief Mehwertsteuersatz
   * @note Wird Gesamt behandelt!
   */
  int taxValue;

  /**
   * @brief Versankosten hinzufügen
   */
  qreal packagePrice;
};
Q_DECLARE_METATYPE(BillingInfo);

/**
 * @brief Drucker Dialog Klasse
 * @class Printing
 * @defgroup Printing Printing
 * Das Dokument wird aus drei Textkörpern erstellt.
 * Mit einem Kopfbereich, Inhaltsbereich und Fußzeilenbereich.
 * Anders ist es nicht möglich die Fußzeile sauber zu setzen.
 */
class Printing : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(QFont headerFont READ getHeaderFont WRITE setHeaderFont NOTIFY
                 headerFontChanged)
  Q_PROPERTY(QFont normalFont READ getNormalFont WRITE setNormalFont NOTIFY
                 normalFontChanged)
  Q_PROPERTY(QFont footerFont READ getFooterFont WRITE setFooterFont NOTIFY
                 footerFontChanged)
  Q_PROPERTY(QFont smallFont READ getSmallFont WRITE setSmallFont NOTIFY
                 smallFontChanged)

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
   * @brief Ausgewählter Drucker
   * @note Wenn keiner Ausgwählt wird dann PDF export!
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
  ApplSettings *config;

  /**
   * @brief Währung
   */
  QByteArray p_currency;

  /**
   * @brief Zahlungsfrist
   */
  QString p_grace_period;

  /**
   * @brief Standard Mehrwertsteuersatz
   */
  int p_tax_value;

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
  QPushButton *printButton;
  QPushButton *quitButton;

  /**
   * @brief StatusBar
   */
  QStatusBar *m_statusBar;

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

  /**
   * @brief Tabellenrand Farbe
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
   * @brief Reihe mit Anschrift und Kategorie
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
   * @param vat   - Mehrwertsteuer
   */
  inline qreal addVat(qreal price, qreal vat) const {
    Antiqua::TaxCalculator calc(price);
    return calc.plus_vat(vat);
  };

  inline qreal inclVat(qreal price, qreal vat) const {
    Antiqua::TaxCalculator calc(price);
    return calc.incl_vat(vat);
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
   * @brief Drucker Dialog öffnen
   */
  virtual void openPrintDialog() = 0;

Q_SIGNALS:
  void headerFontChanged();
  void normalFontChanged();
  void footerFontChanged();
  void smallFontChanged();
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
   * @brief Vordefinierte Seitengröße
   */
  const QPageSize pageSize() const;

  /**
   * @brief Drucker Layout
   */
  const QPageLayout pageLayout() const;

  /**
   * @brief PDF File Layout
   */
  const QPageLayout pdfLayout() const;

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

  /**
   * @brief Druckervorschau aktivieren
   */
  void enablePrintPreview(bool b = false);

  static const QByteArray defaultCodec();
};

#endif // PRINTING_PRINTING_H
