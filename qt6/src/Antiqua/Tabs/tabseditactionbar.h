// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_EDITACTIONBAR_H
#define ANTIQUACRM_TABS_EDITACTIONBAR_H

#include <AGlobal>
#include <AntiquaButtons>
#include <QObject>
#include <QPushButton>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TabsEditActionBar
 * @brief Tab Editor Actionsbar
 *
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsEditActionBar final : public QWidget {
  Q_OBJECT

private:
  /**
   * @brief Zurücksetzen
   */
  QPushButton *m_restoreBtn;

  /**
   * @brief Verschiedene Druck Dialoge öffnen
   * @todo __TODO__ PrinterButton
   */
  PrinterButton *m_printerButton;

  /**
   * @brief E-Mail Benachrichtigungen
   * @todo __TODO__ MailButton
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
  void sendPrintRefunding();
  void sendPrintAdmonition();
  void sendCancelClicked();
  void sendAddCustomAction();
  void sendSaveClicked();
  void sendFinishClicked();
  void sendCreateMailMessage(const QString &);

public Q_SLOTS:
  /**
   * @brief Zurücksetzen aktivieren/deaktivieren
   */
  void setRestoreable(bool);

public:
  /**
   * @param parent - parent object
   */
  explicit TabsEditActionBar(QWidget *parent = nullptr);

  /**
   * @brief Drucken anzeigen/ausblenden
   * @param view  - set visible and enabled
   */
  void setViewPrintButton(bool view = false);

  /**
   * @brief Druckerdialog Menü festlegen
   */
  void setPrinterMenu(AntiquaCRM::PrinterGroups);

  /**
   * @brief E-Mail Knopf anzeigen/ausblenden
   * @param view  - set visible and enabled
   */
  void setViewMailButton(bool view = false);

  /**
   * @brief E-Mail Nachrichten Menü festlegen
   */
  void setMailMenu(AntiquaCRM::MailGroups group = MailGroup::NO_MAIL_GROUP);

  /**
   * @brief Article Einfügen Anzeigen
   * @param view  - set visible and enabled
   * @param title - custom title
   */
  void setViewActionAddButton(bool view = false,
                              const QString &title = QString());

  /**
   * @brief Zurücksetzen anzeigen/ausblenden
   * @param view  - set visible and enabled
   */
  void setViewRestoreButton(bool view = false);

  /**
   * @brief Abfrage auf Zurücksetzen Knopf
   */
  bool isRestoreable();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_EDITACTIONBAR_H
