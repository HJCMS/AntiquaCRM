// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STATSACTIONBAR_UTILS_H
#define STATSACTIONBAR_UTILS_H

#include <QComboBox>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QTimerEvent>
#include <QToolBar>

/**
 * @class StatsActionBar
 * @brief Wird unterhalb der Buchtabelle angezeigt.
 * In diesem Toolbar ist enthalten:
 *  @li Text Nachrichten (Label)
 *  @li Verlaufsauswahl  (ComboBox)
 *  @li Erstellen Knopf  (PushButton)
 *
 * Die Verlaufsauswahl löst aus:
 *  @li 1: "#today"
 *  @li 2: "#yesterday"
 *  @li 3: "#last7days"
 *  @li 4: "#thismonth"
 *  @li 5: "#thisyear"
 */
class StatsActionBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Timer ID Warte mit timerEvent() auf Datenbank Verbindung
   */
  int timerID;

  /**
   * @brief Zähler für Timer
   * Wenn keine Datenbank vorhanden ist dann nach 30s ein killTimer ausführen.
   */
  int countIDs;

  /**
   * @brief Zeit bis Timer Zwangsbeendet wird.
   */
  int timeToLife = 30;

  /**
   * @brief Info Textfeld
   * Hier werden Nachrichten ausgegeben.
   */
  QLabel *m_infoLabel;

  /**
   * @brief Neuer Eintrage
   */
  QPushButton *btn_new;

  /**
   * @brief Aktualisieren
   */
  QPushButton *btn_refresh;

  /**
   * @brief Auswahl um den Datenbankverlauf auf zu rufen.
   */
  QComboBox *m_showHistory;

  /**
   * @brief Auswahl Verlaufabfrage erstellen
   * @note Die Definition ist abhängig von @ref BooksTableView::queryHistory()
   */
  void addComboBoxData();

private Q_SLOTS:
  /**
   * @brief Erstellt eine Verlaufsanfrage Löst bei vorhandenen Daten das Signal
   * @ref s_queryHistory aus. Die ComboBox wird danach wieder auf
   * currentIndex(0) gestellt.
   * @note Während des Zurücksetzens sind alle Signale blockiert!
   */
  void historyChanged(int);

protected:
  /**
   * @brief Timer wartet auf db::isOpen Weil zum Zeitpunkt der Klassen
   * Initialisierung noch keine Datenbank Verbindung besteht. Läuft hier der
   * Timer so lange bis er eine findet.
   * @li Wenn Ja - Wird setThisDayHistory ausgelöst und der Timer beendet.
   * @li Wenn Nein - Wird er sich nach 30s beenden und gibt den Speicher frei.
   * Siehe @see timeToLife
   */
  void timerEvent(QTimerEvent *);

Q_SIGNALS:
  /**
   * @brief Knopf auffrischen wurde ausgelöst.
   */
  void sendRefreshView();

  /**
   * @brief Auswahl Verlauf wurde getätigt Gibt den Benutzerdefiniert zurück.
   * @note Siehe Verlaufsauswahl in der Klassenbeschreibung!
   */
  void sendQueryHistory(const QString &);

  /**
   * @brief Einen neuen Eintrag erstellen!
   */
  void sendCreateEntry();

public Q_SLOTS:
  /**
   * @brief Interne Nachrichten
   * @see QLabel::m_infoLabel
   */
  void showMessage(const QString &message, int rows = 0);

  /**
   * @brief Tabellenzeilenanzahl ausgeben
   */
  void showRowCount(int count);

  /**
   * @brief Wähle DB-Verlauf von @b Heute aus
   */
  void setThisDayHistory();

  /**
   * @brief Erstellen Button aktivieren
   */
  void setEnableCreateButton(bool b);

public:
  explicit StatsActionBar(QWidget *parent = nullptr,
                          bool showCreateButton = false);
};

#endif // STATSACTIONBAR_UTILS_H
