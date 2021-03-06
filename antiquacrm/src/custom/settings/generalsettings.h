// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

#include "settingswidget.h"

#include <QObject>
#include <QPushButton>
#include <QToolButton>

class GeneralSettings final : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  LineEdit *m_img_archiv_path; /**< Bilderarchiv */
  LineEdit *m_delivery_path;   /**< Lieferscheine */
  LineEdit *m_invoice_path;    /**< Rechnungen */
  LineEdit *m_cards_path;      /**< Buchkarten */
  Ecurrency *m_currency;       /**< Währung */
  IntSpinBox *m_vat1;          /**< Mehrwertsteuer Normal */
  IntSpinBox *m_vat2;          /**< Mehrwertsteuer Reduziert */
  IntSpinBox *m_searchStart;   /**< Start Zeichen suche */
  IntSpinBox *m_minPrice;      /**< Kleinster Verkaufspreis */
  IntSpinBox *m_payWaitDays;   /**< Zahlungsfrist */
  SizeEdit *m_minSize;         /**< Minimale Bildgröße */
  SizeEdit *m_maxSize;         /**< Maximale Bildgröße */
  QLabel *view_font_config;    /**< Schriftdarstellung */

  QToolButton *btn_imgsrc;
  QToolButton *btn_delivery;
  QToolButton *btn_invoice;
  QToolButton *btn_cards;
  QPushButton *btn_fontdialog;

  void initSignalChanged();

private Q_SLOTS:
  void openFontDialog();
  void setImageArchiv();
  void setDeliveryArchiv();
  void setInvoiceArchiv();
  void setCardsArchiv();

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
