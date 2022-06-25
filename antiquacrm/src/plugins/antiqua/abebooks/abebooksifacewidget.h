// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSIFACEWIDGET_PLUGIN_H
#define ABEBOOKSIFACEWIDGET_PLUGIN_H

#include <QDomDocument>
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
  QDomDocument p_currentDocument;

  const QString stripString(const QVariant &val) const;

  const QVariant tableData(int row, int column);

  void setTableData(int row, int column, const QVariant &val);

  const QJsonValue getString(const QString &objName);
  const QJsonValue getNumeric(const QString &objName);
  const QJsonValue getPrice(const QString &objName);

  const QJsonDocument customerRequest(const QJsonObject &object);

  void parseAddressBody(const QString &, const QJsonObject &);

private Q_SLOTS:
  void readCurrentArticleIds();

public Q_SLOTS:
  void createCustomerDocument();
  void setContent(const QJsonDocument &){/* unused */};
  void setXmlContent(const QDomDocument &);
  void createOrderRequest(const QString &bfId);

public:
  AbeBooksIfaceWidget(const QString &widgetId, QWidget *parent = nullptr);
  void setCustomerId(int customerId);
  const QMap<QString, QString> fieldTranslate() const;
  const ProviderOrder getProviderOrder();
};

#endif // ABEBOOKSIFACEWIDGET_PLUGIN_H
