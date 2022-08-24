// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef REPORTINGDIALOG_H
#define REPORTINGDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QFileInfo>
#include <QJsonDocument>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>

class DateRangeWidget;
class PreviewTable;
class ApplSettings;

/**
 * @brief Erstellt Berichte für den Datenexport
 * @class ReportingDialog
 */
class ReportingDialog final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *m_cfg;
  QLabel *m_infoLabel;
  QStackedWidget *m_stackedWidget;
  DateRangeWidget *m_datePicker;
  PreviewTable *m_previewTable;
  QDialogButtonBox *m_btnBox;
  QPushButton *btn_prev;
  QPushButton *btn_next;
  QPushButton *btn_apply;
  QPushButton *btn_quit;
  QStatusBar *m_statusBar;

  /**
   * @brief Aktueller Seitenindex
   */
  int pageIndex();

  /**
   * @brief SQL Abfrage
   */
  const QString getSqlQueryString();

  /**
   * @brief Erstelle SQL Abfrage aus der Datumsangabe.
   */
  void setSqlRangeQuery();

  /**
   * @brief Json Daten export erstellen.
   */
  const QJsonDocument getSqlQueryJson();

  /**
   * @brief Daten export erstellen.
   */
  bool saveDataExport();

  /**
   * @brief Speichern unter Dialog
   */
  const QFileInfo getSaveFile();

private Q_SLOTS:
  /**
   * @brief gehe zu Seite
   */
  void setPage(int index = 0);

  /**
   * @brief Eine Seite zurück
   */
  void previousPage();

  /**
   * @brief Eine Seite vor
   */
  void nextPage();

  /**
   * @brief Umsetzung Knopf/Aktivierung
   */
  void pageEntered(int index);

  /**
   * @brief Dialog schließen
   */
  void close();

  /**
   * @brief Aktuelle Einstellungen anwenden
   */
  void apply();

protected:
  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

public:
  explicit ReportingDialog(QWidget *parent = nullptr);
};

#endif // REPORTINGDIALOG_H
