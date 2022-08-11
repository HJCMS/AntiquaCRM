// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITORACTIONBAR_UTILS_H
#define EDITORACTIONBAR_UTILS_H

#include <QFlags>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "mailbutton.h"
#include "printerbutton.h"

/**
 * @brief Standard Editor Aktionsleiste
 * @class EditorActionBar
 */
class EditorActionBar final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Zurücksetzen
   */
  QPushButton *m_restoreBtn;

  /**
   * @brief Verschiedene Druck Dialoge öffnen
   */
  PrinterButton *m_printerButton;

  /**
   * @brief E-Mail Benachrichtigungen
   */
  MailButton *m_mailButton;

  /**
   * @brief Abbrechen
   */
  QPushButton *m_cancelBtn;

  /**
   * @brief Speichern
   */
  QPushButton *m_saveBtn;

  /**
   * @brief Zurück zur Hauptansicht
   */
  QPushButton *m_readyBtn;

Q_SIGNALS:
  void s_restoreClicked();
  void s_printDeliveryNote();
  void s_printInvoiceNote();
  void s_printPaymentReminder();
  void s_printBookCard();
  void s_cancelClicked();
  void s_saveClicked();
  void s_finishClicked();

public Q_SLOTS:
  /**
   * @brief Zurücksetzen aktivieren/deaktivieren
   */
  void setRestoreable(bool);

public:
  explicit EditorActionBar(QWidget *parent = nullptr);

  /**
   * @brief Drucken anzeigen/ausblenden
   */
  void setViewPrintButton(bool b = false);

  /**
   * @brief Druckerdialog Menü festlegen
   */
  void setPrinterMenu(PrinterButton::Buttons buttons);

  /**
   * @brief E-Mail Knopf anzeigen/ausblenden
   */
  void setViewMailButton(bool b = false);

  /**
   * @brief E-Mail Nachrichten Menü festlegen
   */
  void setMailMenu(/* TODO */);

  /**
   * @brief Zurücksetzen anzeigen/ausblenden
   */
  void setViewRestoreButton(bool b = false);

  /**
   * @brief Abfrage auf Zurücksetzen Knopf
   */
  bool isRestoreable();
};

#endif // EDITORACTIONBAR_UTILS_H
