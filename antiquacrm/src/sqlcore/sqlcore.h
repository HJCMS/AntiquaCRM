// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCORE_H
#define HJCMS_SQLCORE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtSql/QSqlDatabase>

namespace HJCMS {

class SqlConfig;

class SqlCore : public QObject {
  Q_OBJECT

private:
  SqlConfig *m_cfg;
  QSqlDatabase p_db;
  const QString prepareErrorMessage();

protected:
  void initDatabase();

Q_SIGNALS:
  void s_sendMessage(const QString &);
  void s_connectionStatus(bool);

public Q_SLOTS:
  void statusTrigger();

public:
  QSqlDatabase *database;
  explicit SqlCore(QObject *parent = nullptr);
  ~SqlCore();
};

};     // namespace HJCMS
#endif // HJCMS_SQLCORE_H
