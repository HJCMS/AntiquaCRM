// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatemplates.h"

#include <QDebug>
#include <QRegularExpressionMatch>

AntiquaTemplates::AntiquaTemplates(const QString &name, QObject *parent)
    : QObject{parent}, AntiquaCRM::ASqlFiles{name} {
  setObjectName("antiqua_templates");
  loadDefaults();
}

void AntiquaTemplates::loadDefaults() {
  p_vars.clear();
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup("company");
  foreach (QString k, cfg.allKeys()) {
    QString p = "COMPANY_" + k.toUpper();
    QString v = cfg.value(k).toString().trimmed().replace("#", " ");
    p_vars.insert(p, v);
  }
  cfg.endGroup();
}

bool AntiquaTemplates::queryContent() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(getQueryContent());
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
  return false;
}

const QString AntiquaTemplates::getTemplate() {
  if (queryContent()) {
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
