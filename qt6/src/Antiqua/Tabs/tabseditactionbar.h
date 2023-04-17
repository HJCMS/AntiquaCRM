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

  /**
   * @brief Set focus to a QPushButton!
   * @param suffix
   * Available Suffixes:
   *  @li cancel
   *  @li restore
   *  @li article
   *  @li save
   *  @li back
   * @code
   *  setFocusButton("article");
   * @endcode
   */
  void setFocusButton(const QString &suffix);

public:
  explicit TabsEditActionBar(QWidget *parent = nullptr);

  /**
   * @brief Drucken anzeigen/ausblenden
   */
  void setViewPrintButton(bool b = false);

  /**
   * @brief Druckerdialog Menü festlegen
   */
  void setPrinterMenu(AntiquaCRM::PrinterGroups);

  /**
   * @brief E-Mail Knopf anzeigen/ausblenden
   */
  void setViewMailButton(bool b = false);

  /**
   * @brief E-Mail Nachrichten Menü festlegen
   */
  void setMailMenu(const QMap<QString,QString> &);

  /**
   * @brief Article Einfügen Anzeigen
   */
  void setViewActionAddButton(bool b = false,
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

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_EDITACTIONBAR_H
