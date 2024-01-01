// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreundoperations.h"

#include <QCalendarWidget>
#include <QDateTime>
#include <QLayout>
#include <QMetaType>
#include <QSqlField>

BuchfreundOperations::BuchfreundOperations(QWidget *parent)
    : AntiquaCRM::ProviderActionDialog{parent} {
  setObjectName("buchfreund_actions_dialog");
  setWindowTitle(tr("Update Buchfreund orders.") + "[*]");

  QFrame *m_frame = new QFrame(this);
  m_frame->setFrameStyle(QFrame::NoFrame);
  m_frame->setContentsMargins(5, 2, 5, 2);

  int row = 0;
  QGridLayout *layout = new QGridLayout(m_frame);

  QLabel *_description = new QLabel(this);
  _description->setText(tr("Confirm current order with delivery date."));
  layout->addWidget(_description, row++, 0, 1, 2);

  m_info = new QLabel(this);
  layout->addWidget(m_info, row++, 0, 1, 2);

  layout->addWidget(label(tr("Order number")), row, 0, 1, 1);
  m_id = new AntiquaCRM::TextLine(this);
  m_id->setObjectName("id");
  m_id->setReadOnly(true);
  layout->addWidget(m_id, row++, 1, 1, 1);

  layout->addWidget(label(tr("Deliver service")), row, 0, 1, 1);
  m_service = new AntiquaCRM::SelectDeliverService(this);
  m_service->setObjectName("versender");
  layout->addWidget(m_service, row++, 1, 1, 1);

  layout->addWidget(label(tr("Shipment type")), row, 0, 1, 1);
  m_type = new QComboBox(this);
  m_type->setObjectName("sendungsart");
  m_type->addItem(tr("Default"), "Standard");
  m_type->addItem(tr("Package"), "Paket");
  m_type->addItem(tr("Package International"), "Paket International");
  layout->addWidget(m_type, row++, 1, 1, 1);

  layout->addWidget(label(tr("Tracking number")), row, 0, 1, 1);
  m_srv_number = new AntiquaCRM::TextLine(this);
  m_srv_number->setObjectName("sendungsnummer");
  m_srv_number->setInputToolTip(tr("Tracking number"));
  QSqlField _field;
  _field.setName("sendungsnummer");
  _field.setMetaType(QMetaType(QMetaType::QString));
  _field.setRequiredStatus(QSqlField::Required);
  m_srv_number->setRestrictions(_field);
  layout->addWidget(m_srv_number, row++, 1, 1, 1);

  const QDateTime _dt = QDateTime::currentDateTime();
  layout->addWidget(label(tr("Delivery date")), row, 0, 1, 1);
  m_dlv_date = new QDateTimeEdit(this);
  m_dlv_date->setObjectName("einlieferungsdatum");
  m_dlv_date->setDisplayFormat("dddd MMMM yyyy hh:mm");
  m_dlv_date->setDateTime(_dt);
  m_dlv_date->setMinimumDate(_dt.date().addDays(-365));
  m_dlv_date->setMaximumDate(_dt.date().addDays(365));
  m_dlv_date->setCalendarPopup(true);
  layout->addWidget(m_dlv_date, row++, 1, 1, 1);

  layout->setRowStretch(row, 1);
  m_frame->setLayout(layout);
  stackedWidget->insertWidget(0, m_frame);

  m_networker = new AntiquaCRM::ANetworker(
      AntiquaCRM::NetworkQueryType::JSON_QUERY, this);

  connect(m_networker, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(networkResponse(const QJsonDocument &)));

  connect(this, SIGNAL(sendSubmitClicked()), SLOT(prepareOperation()));
}

QLabel *BuchfreundOperations::label(const QString &text) {
  QLabel *lb = new QLabel(this);
  lb->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  lb->setText(text + ":");
  return lb;
}

const QString BuchfreundOperations::deliveryDate() {
  return m_dlv_date->dateTime().toString("yyyy-MM-dd hh:mm:ss");
}

bool BuchfreundOperations::initConfiguration() {
  p_config.clear();
  QJsonDocument _document;
  AntiquaCRM::ASqlCore dbsql(this);
  QString _sql("SELECT cfg_jsconfig FROM antiquacrm_configs ");
  _sql.append(" WHERE cfg_group='CONFIG_BUCHFREUND';");
  QSqlQuery _q = dbsql.query(_sql);
  if (_q.size() > 0) {
    _q.next();
    _document = QJsonDocument::fromJson(_q.value(0).toByteArray());
  }

  if (_document.isNull())
    return false;

  QJsonObject _config = _document.object();
  QJsonObject::const_iterator it;
  for (it = _config.begin(); it != _config.end(); ++it) {
    p_config.insert(it.key(), it.value().toVariant());
  }

  if (!p_config.contains("api_key") || p_config.value("api_key").isNull()) {
    qWarning("Buchfreund - API Key is not configured!");
    return false;
  }

  return (p_config.size() > 2);
}

const QUrl BuchfreundOperations::apiQuery(const QString &operation) {
  QString _path(p_config.value("m_api_path").toString());
  _path.append(p_config.value("api_key").toString());
  _path.append("/");
  _path.append(operation);

  QUrl _url;
  _url.setScheme("https");
  _url.setHost(p_config.value("api_host").toString());
  _url.setPath(_path);
  return _url;
}

void BuchfreundOperations::networkResponse(const QJsonDocument &document) {
  qDebug() << Q_FUNC_INFO << "TODO" << document.toJson(QJsonDocument::Indented);
}

void BuchfreundOperations::prepareOperation() {
  QJsonObject _obj;
  _obj.insert("id", p_order_id);
  _obj.insert("versender", m_service->getValue().toString());
  const QString _type = m_type->itemData(m_type->currentIndex()).toString();
  _obj.insert("sendungsart", _type);
  QString _nr = m_srv_number->getValue().toString();
  if (!m_srv_number->isValid()) {
    m_srv_number->setFocus();
    return;
  }
  _obj.insert("sendungsnummer", _nr);
  _obj.insert("einlieferungsdatum", deliveryDate());

  QJsonDocument _body(_obj);
  if (_body.isNull()) {
    qWarning("Buchfreund: Invalid Body detected!");
    return;
  }

  m_networker->jsonPostRequest(apiQuery("versandbestaetigung"), _body);
}

int BuchfreundOperations::exec(const QJsonObject &data) {
  if (!initConfiguration()) {
    qWarning("Plugin Buchfreund missing configuration!");
    return QDialog::Rejected;
  }

  QJsonObject _main = data.value("DATA").toObject();
  QJsonObject _order = _main.value("orderinfo").toObject();
  p_order_id = _order.value("o_provider_order_id").toString();
  if (p_order_id.isEmpty()) {
    qWarning("Missing OrderId - Operation rejected!");
    return QDialog::Rejected;
  }

  m_info->setText(tr("Send confirmation to customer <b>%1</b>.")
                      .arg(_main.value("buyer").toString()));

  m_service->initData();
  m_id->setValue(p_order_id);

  return AntiquaCRM::ProviderActionDialog::exec();
}
