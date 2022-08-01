// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "interface.h"
#include "purchaseoverview.h"

#include <QVBoxLayout>

namespace Antiqua {

InterfaceWidget::InterfaceWidget(const QString &orderId, QWidget *parent)
    : QScrollArea{parent}, p_widgetId(orderId) {
  setWidgetResizable(true);
  setObjectName(orderId);
  setWindowTitle(orderId);

  m_order = new Antiqua::PurchaseOverview(orderId, this);
  setWidget(m_order);

  // Weiterleitung Artikel öffnen
  connect(m_order, SIGNAL(inspectArticle(int)), this, SIGNAL(openArticle(int)));
  // Weiterleitung Artikelnummern prüfen
  connect(m_order, SIGNAL(checkOrders()), this, SLOT(readCurrentArticleIds()));
  connect(m_order, SIGNAL(checkCustomer()), this, SLOT(checkCustomerClicked()));
  connect(m_order, SIGNAL(sendUpdateOrderStatus(Antiqua::PaymentStatus)), this,
          SLOT(providerOrderUpdateStatus(Antiqua::PaymentStatus)));
}

void InterfaceWidget::setProviderName(const QString &name) {
  providerName = name;
  emit providerChanged();
}

const QString InterfaceWidget::getProviderName() { return providerName; }

void InterfaceWidget::setOrderId(const QString &id) {
  orderId = id;
  emit providerChanged();
}

const QString InterfaceWidget::getOrderId() { return orderId; }

void InterfaceWidget::setOrderExists(int oId) {
  if (oId > 0) {
    orderExists = oId;
    emit orderExistsChanged();
  }
}

int InterfaceWidget::getOrderExists() { return orderExists; }

const QString InterfaceWidget::sqlParam(const QString &attribute) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.key() == attribute)
      return fi.value();
  }
  return QString();
}

const QString InterfaceWidget::apiParam(const QString &value) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.value() == value)
      return fi.key();
  }
  return QString();
}

const QString InterfaceWidget::stripString(const QVariant &val) const {
  QString buf(val.toString());
  buf.replace("'", "`");
  buf = buf.trimmed();
  if (buf == tr("Please select") || buf == "Please select") {
    qInfo("Discarded invalid input");
    return QString();
  }
  return buf;
}

const QJsonValue InterfaceWidget::getString(const QString &objName) {
  QString data = m_order->getValue(objName).toString();
  return QJsonValue(stripString(data));
}

const QJsonValue InterfaceWidget::getNumeric(const QString &objName) {
  int data = m_order->getValue(objName).toInt();
  return QJsonValue(data);
}

const QJsonValue InterfaceWidget::getPrice(const QString &objName) {
  double data = m_order->getValue(objName).toDouble();
  return QJsonValue(data);
}

const ProviderOrder InterfaceWidget::getProviderOrder() {
  ProviderOrder order;
  order.setProvider(providerName);
  order.setProviderId(orderId);
  int cid = m_order->getCustomerId();
  // qDebug() << Q_FUNC_INFO << providerName << orderId << cid;
  if (cid < 1) {
    order.setCustomerId(-1);
    qWarning("Missing Customer Id");
    return order;
  }
  order.setCustomerId(cid);
  int col = 1; /**< ArticleId Cell */
  QStringList ids;
  for (int r = 0; r < m_order->getTableCount(); r++) {
    QString aid = m_order->getTableData(r, col).toString();
    if (!aid.isEmpty())
      ids.append(aid);
  }
  order.setArticleIds(ids);
  return order;
}

}; // namespace Antiqua
