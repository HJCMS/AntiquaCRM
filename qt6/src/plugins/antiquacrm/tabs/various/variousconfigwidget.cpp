// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "variousconfigwidget.h"
#include "variousconfig.h"

VariousConfigWidget::VariousConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"Various", parent} {
  setContentsMargins(0, 0, 0, 0);

  QJsonObject _jobj = getMenuEntry();
  setWindowTitle(_jobj.value("title").toString() + " [*]");
  setToolTip(_jobj.value("tooltip").toString());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);
  // Price Limits Group
  QGroupBox *m_group0 = new QGroupBox(this);
  m_group0->setTitle(tr("Various price limits"));
  QGridLayout *grLayout2 = new QGridLayout(m_group0);
  grLayout2->setContentsMargins(5, 5, 5, 5);
  m_minPrice = new AntiquaCRM::PriceEdit(m_group0);
  m_minPrice->setObjectName("various_price_lowest");
  m_minPrice->setToolTip(tr("Minimum price."));
  m_minPrice->setBuddyLabel(tr("Price underlimit"));
  grLayout2->addWidget(m_minPrice, 1, 0, 1, 1);
  m_normalPrice = new AntiquaCRM::PriceEdit(m_group0);
  m_normalPrice->setObjectName("various_price_normal");
  m_normalPrice->setToolTip(tr("Default price when a new created."));
  m_normalPrice->setBuddyLabel(tr("Default on create"));
  m_normalPrice->appendStretch(1);
  grLayout2->addWidget(m_normalPrice, 1, 1, 1, 1);
  m_group0->setLayout(grLayout2);
  layout->addWidget(m_group0);
  layout->addStretch(1);
  setLayout(layout);
}

void VariousConfigWidget::loadSectionConfig() {
  QJsonDocument jsDocument;
  QString sql("SELECT cfg_jsconfig FROM antiquacrm_configs");
  sql.append(" WHERE cfg_group='");
  sql.append(VARIOUS_CONFIG_POINTER);
  sql.append("';");
  AntiquaCRM::ASqlCore dbsql(this);
  QSqlQuery q = dbsql.query(sql);
  if (q.size() > 0) {
    q.next();
    jsDocument = QJsonDocument::fromJson(q.value(0).toByteArray());
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

void VariousConfigWidget::saveSectionConfig() {
  QJsonObject _jsObject;
  QList<AntiquaCRM::AInputWidget *> _list =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _list.size(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _list.at(i);
    _jsObject.insert(m_inp->objectName(), m_inp->getValue().toJsonValue());
  }

  QJsonDocument jsDocument(_jsObject);
  QString _sql("DELETE FROM antiquacrm_configs WHERE");
  _sql.append(" cfg_group='" + VARIOUS_CONFIG_POINTER + "';\n");
  _sql.append("INSERT INTO antiquacrm_configs (cfg_group,cfg_jsconfig)");
  _sql.append("VALUES ('" + VARIOUS_CONFIG_POINTER + "','");
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

AntiquaCRM::ConfigType VariousConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject VariousConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "applications-other");
  _o.insert("title", tr("Various"));
  _o.insert("tooltip", tr("Additional Varioustab settings."));
  return _o;
}
