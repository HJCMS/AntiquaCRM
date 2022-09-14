// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MAIN_WINDOW_H
#define ANTIQUACRM_MAIN_WINDOW_H

#include <QAction>
#include <QCloseEvent>
#include <QDockWidget>
#include <QEvent>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QSignalMapper>
#include <QToolBar>

#include <AntiquaCRM>

class Workspace;
class StatusBar;
class AntiquaSystemTray;

class MWindow : public QMainWindow {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  /**
   * @brief Standard Menüs und Aktionen
   */
  QMenuBar *m_menuBar;
  QMenu *m_applicationMenu;
  QAction *m_quitAction;
  QAction *m_toggleFullScreen;

  /**
   * @brief Konfigurationen
   */
  ApplSettings *m_Settings;

  /**
   * @brief Arbeitsbereich
   */
  Workspace *m_workSpace;

  /**
   * @brief Signal Mapper für Registerkartenmenü
   */
  QSignalMapper *m_signalMapper;

  /**
   * @brief Statusleiste
   */
  StatusBar *m_statusBar;

  /**
   * @brief Benachrichtigungsfeld in der Statusleiste des Betriebssystem.
   */
  AntiquaSystemTray *m_systemTray;

  /**
   * @brief Standard Menüeinträge erstellen
   */
  void setupActions();

  /**
   * @brief Registerkarten Menü
   * Erstelt Menüeinträge für Registerkarten die geschloseen werden können und
   * beim Start nicht geladen werden.
   */
  void setupTabMenu(QMenu *parent);

private Q_SLOTS:
  /**
   * @brief Interne Programm beenden Aktion verarbeiten.
   * @note Wenn alles ok ist sendet es @ref sendShutdown().
   */
  void quitApplication();

  /**
   * @brief Dialog StorageLocation
   * @section Ui::Dialogs
   */
  void openStorageLocation(bool);

  /**
   * @brief DeliverServiceDialog
   * @section Ui::Dialogs
   */
  void openDeliveryService(bool);

  /**
   * @brief CompleterDialog::DESIGNATION
   * @section Ui::Dialogs
   */
  void openDesignation(bool);

  /**
   * @brief ReportingDialog
   * @section Ui::Dialogs
   */
  void openReportDialog(bool);

  /**
   * @brief Dialog CategoryEdit
   * @section Ui::Dialogs
   */
  void openCategoryEdit(bool);

  /**
   * @brief Dialog KeywordEdit
   * @section Ui::Dialogs
   */
  void openKeywordEdit(bool);

  /**
   * @brief MessageTemplates
   * @section Ui::Dialogs
   */
  void openMessagesEdit(bool);

  /**
   * @brief ConfigDialog
   * @section Ui::Dialogs
   */
  void openConfiguration(bool);

  /**
   * @brief SQL Fehler in einer MessagebBox darstellen!
   */
  void sqlErrorMessageBox(const QString &);

  /**
   * @brief Vollbildmodus umschalten
   */
  void toggleFullScreen(bool);

protected Q_SLOTS:
  void closeEvent(QCloseEvent *) override;

Q_SIGNALS:
  /**
   * @brief Sendet Programm beenden an das Hauptprogramm
   */
  void sendShutdown();

  /**
   * @brief Versuch sich mit der Datenbank Neu zu verbinden.
   */
  void sendDatabaseConnect(bool);

public Q_SLOTS:
  /**
   * @brief Status Nachrichten verarbeiten.
   * Im Standard Modus werden alle Nachrichten an die Statusleiste gesendet.
   * Ist das Fenster im Systray gehen diese an @ref m_systemTray.
   * @param status Siehe Antiqua::ErrorStatus
   * @param info   Nachrichten body
   */
  void statusMessage(Antiqua::ErrorStatus status, const QString &info);

public:
  explicit MWindow(QWidget *parent = nullptr);

  /**
   * @brief Standard Konfiguration und Ansichten laden.
   * @li Fenster Dimensionen laden.
   * @li Registerkarten laden.
   * @li Standard Registerkarte nach vorne holen.
   * @li Den SystemTrayIcon anzeigen.
   */
  bool initDefaults();
};

#endif // ANTIQUACRM_MAIN_WINDOW_H
