// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_CONFIGDIALOG_H
#define ANTIQUA_CONFIGDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QIcon>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>
#include <SettingsDialog>

class GeneralSettings;
class PgSQLSettings;
class PrintSettings;
class ProviderSettings;
class PathSettings;

/**
 * @ingroup utils
 * @brief AntqiuaCRM Settings
 * @class ConfigDialog
 * @brief The ConfigDialog class
 */
class ConfigDialog final : public QDialog {
  Q_OBJECT

private:
  GeneralSettings *m_pageGeneral;
  PathSettings *m_pagePaths;
  PgSQLSettings *m_pageDatabase;
  PrintSettings *m_pagePrinting;
  ProviderSettings *m_pageProviders;

  QListWidget *m_listWidget;
  QPushButton *btn_save;
  QPushButton *btn_close;
  QStatusBar *m_statusbar;

  static const QIcon getIcon(const QString &name);
  static const QString getConfigPath(const QString &section);

  void closeEvent(QCloseEvent *) override;

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
  int exec() override;
};

#endif // ANTIQUA_CONFIGDIALOG_H
