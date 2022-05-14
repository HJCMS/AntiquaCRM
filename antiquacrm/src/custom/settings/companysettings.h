// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPANY_SETTINGS_H
#define COMPANY_SETTINGS_H

#include "settingswidget.h"
#include <Utils>

#include <QObject>
#include <QLineEdit>
#include <QWidget>

class CompanySettings : public SettingsWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLineEdit *m_companyName;
  QLineEdit *m_companyShortName;
  QLineEdit *m_companyStreet;
  QLineEdit *m_companyLocation;
  QLineEdit *m_employer;
  QLineEdit *m_tax_number;
  QLineEdit *m_phone;
  QLineEdit *m_fax;
  QLineEdit *m_email;
  TextField *m_remarks;

public:
  CompanySettings(QWidget * parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // COMPANY_SETTINGS_H
