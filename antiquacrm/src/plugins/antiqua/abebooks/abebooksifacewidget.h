// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSIFACEWIDGET_PLUGIN_H
#define ABEBOOKSIFACEWIDGET_PLUGIN_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class ANTIQUACORE_EXPORT AbeBooksIfaceWidget final
    : public Antiqua::InterfaceWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  Antiqua::PurchaserOrderTable *m_orderTable;
  const QVariant tableData(int row, int column);
  const QJsonDocument customerRequest(const QJsonObject &object);
  void parseAddressBody(const QString &section, const QJsonObject &object);

public Q_SLOTS:
  void createCustomerDocument();
  void setContent(const QJsonDocument &);
  void createOrderRequest(const QString &bfId);

public:
  AbeBooksIfaceWidget(const QString &widgetId, QWidget *parent = nullptr);
  void setCustomerId(int customerId);
  const QMap<QString, QString> fieldTranslate() const;
  const ProviderOrder getProviderOrder();
};

#endif // ABEBOOKSIFACEWIDGET_PLUGIN_H
