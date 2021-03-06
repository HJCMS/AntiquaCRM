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
  QHash<QString, QString> p_customer;
  QDomDocument p_currentDocument;

  const QJsonDocument customerRequest(const QJsonObject &object);

  void parseAddressBody(const QString &, const QJsonObject &);

private Q_SLOTS:
  void checkCustomerClicked();
  void readCurrentArticleIds();

public Q_SLOTS:
  void createCustomerDocument();
  void setContent(const QJsonDocument &){/* unused */};
  void setXmlContent(const QDomDocument &);
  void createOrderRequest();

public:
  AbeBooksIfaceWidget(const QString &orderId, QWidget *parent = nullptr);
  void setCustomerId(int customerId);
  const QString purchaseType(const QString &key) const;
  const QMap<QString, QString> fieldTranslate() const;
};

#endif // ABEBOOKSIFACEWIDGET_PLUGIN_H
