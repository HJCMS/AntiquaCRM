// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatemplates.h"

#include <QDebug>
#include <QRegularExpressionMatch>

AntiquaTemplates::AntiquaTemplates(QObject *parent) : QObject{parent} {
  setObjectName("antiqua_templates");
}

bool AntiquaTemplates::queryCompanyData() {
  AntiquaCRM::ASqlFiles file("union_template_values");
  if (file.openTemplate()) {
    QSqlQuery q = m_sql->query(file.getQueryContent());
    if (q.size() > 0) {
      while (q.next()) {
        QString p = q.value("tpl_macro").toString();
        QString v = q.value("tpl_value").toString();
        p_vars.insert(p, v);
      }
      return true;
    } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
      qWarning("SQL-Template-Query-Error");
#endif
    }
  }
  return false;
}

bool AntiquaTemplates::queryTemplateBody(const QString &sql) {
  QString str =
      AntiquaCRM::ASqlFiles::selectStatement("select_statement_template_body");
  str.append(" ");
  str.append(sql.trimmed());
  QSqlQuery q = m_sql->query(str);
  if (q.size() > 0) {
    q.next();
    QSqlRecord r = q.record();
    tb_message = q.value("tb_message").toString();
    for (int i = 0; i < r.count(); i++) {
      QSqlField f = r.field(i);
      QRegularExpressionMatch isMacro = pattern.match(f.name());
      if (isMacro.hasMatch()) {
        // qDebug() << f.name().toUpper() << q.value(f.name()).toString();
        p_vars.insert(f.name().toUpper(), q.value(f.name()).toString());
      }
    }
    // qDebug() << Q_FUNC_INFO << str;
    return true;
  } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
    qWarning("SQL-Template-Query-Error");
#endif
  }
  return false;
}

const QString AntiquaTemplates::getTemplate(const QString &caller,
                                            const QString &whereClause) {

  if (whereClause.isEmpty() || caller.isEmpty()) {
    qWarning("Missing where clause to select your template!");
    return QString();
  }

  // NOTE tb_caller must have a unique CONSTRAINT
  QString query("LEFT JOIN ui_template_body ON tb_caller='");
  query.append(caller + "' WHERE ");
  query.append(whereClause);
  query.append(" ORDER BY o_id;");

  p_vars.clear();

  m_sql = new AntiquaCRM::ASqlCore(this);
  if (queryCompanyData() && queryTemplateBody(query)) {
    QString buffer = tb_message;
    QMapIterator<QString, QString> it(p_vars);
    while (it.hasNext()) {
      it.next();
      buffer.replace("@" + it.key() + "@", it.value());
    }
    return buffer;
  }
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << query;
#endif
  return QString();
}
