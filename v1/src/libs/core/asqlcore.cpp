// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlcore.h"

namespace AntiquaCRM {

ASqlCore::ASqlCore(QObject *parent) : QObject{parent} {
  setObjectName("antiquacrm_sqlcore");
  m_cfg = new ASqlSettings(this);
}

bool ASqlCore::init() {
  p_db = QSqlDatabase::addDatabase("QPSQL", m_cfg->connectionName());

  return false;
}

}; // namespace AntiquaCRM
