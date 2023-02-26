// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINT_SETTINGS_H
#define PRINT_SETTINGS_H

#include <QFont>
#include <QGroupBox>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QSignalMapper>
#include <QWidget>
#include <SettingsDialog>

class PrinterFonts;
class PrinterSetup;
class PrintingLayout;

class PrintSettings : public SettingsWidget {
  Q_OBJECT

private:
  /**
   * @brief Schriften
   */
  PrinterFonts *m_printerFonts;

  /**
   * @brief Pfad für Dateianhänge!
   */
  LineEdit *m_attachments;

  /**
   * @brief Wasserzeichen Bild
   */
  LineEdit *m_watermark;

  /**
   * @brief Standard Drucker Setzen
   */
  PrinterSetup *m_printerSetup;

  /**
   * @brief Tabellenrand Abstände
   */
  PrintingLayout *m_printLayout;

  /**
   * @brief QRCode Einstellungen
   */
  QGroupBox *qrcodeGroup;
  LineEdit *qr_server_schema;
  LineEdit *qr_server_address;
  LineEdit *qr_server_path;
  LineEdit *qr_server_query;

  /**
   * @note Wird hier nicht benötigt!
   * @see setLabelFont
   */
  void initSignalChanged(){};

private Q_SLOTS:
  void openDirectoryDialog();
  void openWatermarkDialog();

public:
  explicit PrintSettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // PRINT_SETTINGS_H
