// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailtemplatestree.h"
#include "macrostreelist.h"
#include "templatestreelist.h"

MailTemplatesTree::MailTemplatesTree(QWidget *parent) : QTabWidget{parent} {
  setMinimumWidth(200);
  setWindowIcon(AntiquaCRM::antiquaIcon("document-edit"));

  m_templates = new TemplatesTreeList(this);
  insertTab(0, m_templates, windowIcon(), tr("Templates"));

  connect(m_templates, SIGNAL(sendTemplate(const QJsonObject &)),
          SIGNAL(sendTemplate(const QJsonObject &)));

  m_macros = new MacrosTreeList(this);
  insertTab(1, m_macros, windowIcon(), tr("Macros"));
}

bool MailTemplatesTree::createTemplatesTree(AntiquaCRM::ASqlCore *msql) {
  QString _sql("SELECT * FROM ui_template_body");
  _sql.append(" ORDER BY (tb_category, tb_title);");
  QSqlQuery _q = msql->query(_sql);
  if (_q.size() > 0) {
    QJsonObject _tree;
    QJsonArray _array;
    AntiquaCRM::ASqlDataQuery _tpl(_q.record());
    while (_q.next()) {
      QJsonObject _child;
      foreach (QString _f, _tpl.columnNames()) {
        QVariant _value = _q.value(_f);
        if (!_value.isNull())
          _child.insert(_f, QJsonValue::fromVariant(_value));
      }
      _array.append(_child);
    }
    _tree.insert("tree", _array);
    m_templates->addTemplates(_tree);
    return msql->lastError().isEmpty();
  }
  return false;
}

bool MailTemplatesTree::createMacrosTree(AntiquaCRM::ASqlCore *msql) {
  AntiquaCRM::ASqlFiles _tpl("union_template_keys");
  if (_tpl.openTemplate()) {
    QSqlQuery _q = msql->query(_tpl.getQueryContent());
    if (_q.size() > 0) {
      while (_q.next()) {
        QJsonObject _obj;
        _obj.insert("title", QJsonValue(_q.value("title").toString()));
        _obj.insert("key", QJsonValue(_q.value("macro").toString()));
        m_macros->addMacro(_q.value("category").toString(), _obj);
      }
    }
    m_macros->expandAll();
    return msql->lastError().isEmpty();
  }
  return false;
}

bool MailTemplatesTree::init(AntiquaCRM::ASqlCore *psql) {
  if (psql == nullptr)
    return false;

  m_templates->cleanup();
  if (!createTemplatesTree(psql))
    return false;

  m_templates->expandAll();

  return createMacrosTree(psql);
}
