// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLCORE_H
#define ANTIQUACRM_SQLCORE_H

#include <QObject>
#include <QString>
#include <QtSql>
#include <AntiquaCRM>

namespace AntiquaCRM {

class ASqlCore : public QObject  {
  Q_OBJECT

private:
  ASqlSettings *m_cfg;
  QSqlDatabase p_db;

public:
  explicit ASqlCore(QObject *parent = nullptr);
  bool init();
};

}; // namespace AntiquaCRM

#endif // #ifndef ANTIQUACRM_SQLCORE_H
