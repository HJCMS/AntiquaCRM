// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksifacewidget.h"
#include "abebooksconfig.h"

#include <QtWidgets>

AbeBooksIfaceWidget::AbeBooksIfaceWidget(const QString &widgetId,
                                         QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
  setWidgetResizable(true);
  setObjectName(widgetId);
  setWindowTitle(widgetId);

  QWidget *mainWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(mainWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addStretch(1);
  mainWidget->setLayout(layout);
  setWidget(mainWidget);
}

const QVariant AbeBooksIfaceWidget::tableData(int row, int column) {
  return QVariant();
}

void AbeBooksIfaceWidget::createCustomerDocument() {}

const QJsonDocument
AbeBooksIfaceWidget::customerRequest(const QJsonObject &object) {
  return QJsonDocument();
}

void AbeBooksIfaceWidget::parseAddressBody(const QString &section,
                                           const QJsonObject &object) {}

void AbeBooksIfaceWidget::setContent(const QJsonDocument &doc) {}

void AbeBooksIfaceWidget::createOrderRequest(const QString &bfId) {}

void AbeBooksIfaceWidget::setCustomerId(int customerId) {}

const QMap<QString, QString> AbeBooksIfaceWidget::fieldTranslate() const {
  QMap<QString, QString> m;
  return m;
}

const ProviderOrder AbeBooksIfaceWidget::getProviderOrder() {
  ProviderOrder o;
  return o;
}
