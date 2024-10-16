// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdsvinylconfigwidget.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLayout>

CDsVinylConfigWidget::CDsVinylConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{CDSVINYL_INTERFACE_NAME, parent} {
  setContentsMargins(0, 0, 0, 0);

  QJsonObject _jobj = getMenuEntry();
  setWindowTitle(_jobj.value("title").toString() + " [*]");
  setToolTip(_jobj.value("tooltip").toString());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);
  // Price Limits Group
  QGroupBox *m_group0 = new QGroupBox(this);
  m_group0->setTitle(tr("Media price limits"));
  QGridLayout *grLayout2 = new QGridLayout(m_group0);
  grLayout2->setContentsMargins(5, 5, 5, 5);
  m_minPrice = new AntiquaCRM::PriceEdit(m_group0);
  m_minPrice->setObjectName("media_price_lowest");
  m_minPrice->setToolTip(tr("Underlimit price for media."));
  m_minPrice->setBuddyLabel(tr("Underlimit"));
  grLayout2->addWidget(m_minPrice, 1, 0, 1, 1);
  m_normalPrice = new AntiquaCRM::PriceEdit(m_group0);
  m_normalPrice->setObjectName("media_price_normal");
  m_normalPrice->setToolTip(tr("Default Media price when a new created."));
  m_normalPrice->setBuddyLabel(tr("Default on create"));
  m_normalPrice->appendStretch(1);
  grLayout2->addWidget(m_normalPrice, 1, 1, 1, 1);
  m_group0->setLayout(grLayout2);
  layout->addWidget(m_group0);

  m_storage = new AntiquaCRM::SelectStorage(this);
  m_storage->setObjectName("media_storage_location");
  m_storage->setBuddyLabel(tr("Storage location"));
  QString _info = tr("Default Storage location when create a new entry!");
  m_storage->setWhatsThisText(_info);
  m_storage->appendStretch(0);
  layout->addWidget(m_storage);

  layout->addStretch(1);
  setLayout(layout);
}

void CDsVinylConfigWidget::loadSectionConfig() {
  QJsonDocument jsDocument;
  QString sql("SELECT cfg_jsconfig FROM antiquacrm_configs ");
  sql.append("WHERE cfg_group='" + CDSVINYL_CONFIG_POINTER + "';");
  AntiquaCRM::ASqlCore dbsql(this);
  QSqlQuery q = dbsql.query(sql);
  if (q.size() > 0) {
    q.next();
    jsDocument = QJsonDocument::fromJson(q.value(0).toByteArray());
  } else {
    qDebug() << Q_FUNC_INFO << dbsql.lastError();
  }

  m_storage->initData();

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

void CDsVinylConfigWidget::saveSectionConfig() {
  QJsonObject _jsObject;
  QList<AntiquaCRM::AInputWidget *> _list =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _list.size(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _list.at(i);
    _jsObject.insert(m_inp->objectName(), m_inp->getValue().toJsonValue());
  }

  QJsonDocument jsDocument(_jsObject);
  QString _sql("DELETE FROM antiquacrm_configs WHERE");
  _sql.append(" cfg_group='" + CDSVINYL_CONFIG_POINTER + "';\n");
  _sql.append("INSERT INTO antiquacrm_configs (cfg_group,cfg_jsconfig)");
  _sql.append("VALUES ('" + CDSVINYL_CONFIG_POINTER + "','");
  _sql.append(jsDocument.toJson(QJsonDocument::Compact));
  _sql.append("');");

  AntiquaCRM::ASqlCore dbsql(this);
  dbsql.query(_sql);
  if (dbsql.lastError().isEmpty())
    return;

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << dbsql.lastError();
#endif
}

AntiquaCRM::ConfigType
CDsVinylConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject CDsVinylConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "media-optical-dvd-video");
  _o.insert("title", tr("CD - Vinyl"));
  _o.insert("tooltip", tr("Additional CDs and Vinyl tab settings."));
  return _o;
}
