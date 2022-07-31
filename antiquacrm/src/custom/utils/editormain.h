// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITORMAIN_UTILS_H
#define EDITORMAIN_UTILS_H

#include <QObject>
#include <QRegularExpression>
#include <QVariant>
#include <QWidget>
#include <QLocalSocket>

#include <AntiquaCRM>
#include <SqlCore>

namespace HJCMS {
class SqlCore;
};

class ApplicationClient;

/**
 * @class EditorMain
 * @abstract
 * @ingroup Utils
 * @brief Hauptklasse für die Inventory Editoren
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
   * @brief Message Client
   */
  ApplicationClient *m_ipc;

  /**
   * @group Feldlisten
   * @{
   * @brief Objektnamen-Liste der Eingabefelder
   * Die vorgesehene Mehtode zum befüllen ist @ref setInputList
   */
  QStringList inputList;

  /**
   * @ingroup Feldlisten
   * @brief Felder von Standardabfragen ausschließen
   */
  QStringList ignoreList;
  bool isInIgnoreList(const QString &);

  /**
   * @ingroup Feldlisten
   * @brief Alle Eingabefelder in @ref inputList einfügen.
   */
  virtual void setInputList() = 0;
  /** }@ */

  /**
   * @brief Nachrichten an Hauptfenster
   */
  QLocalSocket *m_socketClient;
  void socketStatusMessage(const QString &message);

  /**
   * @brief Hier werden die Daten aus der Abfrage eingefügt.
   * Die vorgesehene Mehtode zum befüllen ist @ref importSqlResult
   */
  DataFields sqlQueryResult;

  /**
   * @brief Methode zum befüllen von @ref sqlQueryResult
   */
  virtual void importSqlResult() = 0;

  /**
   * @brief Suche in "sqlQueryResult" für Vergleichsoperationen
   * @param key  Zeiger auf Datenfeld
   */
  const QVariant findResultValue(const QString &key);

  /**
   * @brief Datensätze aufarbeiten
   * Soll von @ref createSqlUpdate und @ref createSqlInsert
   * aufgerufen werden.
   * @note Bei Fehlern einen leeren Hash zurück geben.
   * @return Hash
   */
  virtual const QHash<QString, QVariant> createSqlDataset() = 0;

  /**
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
  virtual void setData(const QString &key, const QVariant &value,
                       bool required = false) = 0;

  /**
   * @brief Routine für Eingabeklassen
   * Wenn Eingaben auf isModified stehen.
   * Kann das hier wieder zurück gesetzt werden.
   */
  void resetModified(const QStringList &);

  /**
   * @brief Prüfung für Datensatzänderungen!
   * @return Bei @b true, wurden Datenfelder nicht gespeichert!
   */
  bool checkIsModified(const QRegularExpression &);

  /**
   * @brief SQL Fehler Nachrichten ausgeben
   * @param code  - SQL Statement
   * @param error - SQL Error
   */
  int sqlErrnoMessage(const QString &code, const QString &error);

  /**
   * @brief SQL Erfolgreich Nachricht
   * @param info
   */
  int sqlSuccessMessage(const QString &info, int timeout = 1);

  /**
   * @brief SQL Hinweis Nachricht
   * @param info
   */
  int sqlNoticeMessage(const QString &info);

protected Q_SLOTS:
  /**
   * @brief Daten speichern Methode
   */
  virtual void saveData() = 0;

  /**
   * @brief Routine zum Daten zurücksetzen
   */
  void clearDataFields(const QRegularExpression &);

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
  void s_openImageEditor(int);

  /**
   * @brief Meldungen absenden!
   */
  void s_postMessage(const QString &);

  /**
   * @brief Meldung Artikelbestand Remote ändern!
   * Info senden das die Artikelanzahl sich geändert hat. Wird in
   * "createSqlUpdate" und in "realyDeactivateBookEntry" ausgelöst! Es wird
   * nicht beim Insert abgefangen weil die Artikel ID zu diesem Zeitpunkt noch
   * nicht vorhanden ist. Also auch keine Aufträge vorhanden sein können!
   * @note Das Signal ist Relevant für die Auftrags und Shop Verwaltung!
   *       Es soll nur mitteilen das sich was geändert hat.
   *       Die Prüfung der Datensätze erlogt an anderer Stelle!
   *
   * @param articleId  - Artikel Nummer
   * @param count      - Artikel Bestand
   */
  void s_articleCount(int articleId, int count);

  /**
   * @brief Datenbank Bild zu einem Dienstleister hochladen!
   * @param articleId
   */
  void s_uploadImage(int articleId);

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
