// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef REPORTS_PRINTING_H
#define REPORTS_PRINTING_H

#include <ASettings>
#include <QDate>
#include <QDialog>
#include <QList>
#include <QMetaType>
#include <QObject>
#include <QPrinter>
#include <QStatusBar>
#include <QTextTable>
#include <QWidget>

class TextEditor;

class Reports final : public QDialog {
  Q_OBJECT

private:
  QPageLayout p_pageLayout;

  AntiquaCRM::ASettings *config;

  TextEditor *m_edit;

  QStatusBar *m_statusBar;

  /**
   * @brief Tabellenansicht
   */
  QTextTable *m_table;

  /**
   * @brief Tabellen Format
   */
  const QTextTableFormat tableFormat();

  /**
   * @brief Tabellen Spalten
   */
  const QTextTableCellFormat cellFormat();

  /**
   * @brief Datenkopf format
   */
  const QTextCharFormat headerFormat();

  /**
   * @brief Standard Textformat
   */
  const QTextCharFormat textFormat(qint8 size = 0);

  /**
   * @brief Firma, Titel, Datum Monatsbericht
   */
  QString p_title;

  /**
   * @brief Kopfzellen
   */
  QStringList p_header;

  /**
   * @brief Dateneingabe
   */
  QStringList p_data;

  /**
   * @brief Standard Druckkopfbreich
   */
  void constructSubject();

  /**
   * @brief Anschrifft und Dokument Definition
   */
  void constructBody();

private Q_SLOTS:
  /**
   * @brief Drucker ausgabe generieren
   */
  bool generateDocument(QPrinter *printer);

  /**
   * @brief Öffne Drucker Dialog
   */
  void openPrintDialog();

Q_SIGNALS:
  void statusMessage(const QString &);

public:
  explicit Reports(QWidget *parent = nullptr);

  void setHeaderInfo(const QString &title, const QString &header);

  int exec() override;
  int exec(const QStringList &rows);
};

#endif // REPORTS_PRINTING_H
