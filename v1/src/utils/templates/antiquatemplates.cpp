// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatemplates.h"

#include <QDebug>
#include <QRegularExpressionMatch>

AntiquaTemplates::AntiquaTemplates(QObject *parent) : QObject{parent} {
  setObjectName("antiqua_templates");
}

bool AntiquaTemplates::queryCompanyData() {
  AntiquaCRM::ASqlFiles file("query_company_data");
  if (file.openTemplate()) {
    QSqlQuery q = m_sql->query(file.getQueryContent());
    if (q.size() > 0) {
      while (q.next()) {
        QString p = q.value("ac_class").toString();
        QString v = q.value("ac_value").toString();
        p_vars.insert(p, v);
      }
      return true;
    }
  }
  return false;
}

bool AntiquaTemplates::queryTemplateBody(const QString &sql) {
  AntiquaCRM::ASqlFiles file("query_template_body");
  if (file.openTemplate()) {
    file.setWhereClause(sql);
    QSqlQuery q = m_sql->query(file.getQueryContent());
    if (q.size() > 0) {
      q.next();
      QSqlRecord r = q.record();
      tb_attachment = q.value("tb_attachment").toBool();
      tb_message = q.value("tb_message").toString();
      for (int i = 0; i < r.count(); i++) {
        QSqlField f = r.field(i);
        QRegularExpressionMatch isMacro = pattern.match(f.name());
        if (isMacro.hasMatch()) {
          p_vars.insert(f.name().toUpper(), q.value(f.name()).toString());
        }
      }
      return true;
    }
  }
  return false;
}

const QString AntiquaTemplates::getTemplate(const QString &whereClause) {
  if (whereClause.isEmpty()) {
    qWarning("Missing where clause to select your template!");
    return QString();
  }

  p_vars.clear();
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (queryCompanyData() && queryTemplateBody(whereClause)) {
    QString buffer = tb_message;
    QMapIterator<QString, QString> it(p_vars);
    while (it.hasNext()) {
      it.next();
      buffer.replace("@" + it.key() + "@", it.value());
    }
    return buffer;
  }
  return QString();
}
