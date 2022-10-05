// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "workload.h"

Workload::Workload(AntiquaCRM::ASqlCore *pgsql)
  : QObject{}, m_sql{pgsql} {
}
