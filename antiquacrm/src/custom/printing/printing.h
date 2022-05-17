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
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>
#include <QWidget>

class TextEditor;

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
  Q_PROPERTY(QFont smallFont READ getSmallFont WRITE setSmallFont NOTIFY smallFontChanged)

protected:
  /**
   * @brief Schriften
   */
  QFont headerFont;
  QFont normalFont;
  QFont smallFont;

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
  const QTextCharFormat smallFormat();

  /**
   * @brief Tabellen Format
   */
  const QTextTableFormat tableFormat();

  /**
   * @brief Erstelle Briefkopf
   */
  virtual void constructHeader() = 0;

  /**
   * @brief Inhalt erstellen
   */
  virtual void constructBody() = 0;

  /**
   * @brief Fußnote erstellen
   */
  virtual void constructFooter() = 0;

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
  void smallFontChanged();

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
   * @brief Fußnoten schrift ändern
   */
  void setSmallFont(const QFont &font);
  const QFont getSmallFont();
};

#endif // PRINTING_PRINTING_H
