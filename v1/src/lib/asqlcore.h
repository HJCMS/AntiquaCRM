// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLCORE_H
#define ANTIQUACRM_SQLCORE_H

#include <AGlobal>
#include <QObject>
#include <QString>
#include <QtSql>

namespace AntiquaCRM {

class ASqlSettings;

/**
 * @brief Primärklasse für die Datenbank
 * @section sql
 * Die ASqlCore prüft Standardmäßig die Treiber und greift auf die
 * Datenbankverbindung zu!
 * @note AntiquaCRM macht beim Start eine Session-Verbindung auf!
 */
class ANTIQUACRM_LIBRARY ASqlCore final : public QObject {
  Q_OBJECT

private:
  ASqlSettings *m_cfg;
  QSqlDatabase *database;

  /**
   * @brief Erstelle Datenbankverbindung
   */
  bool initDatabase();

  /**
   * @brief Prüfe Verbindung
   */
  bool isConnected();

public:
  /**
   * @brief Enumwert für Datenbank Status
   */
  enum Status { CLOSED = 0x0, OPEN = 0x1 };
  Q_ENUM(ASqlCore::Status);

  explicit ASqlCore(QObject *parent = nullptr);

  /**
   * @brief Datenbank öffnen
   * @note Muss nur einmal aufgerufen werden.
   * Für gewöhnlich in der Q*Application Klasse.
   */
  bool open();

  /**
   * @brief Gibt den Status der Datenbank-Verbindung zurück!
   */
  AntiquaCRM::ASqlCore::Status status();

  /**
   * @brief Optional Datenbank Verbindung beziehen!
   * @note Nicht empfohlen und wirklich nur einsetzen wenn es nicht anders geht!
   */
  const QSqlDatabase db();

  /**
   * @brief Ausführliche Tabellen Informationen beziehen.
   * @note Es ist bei Nested-Abfragen besser QSqlQuery::record() zu verwenden!
   */
  const QSqlRecord record(const QString &table);

  /**
   * @brief Nehme Feldnamen von Tabelle
   */
  const QStringList fieldNames(const QString &table);

  /**
   * @brief Standard Verarbeitung der SQL Anfragen.
   */
  const QSqlQuery query(const QString &statement);

  /**
   * @brief Gibt die letzte Fehlermeldung zurück.
   */
  const QString lastError();

  /**
   * @brief Erzwinge Datenbank verbindung schließen.
   * @note Normalerweise nicht Notwendig!
   */
  void close();
};

}; // namespace AntiquaCRM

#endif // #ifndef ANTIQUACRM_SQLCORE_H
