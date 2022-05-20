// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_CONFIGDIALOG_H
#define ANTIQUA_CONFIGDIALOG_H

#include <SettingsDialog>

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>

class ConfigDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QListWidget *m_listWidget;
  GeneralSettings *m_page1;
  PgSQLSettings *m_page2;
  ViewSettings *m_page3;
  CompanySettings *m_page4;
  PrintSettings *m_page5;
  ProviderSettings *m_page6;
  QStatusBar *m_statusbar;

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
