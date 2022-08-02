// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKERIFACEWIDGET_PLUGIN_H
#define BOOKLOOKERIFACEWIDGET_PLUGIN_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class BooklookerRequester;

class ANTIQUACORE_EXPORT BooklookerIfaceWidget final
    : public Antiqua::InterfaceWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QHash<QString, QString> p_customer;
  BooklookerRequester *m_requester;

  QJsonDocument p_currentDocument;
  void setCurrentDocument(const QJsonObject &obj);
  const QJsonDocument customerRequest(const QJsonObject &object);
  void createOrderDataSet(const QJsonArray &array);
  void parseAddressBody(const QString &, const QJsonObject &);

private Q_SLOTS:
  void checkCustomerExists();
  void readCurrentArticleIds();

public Q_SLOTS:
  void createCustomerDocument();
  void setContent(const QJsonDocument &);
  void createOrderRequest();
  void createProviderOrderUpdate();

public:
  BooklookerIfaceWidget(const QString &widgetId, QWidget *parent = nullptr);
  void setOrderUpdateTypes();
  const QMap<QString, QString> fieldTranslate() const;
};

#endif // BOOKLOOKERIFACEWIDGET_PLUGIN_H
