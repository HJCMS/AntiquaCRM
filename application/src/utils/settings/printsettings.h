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

class PrinterSetup;
class PrintingLayout;

class PrintSettings : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  /**
   * @brief Für die Dialogknöpfe
   */
  QSignalMapper *m_signalMapper;

  /**
   * @brief Briefkopf
   */
  QLabel *header_font;

  /**
   * @brief Standard Dokumenten Schrift
   */
  QLabel *body_font;

  /**
   * @brief Fußzeilen Schrifft
   */
  QLabel *footer_font;

  /**
   * @brief Schrifft für nicht Tabellen Dokumente
   */
  QLabel *address_font;

  /**
   * @brief Schrifft für nicht Tabellen Dokumente
   */
  QLabel *normal_font;

  /**
   * @brief Kleine Schriften
   */
  QLabel *small_font;

  /**
   * @brief Wird für QObject::findchild benötigt!
   * Dieser Reguläre Ausdruck wird verwendet um die Eingabe-Objektklassen zu
   * finden.
   */
  const QRegularExpression p_fontPattern = QRegularExpression("^\\w+_font$");

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
   * @brief Setze font für Kategorie und Label
   */
  void setLabelFont(QLabel *);

  /**
   * @brief Setze QPushButton und ...
   * Registriere die Signale für den Mapper.
   * @param objName - Empfänger Objektname
   */
  QPushButton *setFontButton(const QString &objName);

  /**
   * @note Wird hier nicht benötigt!
   * @see setLabelFont
   */
  void initSignalChanged(){};

private Q_SLOTS:
  void openFontDialog(const QString &objName);
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
