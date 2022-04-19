// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STATSBOOKBAR_H
#define STATSBOOKBAR_H

#include <QtCore/QObject>
#include <QtCore/QTimerEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>

/**
    @brief StatsBookBar
    Wird unterhalb der Buchtabelle angezeigt.
    In diesem Toolbar ist enthalten:
     @li Text Nachrichten (Label)
     @li Verlaufsauswahl  (ComboBox)
     @li Erstellen Knopf  (PushButton)

    Die Verlaufsauswahl löst aus:
     @li 1: "#today"
     @li 2: "#yesterday"
     @li 3: "#last7days"
     @li 4: "#thismonth"
     @li 5: "#thisyear"
*/
class StatsBookBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
    @brief Timer ID
    Warte mit timerEvent() auf Datenbank Verbindung
  */
  int timerID;

  /**
    @brief Zähler für Timer
    Wenn keine Datenbank vorhanden ist dann
    nach 30s ein killTimer ausführen.
  */
  int countIDs;

  /**
     @brief timeToLife
     Zeit bis Timer Zwangsbeendet wird.
  */
  int timeToLife = 30;

  /**
    @brief Info Textfeld
    Hier werden Nachrichten ausgegeben.
  */
  QLabel *m_infoLabel;

  /**
    @brief Auswahl ...
    um den Datenbankverlauf auf zu rufen.
   */
  QComboBox *m_showHistory;

  /**
   @brief Auswahl Verlaufabfrage erstellen
   @note Die Definition ist abhängig von
       @ref BooksTableView::queryHistory()
  */
  void addComboBoxData();

private Q_SLOTS:
  /**
    @brief Erstellt eine Verlaufsanfrage
    Löst bei vorhandenen Daten das Signal
    @ref s_queryHistory aus. Die ComboBox
    wird danach wieder auf currentIndex(0)
    gestellt.
    @note Während des Zurücksetzens sind
          alle Signale blockiert!
  */
  void historyChanged(int);

protected:
  /**
   @brief Timer wartet auf db::isOpen
   Weil zum Zeitpunkt der Klassen
   Initialisierung noch keine Datenbank
   Verbindung besteht. Läuft hier der
   Timer so lange bis er eine findet.
   @li Wenn Ja - Wird setThisDayHistory
       ausgelöst und der Timer beendet.
   @li Wenn Nein - Wird er sich nach 30s
       beenden und gibt den Speicher frei.
       Siehe @see timeToLife
  */
  void timerEvent(QTimerEvent *);

Q_SIGNALS:
  /**
     @brief Knopf auffrischen wurde ausgelöst.
   */
  void s_refreshView();

  /**
     @brief Auswahl Verlauf wurde getätigt
     Gibt den Benutzerdefiniert zurück.
     @note Siehe Verlaufsauswahl in der
           Klassenbeschreibung!
  */
  void s_queryHistory(const QString &);

public Q_SLOTS:
  /**
     @brief Interne Nachrichten
     @param QString
     @see QLabel::m_infoLabel
   */
  void showMessage(const QString &);

  /**
     @brief showRowCount
     Tabellenzeilenanzahl ausgeben
     @param count
   */
  void showRowCount(int count);

  /**
    @brief Wähle DB-Verlauf von @b Heute aus
  */
  void setThisDayHistory();

public:
  explicit StatsBookBar(QWidget *parent = nullptr);
};

#endif // STATSBOOKBAR_H
