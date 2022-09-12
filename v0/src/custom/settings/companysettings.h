// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPANY_SETTINGS_H
#define COMPANY_SETTINGS_H

#include "settingswidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

/**
 * @brief The CompanySettings class
 * @class CompanySettings
 * Hier werden all Unternehmens Einstellungen eingefügt!
 */
class CompanySettings : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLineEdit *m_companyName;
  QLineEdit *m_companyShortName;
  QLineEdit *m_companyStreet;
  QLineEdit *m_companyLocation;
  QLineEdit *m_employer;
  QLineEdit *m_phone;
  QLineEdit *m_fax;
  QLineEdit *m_email;
  QLineEdit *m_bankname;
  QLineEdit *m_bicswift;
  QLineEdit *m_iban;
  QLineEdit *m_tax_number;
  QLineEdit *m_legality;
  QTextEdit *m_signature;

  /**
   * @brief GridLayout Info Labels
   */
  QLabel *infoCell(const QString &);

  /**
   * @note Ist an dieser Stelle nicht Notwendig
   * @see loadSectionConfig
   */
  void initSignalChanged(){};

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
