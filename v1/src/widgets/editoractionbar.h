// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_EDITORACTIONBAR_H
#define ANTIQUACRM_EDITORACTIONBAR_H

#include <AntiquaInputEdit>
#include <QFlags>
#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @brief Standard Editor Aktionsleiste
 * @class EditorActionBar
 */
class EditorActionBar final : public QWidget {
  Q_OBJECT

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
   * @brief Artikel hinzufügen
   */
  QPushButton *m_addArticle;

  /**
   * @brief Speichern
   */
  QPushButton *m_saveBtn;

  /**
   * @brief Zurück zur Hauptansicht
   */
  QPushButton *m_readyBtn;

Q_SIGNALS:
  void sendRestoreClicked();
  void sendPrintDeliveryNote();
  void sendPrintInvoiceNote();
  void sendPrintPaymentReminder();
  void sendPrintBookCard();
  void sendCancelClicked();
  void sendAddArticle();
  void sendSaveClicked();
  void sendFinishClicked();
  void sendCreateMailMessage(const QString &);

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
  void setMailMenu(MailButton::Sections sections);

  /**
   * @brief Article Einfügen Anzeigen
   */
  void setViewAddArticleButton(bool b = false,
                               const QString &customTitle = QString());

  /**
   * @brief Zurücksetzen anzeigen/ausblenden
   */
  void setViewRestoreButton(bool b = false);

  /**
   * @brief Abfrage auf Zurücksetzen Knopf
   */
  bool isRestoreable();
};

#endif // ANTIQUACRM_EDITORACTIONBAR_H
