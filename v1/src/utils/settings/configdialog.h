// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_CONFIGDIALOG_H
#define ANTIQUA_CONFIGDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>
#include <SettingsDialog>

class GeneralSettings;
class PgSQLSettings;
class CompanySettings;
class PrintSettings;
class ProviderSettings;

class ConfigDialog : public QDialog {
  Q_OBJECT

private:
  QListWidget *m_listWidget;
  GeneralSettings *m_pageGeneral;
  PgSQLSettings *m_pageDatabase;
  CompanySettings *m_pageCompany;
  PrintSettings *m_pagePrinting;
  ProviderSettings *m_pageProviders;
  QStatusBar *m_statusbar;

  static const QIcon getIcon(const QString &name);

private Q_SLOTS:
  void setPage(QListWidgetItem *);

public Q_SLOTS:
  void statusMessage(const QString &);
  void saveConfig();
  void closeDialog();

public:
  /**
   * @brief Configuration Pages
   */
  QStackedWidget *pages;

  explicit ConfigDialog(QWidget *parent = nullptr);

  /**
   * @brief Lädt die Einstellungen und erstellt das Menü!
   */
  int exec();
};

#endif // ANTIQUA_CONFIGDIALOG_H
