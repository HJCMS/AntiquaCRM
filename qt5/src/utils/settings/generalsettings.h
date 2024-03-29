// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

#include <QObject>
#include <QPushButton>
#include <QToolButton>
#include <SettingsDialog>

/**
 * @brief The General Settings widget
 * @ingroup _settings
 */
class GeneralSettings final : public SettingsWidget {
  Q_OBJECT

private:
  LineEdit *m_mailler_path;       /**< E-Mail Programm */
  Ecurrency *m_currency;          /**< Währung */
  IntSpinBox *m_vat1;             /**< Mehrwertsteuer Normal */
  IntSpinBox *m_vat2;             /**< Mehrwertsteuer Reduziert */
  IntSpinBox *m_searchStart;      /**< Start Zeichen suche */
  IntSpinBox *m_transfer_timeout; /**< Netzwerk Zeitüberschreitung */
  IntSpinBox *m_popup_timeout;    /**< Popup Fenster timeout */
  BoolBox *m_mouseWheel;          /**< Mausrad Unterstützung */
  IntSpinBox *m_queryLimit;       /**< Max. Anzeige in Tabellen */
  IntSpinBox *m_queryAutoUpdate;  /**< Auto Aktualisieren erst ab ... */
  PriceEdit *m_minPrice;         /**< Kleinster Verkaufspreis */
  PriceEdit *m_minPriceMedia;    /**< Kleinster Verkaufspreis Medien */
  PriceEdit *m_defaultCDPrice;   /**< Standard CD Verkaufspreis */
  IntSpinBox *m_payWaitDays;      /**< Zahlungsfrist */
  SizeEdit *m_minSize;            /**< Minimale Bildgröße */
  SizeEdit *m_maxSize;            /**< Maximale Bildgröße */
  QLabel *view_font_config;       /**< Schriftdarstellung */

  QToolButton *btn_mailler;
  QPushButton *btn_fontdialog;

  void initSignalChanged();

private Q_SLOTS:
  void openFontDialog();
  void setMailApplication();

public:
  explicit GeneralSettings(QWidget *parent = nullptr);
  /**
   * @brief PageTitle
   */
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // GENERAL_SETTINGS_H
