// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITORMAIN_UTILS_H
#define EDITORMAIN_UTILS_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtWidgets/QWidget>

/**
 * @defgroup HJCMS
 * @{
 */
#include <SqlCore>

namespace HJCMS {
class SqlCore;
};
/** }@ */

/**
 * @class EditorMain
 * @brief Abstraktions Klasse für Editoren
 * Diese Klasse dient für die Vereinheitlichung von
 * SQL Abfragen und Signalen in den Editor Fenstern.
 * @see Siehe auch @class Inventory
 */
class EditorMain : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  /**
   * @brief Datenbank Verbindung
   * Stellt die Datenbank Verbindung für die
   * Unterklassen.
   */
  HJCMS::SqlCore *m_sql;

  /**
     @brief DataEntries
     Lese die Datenfelder und schreibe sie für die Zurücksetzen
     funktion hier rein.

     @note Die Objektnamen die Typangabe in @b field (Feldnamen)
      stimmen mit den SQL Tabellenspalten Bezeichnungen überein!

     @li Die Typangabe @b field ist für das Identifizieren
      der Eingabefelder in der Klasse zuständig.

     @li Mit @b vtype wird die Entscheidung getroffen welches
      Datensatzformat verwendet werden soll. Die Entscheidung
      liegt zu 100% beim Rückgabe ergebnis von QSqlQuery.

     @li Der Wert @b data ist vom Type Variant, was die Erstellung
      der SQL INSERT/UPDATE Statements vereinfachen soll.

     @warning Wenn sich bei der Datenbank Tabelle ein Spalten Typ
      ändert. Muss das hier Kontrolliert und Überarbeitet werden!
   */
  struct DataEntries {
    QString field; /**< @brief Feld ist gleichwertig mit {INPUT}.objectName() */
    int vtype;     /**< @brief QVariant::Type */
    QVariant data; /**< @brief Datenwert */
  };

  /**
   * @defgroup inputList
   * @{
   * @brief Objektnamen-Liste der Eingabefelder
   * Die vorgesehene Mehtode zum befüllen ist @ref setInputList
   */
  QStringList inputList;

  /**
   * @brief Alle Eingabefelder in @ref inputList einfügen.
   */
  virtual void setInputList() = 0;
  /** }@ */

  /**
   * @defgroup sqlQueryResult
   * @{
   * @brief Hier werden die Daten aus der Abfrage eingefügt.
   * Die vorgesehene Mehtode zum befüllen ist @ref importSqlResult
   */
  QVector<DataEntries> sqlQueryResult;

  /**
   * @brief Methode zum befüllen von @ref sqlQueryResult
   */
  virtual void importSqlResult() = 0;
  /** }@ */

  /**
   * @defgroup SQL Query Processing
   * @{
   * @brief Methode für SQL Anfragen absenden.
   */
  virtual bool sendSqlQuery(const QString &) = 0;

  /**
   * @brief SQL UPDATE Statement erstellen!
   */
  virtual void createSqlUpdate() = 0;

  /**
   * @brief SQL INSERT Statement erstellen!
   */
  virtual void createSqlInsert() = 0;

  /**
   * @brief Methode zum setzen des Datensatzeintrages
   * Hier sollen die Eingabemasken befüllt werden.
   * @param key   =  QString(Datenfeld Bezeichner)
   * @param value = QVariant(Datenfeld Wert)
   */
  virtual void setSqlQueryData(const QString &key, const QVariant &value) = 0;
  /** }@ */

  /**
   * @brief Routine für Eingabeklassen
   * Wenn Eingaben auf isModified stehen.
   * Kann das hier wieder zurück gesetzt werden.
   */
  virtual void resetModified() = 0;

protected Q_SLOTS:
  /**
   * @brief Daten speichern Methode
   */
  virtual void saveData() = 0;

  /**
   * @brief Routine zum Daten zurücksetzen
   */
  virtual void clearDataFields() = 0;

  /**
   * @brief Prüfung für Datensatzänderungen!
   * @return Bei @b true, wurden Datenfelder nicht gespeichert!
   */
  virtual bool checkIsModified() = 0;

  /**
   * @brief Vor dem verlassen nach Änderungen suchen.
   * Die Methode soll z.B. @ref checkIsModified() aufrufen.
   *
   * Wenn es keine Daten zu Speichern gibt, kann weiter zu
   * @ref finalLeaveEditor gegangen werden!
   */
  virtual void checkLeaveEditor() = 0;

  /**
   * @brief Beendet den Editor entgültig!
   * @note Ist zum Aufräumen vorgesehen!
   */
  virtual void finalLeaveEditor() = 0;

Q_SIGNALS:
  /**
   * @brief Verteile Änderungen ...
   */
  void s_isModified(bool);

  /**
   * @brief Signal an Elternfenster das Editor verlassen werden kann.
   * @note Es ist Aufgabe vom Elternfenster dies zu erledigen!
   */
  void s_leaveEditor();

  /**
   * @brief Bildbearbeitung öffnen
   */
  void s_openImageEditor(double);

  /**
   * @brief Meldungen absenden!
   */
  void s_postMessage(const QString &);

  /**
   * @brief Meldung Artikel Aktiviert/Deaktiviert.
   * Nachricht das der Artikel Aktiviert oder Deaktiviert wurde.
   * Wird in @ref createSqlUpdate und in @ref realyDeactivateBookEntry
   * ausgelöst! Es wird nicht beim Insert abgefangen weil die Artikel ID zu
   * diesem Zeitpunkt noch nicht vorhanden ist. (Also auch keine Augträge
   * vorhanden sein können!)
   * @note Das Signal ist Relevant für die Auftrags und Shop Verwaltung!
   *       Es soll nur mitteilen das sich was geändert hat.
   *       Die Prüfung der Datensätze erlogt an anderer Stelle!
   *
   * @li true  = Artikel wurde @b Aktiviert
   * @li false = Artikel wurde @b Deaktiviert
   */
  void s_articleActivation(bool);

public Q_SLOTS:
  /**
   * @brief Zurücksetzen der Daten
   */
  virtual void restoreDataset() = 0;

public:
  explicit EditorMain(QWidget *parent = nullptr);
  virtual ~EditorMain();
};

#endif // EDITORMAIN_UTILS_H
