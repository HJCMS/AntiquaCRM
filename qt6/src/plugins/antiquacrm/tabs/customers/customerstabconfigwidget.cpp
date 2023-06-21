// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstabconfigwidget.h"
#include "customersconfig.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLayout>

CustomersTabConfigWidget::CustomersTabConfigWidget(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"tabs", CUSTOMERS_INTERFACE_NAME, parent} {
  setContentsMargins(0, 0, 0, 0);

  QJsonObject _jobj = getMenuEntry();
  setWindowTitle(_jobj.value("title").toString() + " [*]");
  setToolTip(_jobj.value("tooltip").toString());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);

  // Main Group
  QGroupBox *m_group0 = new QGroupBox(this);
  m_group0->setTitle(tr("Customer Settings"));
  QGridLayout *grLayout2 = new QGridLayout(m_group0);
  grLayout2->setContentsMargins(5, 5, 5, 5);

  m_group0->setLayout(grLayout2);
  layout->addWidget(m_group0);

  layout->addStretch(1);
  setLayout(layout);
}

void CustomersTabConfigWidget::loadSectionConfig() {
  QJsonDocument jsDocument;
  QString sql("SELECT cfg_jsconfig FROM antiquacrm_configs");
  sql.append(" WHERE cfg_group='");
  sql.append(CUSTOMERS_CONFIG_POINTER);
  sql.append("';");
  AntiquaCRM::ASqlCore dbsql(this);
  QSqlQuery q = dbsql.query(sql);
  if (q.size() > 0) {
    q.next();
    jsDocument = QJsonDocument::fromJson(q.value(0).toByteArray());
  } else {
    qDebug() << Q_FUNC_INFO << dbsql.lastError();
  }

  if (jsDocument.isNull())
    return;

  QJsonObject _jsObject = jsDocument.object();
  QJsonObject::const_iterator it;
  for (it = _jsObject.begin(); it != _jsObject.end(); ++it) {
    AntiquaCRM::AInputWidget *inp =
        findChild<AntiquaCRM::AInputWidget *>(it.key());
    if (inp != nullptr)
      inp->setValue(it.value().toVariant());
  }
}

void CustomersTabConfigWidget::saveSectionConfig() {
  QJsonObject _jsObject;
  QList<AntiquaCRM::AInputWidget *> _list =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _list.size(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _list.at(i);
    _jsObject.insert(m_inp->objectName(), m_inp->getValue().toJsonValue());
  }

  QJsonDocument jsDocument(_jsObject);
  QString _sql("DELETE FROM antiquacrm_configs WHERE");
  _sql.append(" cfg_group='" + CUSTOMERS_CONFIG_POINTER + "';\n");
  _sql.append("INSERT INTO antiquacrm_configs (cfg_group,cfg_jsconfig)");
  _sql.append("VALUES ('" + CUSTOMERS_CONFIG_POINTER + "','");
  _sql.append(jsDocument.toJson(QJsonDocument::Compact));
  _sql.append("');");

  AntiquaCRM::ASqlCore dbsql(this);
  dbsql.query(_sql);
  if (dbsql.lastError().isEmpty())
    return;

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << dbsql.lastError();
#endif
}

AntiquaCRM::TabsConfigWidget::ConfigType CustomersTabConfigWidget::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_DATABASE;
}

const QJsonObject CustomersTabConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "kjournal");
  _o.insert("title", tr("Customers"));
  _o.insert("tooltip", tr("Additional Customers tab settings."));
  return _o;
}
