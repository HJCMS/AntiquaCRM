// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_PRINTING_H
#define PRINTING_PRINTING_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFont>
#include <QObject>
#include <QPageLayout>
#include <QPageSize>
#include <QPushButton>
#include <QSize>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QWidget>

class TextEditor;

/**
 * @brief Abstract Printing class
 * @class Printing
 * @group Printing
 * Drucker Dialog Klasse
 */
class Printing : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(QFont headerFont READ getHeaderFont WRITE setHeaderFont NOTIFY
                 headerFontChanged)
  Q_PROPERTY(QFont normalFont READ getNormalFont WRITE setNormalFont NOTIFY
                 normalFontChanged)
  Q_PROPERTY(QFont smallFont READ getSmallFont WRITE setSmallFont NOTIFY
                 smallFontChanged)

protected:
  int resolution = 300;

  /**
   * @brief Briefkopf schrift
   */
  QFont headerFont;

  /**
   * @brief Standard schrift
   */
  QFont normalFont;

  /**
   * @brief Fußnoten schrift
   */
  QFont smallFont;

  /**
   * @brief Seitengröße
   */
  QPageSize page_size;

  /**
   * @brief Seiten Layout DIN A4
   */
  QPageLayout page_layout;

  /**
   * @brief Text Editor
   */
  TextEditor *editor;

  /**
   * @brief Dialog Knopfleiste
   */
  QDialogButtonBox *buttonBox;

  /**
   * @brief drucken Knopf
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
   * @brief Adressen Kopf Table
   */
  QTextTable *m_addressTable;

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
