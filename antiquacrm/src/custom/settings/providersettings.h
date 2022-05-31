// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PROVIDER_SETTINGS_H
#define PROVIDER_SETTINGS_H

#include "settingswidget.h"
#include <QObject>

class ProviderSettings : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QGroupBox *m_whsoft;
  LineEdit *m_whsoft_scheme;
  LineEdit *m_whsoft_api_host;
  LineEdit *m_whsoft_api_key;
  LineEdit *m_whsoft_api_basepath;

  void initSignalChanged();

public:
  ProviderSettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // PROVIDER_SETTINGS_H
