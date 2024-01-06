// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerconfigwidget.h"
#include "booklookerconfig.h"

#include <QGroupBox>
#include <QLayout>

BookLookerConfigWidget::BookLookerConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"booklooker", parent} {
  setObjectName("config_booklooker");

  QJsonObject _jobj = getMenuEntry();
  setWindowTitle(_jobj.value("title").toString() + " [*]");
  setToolTip(_jobj.value("tooltip").toString());

  int _row = 0;
  QString _info = toolTip();
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(new QLabel(_info, this));

  QGroupBox *gbox = new QGroupBox(this);
  gbox->setTitle(tr("Access Configuration"));

  AntiquaCRM::ConfigGridLayout *gb1_layout =
      new AntiquaCRM::ConfigGridLayout(gbox);
  gb1_layout->setColumnStretch(1, 1);

  m_api_host = new AntiquaCRM::TextLine(this);
  m_api_host->setObjectName("api_host");
  m_api_host->setInputToolTip(tr("API Hostname"));
  m_api_host->setValue("api.booklooker.de");
  gb1_layout->addWidget(label(tr("Host")), _row, 0, 1, 1);
  gb1_layout->addWidget(m_api_host, _row, 1, 1, 1);
  _info = tr("API Hostname. Default: api.booklooker.de");
  gb1_layout->addToolTip(_row++, 2, _info);

  m_api_path = new AntiquaCRM::TextLine(this);
  m_api_path->setObjectName("m_api_path");
  m_api_path->setInputToolTip(tr("API Hostname"));
  m_api_path->setValue("/2.0/");
  gb1_layout->addWidget(label(tr("Path")), _row, 0, 1, 1);
  gb1_layout->addWidget(m_api_path, _row, 1, 1, 1);
  _info = tr("API Query path. Default: /2.0/");
  gb1_layout->addToolTip(_row++, 2, _info);

  m_api_key = new AntiquaCRM::TextLine(this);
  m_api_key->setObjectName("api_key");
  m_api_key->setInputToolTip(tr("API Key"));
  gb1_layout->addWidget(label(tr("Key")), _row, 0, 1, 1);
  gb1_layout->addWidget(m_api_key, _row, 1, 1, 1);
  _info = tr("Connection API Key");
  gb1_layout->addToolTip(_row++, 2, _info);

  m_api_port = new AntiquaCRM::NumEdit(this);
  m_api_port->setObjectName("api_port");
  m_api_port->setRange(80, 49151);
  m_api_port->setValue(443);
  m_api_port->setInputToolTip(tr("Port"));
  m_api_port->appendStretch(0);
  gb1_layout->addWidget(label(tr("Port")), _row, 0, 1, 1);
  gb1_layout->addWidget(m_api_port, _row, 1, 1, 1);
  _info = tr("API Connection Port. Default: 443");
  gb1_layout->addToolTip(_row++, 2, _info);
  gbox->setLayout(gb1_layout);
  layout->addWidget(gbox);

  _info = tr("As part of GDPR, %1 only supports secure connections.")
              .arg(ANTIQUACRM_DISPLAYNAME);
  layout->addWidget(new QLabel(_info, this));

  layout->addStretch(1);

  setLayout(layout);
}

QLabel *BookLookerConfigWidget::label(const QString &text) {
  QLabel *lb = new QLabel(this);
  lb->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  lb->setText(text + ":");
  return lb;
}

void BookLookerConfigWidget::loadSectionConfig() {
  QJsonDocument jsDocument;
  AntiquaCRM::ASqlCore dbsql(this);
  QString _sql("SELECT cfg_jsconfig FROM antiquacrm_configs ");
  _sql.append(" WHERE cfg_group='");
  _sql.append(BOOKLOOKER_CONFIG_POINTER);
  _sql.append("';");
  QSqlQuery _q = dbsql.query(_sql);
  if (_q.size() > 0) {
    _q.next();
    jsDocument = QJsonDocument::fromJson(_q.value(0).toByteArray());
  } else if (!dbsql.lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << dbsql.lastError();
#else
    qWarning("SQL Query-Error: %s", __FUNCTION__);
#endif
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

void BookLookerConfigWidget::saveSectionConfig() {
  QJsonObject _jsObject;
  QList<AntiquaCRM::AInputWidget *> _list =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _list.size(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _list.at(i);
    _jsObject.insert(m_inp->objectName(), m_inp->getValue().toJsonValue());
  }

  QJsonDocument jsDocument(_jsObject);
  QString _sql("DELETE FROM antiquacrm_configs WHERE");
  _sql.append(" cfg_group='" + BOOKLOOKER_CONFIG_POINTER + "';\n");
  _sql.append("INSERT INTO antiquacrm_configs (cfg_group,cfg_jsconfig)");
  _sql.append(" VALUES ('" + BOOKLOOKER_CONFIG_POINTER + "','");
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

AntiquaCRM::ConfigType BookLookerConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject BookLookerConfigWidget::getMenuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", "Booklooker");
  _jo.insert("id", "booklooker_tab");
  _jo.insert("icon", "antiquacrm");
  _jo.insert("tooltip", tr("Booklooker - booklooker.de"));
  return _jo;
}
