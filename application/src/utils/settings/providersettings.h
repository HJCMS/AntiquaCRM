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

/**
 * @brief The ProviderSettings class
 * @ingroup AntiquaSettings
 */
class ProviderSettings : public SettingsWidget {
  Q_OBJECT

private:
  /**
   * @brief Buchfreund
   */
  QGroupBox *m_buchfreund;
  LineEdit *m_buchfreund_api_url;
  LineEdit *m_buchfreund_api_key;

  /**
   * @brief AbeBooks
   */
  QGroupBox *m_abebooks;
  LineEdit *m_abebooks_api_user;
  LineEdit *m_abebooks_api_url;
  LineEdit *m_abebooks_api_key;
  LineEdit *m_abebooks_api_port;

  /**
   * @brief Booklooker
   */
  QGroupBox *m_booklooker;
  LineEdit *m_booklooker_api_url;
  LineEdit *m_booklooker_api_key;

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
