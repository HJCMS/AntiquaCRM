// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PROVIDER_SETTINGS_H
#define PROVIDER_SETTINGS_H

#include <QGroupBox>
#include <QObject>
#include <QRegularExpression>
#include <QSpinBox>
#include <SettingsDialog>

class ProviderSettings : public SettingsWidget {
  Q_OBJECT

private:
  /**
   * @brief W+H Ssoft
   */
  QGroupBox *m_whsoft;
  LineEdit *m_whsoft_scheme;
  QSpinBox *m_whsoft_history;
  LineEdit *m_whsoft_api_host;
  LineEdit *m_whsoft_api_key;
  LineEdit *m_whsoft_api_basepath;

  /**
   * @brief AbeBooks
   */
  QGroupBox *m_abebooks;
  LineEdit *m_abebooks_user;
  LineEdit *m_abebooks_password;
  LineEdit *m_abebooks_scheme;
  LineEdit *m_abebooks_api_host;
  LineEdit *m_abebooks_api_key;
  LineEdit *m_abebooks_api_port;

  /**
   * @brief Booklooker
   */
  QGroupBox *m_booklooker;
  LineEdit *m_booklooker_user;
  LineEdit *m_booklooker_scheme;
  QSpinBox *m_booklooker_history;
  LineEdit *m_booklooker_api_host;
  LineEdit *m_booklooker_api_key;
  LineEdit *m_booklooker_api_port;

  /**
   * @brief findChildren Options
   */
  const QString p_fs = QString();
  Qt::FindChildOptions p_fo = Qt::FindDirectChildrenOnly;

  void initSignalChanged();

  void setPassword(LineEdit *e, const QByteArray &data);
  const QByteArray passwordToBase64(LineEdit *e);

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
