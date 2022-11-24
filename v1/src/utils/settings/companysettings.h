// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPANY_SETTINGS_H
#define COMPANY_SETTINGS_H

#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include <SettingsDialog>

/**
 * @brief The CompanySettings class
 * @class CompanySettings
 * Hier werden all Unternehmens Einstellungen eingefügt!
 */
class CompanySettings : public SettingsWidget {
  Q_OBJECT

private:
  int layoutRow = 0;
  QGridLayout *m_layout;

  QLabel *infoCell(const QString &);

  void initSignalChanged();

private Q_SLOTS:
  /**
   * @brief Weiterleitung an @ref chieldModified(bool)
   */
  void lineEditChanged(const QString &);

public:
  CompanySettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // COMPANY_SETTINGS_H
