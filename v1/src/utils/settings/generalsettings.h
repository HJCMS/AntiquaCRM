// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

#include <QObject>
#include <QPushButton>
#include <QToolButton>
#include <SettingsDialog>

class GeneralSettings final : public SettingsWidget {
  Q_OBJECT

private:
  LineEdit *m_img_archiv_path; /**< Bilderarchiv */
  LineEdit *m_delivery_path;   /**< Lieferscheine */
  LineEdit *m_invoice_path;    /**< Rechnungen */
  LineEdit *m_reports_path;    /**< Berichte */
  LineEdit *m_reminder_path;   /**< Erinnerungen */
  LineEdit *m_cards_path;      /**< Buchkarten */
  LineEdit *m_mailler_path;    /**< E-Mail Programm */
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
  QToolButton *btn_reports;
  QToolButton *btn_reminder;
  QToolButton *btn_cards;
  QToolButton *btn_mailler;
  QPushButton *btn_fontdialog;

  void initSignalChanged();

private Q_SLOTS:
  void openFontDialog();
  void setImageArchiv();
  void setDeliveryArchiv();
  void setInvoiceArchiv();
  void setReportsArchiv();
  void setReminderArchiv();
  void setCardsArchiv();
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
