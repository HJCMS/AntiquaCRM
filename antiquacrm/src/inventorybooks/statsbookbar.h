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
  Q_CLASSINFO("URL", "http://www.hjcms.de")

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
   @brief Knopf für den erstellen Dialog
  */
  QPushButton *m_createEntryBtn;

  /**
   @brief Auswahl Verlaufabfrage erstellen
  */
  void addComboBoxData();

private Q_SLOTS:
  /**
   @brief Auswahl verlauf wurde ausgelöst
  */
  void historyChanged(int);

protected:
  /**
   @brief Timer für
  */
  void timerEvent(QTimerEvent *);

Q_SIGNALS:
  /**
   @brief Signal neuen Eintrag erstellen
  */
  void s_createEntryClicked();

  /**
     @brief Auswahl Verlauf wurde getätigt
     Gibt den Wert von Qt::UserRole zurück.
     @see historyChanged(int)
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
    @brief Wähle DB-Verlauf von @b Heute aus
  */
  void setThisDayHistory();

public:
  explicit StatsBookBar(QWidget *parent = nullptr);
};

#endif // STATSBOOKBAR_H
