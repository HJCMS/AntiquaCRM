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
 * @brief core sql functionality
 * @section sql
 */
class ANTIQUACRM_LIBRARY ASqlCore final : public QObject {
  Q_OBJECT

private:
  ASqlSettings *m_cfg;
  QSqlDatabase *database;

  bool initDatabase();
  bool isConnected();

public:
  explicit ASqlCore(QObject *parent = nullptr);
  bool open();
  const QSqlDatabase db();
  const QSqlRecord record(const QString &table);
  const QStringList fieldNames(const QString &table);
  const QSqlQuery query(const QString &statement);
  const QString lastError();
  void close();
};

}; // namespace AntiquaCRM

#endif // #ifndef ANTIQUACRM_SQLCORE_H
