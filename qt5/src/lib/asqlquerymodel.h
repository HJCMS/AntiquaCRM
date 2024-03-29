// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SQLQUERYMODEL_H
#define ANTIQUACRM_SQLQUERYMODEL_H

#include <AGlobal>
#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

namespace AntiquaCRM {

class ASqlCore;

/**
 * @brief Standard SqlQueryModel für Tabellenansichten
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY ASqlQueryModel : public QSqlQueryModel {
  Q_OBJECT

private:
  /**
   * @brief Tabellenname
   */
  const QString p_table;

protected:
  AntiquaCRM::ASqlCore *m_sql;

  /**
   * @brief Standard Formatierung für die Spaltenköpfe!
   */
  const QString setHeaderTitle(const QString &text) const;

  /**
   * @brief Datum in ein lesbares Format konvertieren!
   */
  const QString displayDate(const QVariant &value) const;

  /**
   * @brief Standard Formatierung für die Zeilennummern
   */
  const QString verticalHeader(int row, int role = Qt::DisplayRole) const;

Q_SIGNALS:
  /**
   * @brief Bei SQL Fehlern wird hiermit die Nachricht übermittelt!
   * @note Die Signalbehandlung muss in der Tabellenansicht erfolgen!
   */
  void sqlErrorMessage(const QString &table, const QString &message);

public:
  explicit ASqlQueryModel(const QString &table, QObject *parent = nullptr);

  /**
   * @brief Vordefinierte Tabellenspalten Titel
   */
  virtual const QMap<int, QString> headerList() const = 0;

  /**
   * @brief SQL abfrage ...
   */
  bool querySelect(const QString &sql);

  /**
   * @brief Tabellenname - Siehe Konstruktor und p_table
   */
  const QString tableName() const;

  /**
   * @brief SqlRecord der Abfrage
   */
  const QSqlRecord queryRecord() const;

  /**
   * @brief SqlRecord Tabellenrecord
   */
  const QSqlRecord tableRecord() const;

  /**
   * @brief Feldname/Aliasname der Abfrage!
   */
  const QString fieldName(int column = 0) const;

  /**
   * @brief Spalten Nummer der abfrage!
   */
  int column(const QString &fieldName) const;

  /**
   * @brief Datensatz abfragen
   */
  QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

  /**
   * @brief Gibt eine Infozeile mit Uhrzeit und Ergebnisgröße zurück!
   */
  const QString queryResultInfo();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLQUERYMODEL_H
