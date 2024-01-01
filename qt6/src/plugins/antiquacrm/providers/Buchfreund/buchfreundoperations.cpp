// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreundoperations.h"

BuchfreundOperations::BuchfreundOperations(QWidget *parent)
    : AntiquaCRM::ProviderActionDialog{parent} {
  setObjectName("buchfreund_actions_dialog");
  setWindowTitle(tr("Update Buchfreund orders.") + "[*]");
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

const QUrl BuchfreundOperations::apiQuery(const QString &target) {
  Q_UNUSED(target);
  qDebug() << Q_FUNC_INFO << "TODO";
  return QUrl();
}

void BuchfreundOperations::prepareOperation() {}

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

  QString _info = tr("Create remote action for <b>%1</b> with Order Id: %2")
                      .arg(_main.value("buyer").toString(), p_order_id);
  qDebug() << Q_FUNC_INFO << _info;

  return AntiquaCRM::ProviderActionDialog::exec();
}
