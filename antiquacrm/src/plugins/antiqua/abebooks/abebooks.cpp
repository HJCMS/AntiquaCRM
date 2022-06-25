// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"
#include "abebooksconfig.h"
#include "abebooksifacewidget.h"
#include "abebooksproviderwidget.h"
#include "abebooksrequester.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QPushButton>
#include <QStyle>
#include <QTabWidget>
#include <QTableWidget>
#include <QTime>
#include <QVBoxLayout>

void AbeBooks::responseImport(const QDomDocument &doc) {
  AbeBooksDocument xml(doc);
  QJsonArray senderArray;
  QDomNodeList n_list = xml.getPurchaseOrderList();
  for (int i = 0; i < n_list.count(); i++) {
    QDomNode n = n_list.at(i);
    if (n.hasAttributes()) {
      QDomElement e = n.toElement();
      if (!e.hasAttribute("id"))
        continue;

      QString id = e.attribute("id", "0");
      QJsonObject entry;
      entry.insert("id", id);
      QDateTime d = xml.getOrderDate(e);
      entry.insert("datum", QJsonValue(d.toString(ANTIQUA_DATETIME_FORMAT)));
      senderArray.append(entry);
    }
  }
  QJsonObject senderObject;
  senderObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  senderObject.insert("items", senderArray);
  QJsonDocument jsDoc(senderObject);
  emit listResponse(jsDoc);
}

bool AbeBooks::createInterface(QObject *parent) {
  if (parent) {
    m_handle = new QObject(parent);
    m_handle->setObjectName("abebooks_plugin");
    return true;
  }
  return false;
}

Antiqua::ProviderWidget *AbeBooks::providerWidget(const QString &widgetId,
                                                  QWidget *parent) {
  m_abeBooksProviderWidget = new AbeBooksProviderWidget(widgetId, parent);
  m_abeBooksProviderWidget->setObjectName(widgetId);
  return m_abeBooksProviderWidget;
}

Antiqua::InterfaceWidget *AbeBooks::addWidget(const QString &widgetId,
                                              QWidget *parent) {
  m_abeBooksIfaceWidget = new AbeBooksIfaceWidget(widgetId, parent);
  m_abeBooksIfaceWidget->setObjectName(widgetId);
  return m_abeBooksIfaceWidget;
}

const QString AbeBooks::provider() const { return QString(CONFIG_PROVIDER); }

const QString AbeBooks::configGroup() const { return QString(CONFIG_GROUP); }

void AbeBooks::queryMenueEntries() {
#ifdef ABEBOOKS_TESTMODE
  responseImport(testSources("abebooks_20220624_0808.xml"));
  return;
#endif
  AbeBooksRequester *req = new AbeBooksRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QDomDocument &)), this,
          SLOT(responseImport(const QDomDocument &)));

  req->queryList();
}

void AbeBooks::updateArticleCount(int articleId, int count) {}
