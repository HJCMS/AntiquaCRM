// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "myicontheme.h"
#include "providersstatements.h"
#include "providerstoolbar.h"
#include "providerstreeview.h"

#include <QDebug>
#include <QStringList>
#include <QVBoxLayout>

// QJson
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>

InventoryProviders::InventoryProviders(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_providers");
  setWindowTitle(tr("Providers") + "[*]");
  setClosable(false);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_providers_layout");
  layout->setContentsMargins(2, 2, 2, 2);

  m_splitter = new QSplitter(this);
  m_splitter->setObjectName("inventory_providers_splitter");
  m_splitter->setOrientation(Qt::Horizontal);
  layout->addWidget(m_splitter);

  m_pageView = new QTabWidget(m_splitter);
  m_pageView->setObjectName("providers_pages");
  m_pageView->setTabPosition(QTabWidget::South);
  m_splitter->insertWidget(0, m_pageView);

  m_listView = new ProvidersTreeView(m_splitter);
  m_listView->setObjectName("providers_payments_list");
  m_listView->setTreeViewHeaders();
  m_splitter->insertWidget(1, m_listView);

  m_toolBar = new ProvidersToolBar(this);
  layout->addWidget(m_toolBar);

  m_splitter->setStretchFactor(0, 70);
  m_splitter->setStretchFactor(1, 30);
  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_toolBar, SIGNAL(s_customerView()), this, SLOT(checkCustomer()));
  connect(m_toolBar, SIGNAL(s_refresh()), this, SLOT(searchConvert()));
  connect(m_toolBar, SIGNAL(s_createOrder()), this, SLOT(createEditOrders()));
  connect(m_listView, SIGNAL(s_queryOrder(const QString &, const QString &)),
          this, SLOT(queryOrder(const QString &, const QString &)));
}

void InventoryProviders::openEditor(const QString &customerId) {
  qDebug() << Q_FUNC_INFO << "TODO" << customerId;
  /**
   * @todo Abstrakte Klasse erstellen damit ich die Daten neutraler abfangen
   * kann!
   */
}

bool InventoryProviders::tabExists(const QString &id) {
  for (int p = 0; p < m_pageView->count(); p++) {
    if (m_pageView->widget(p)->objectName() == id) {
      m_pageView->setCurrentIndex(p);
      return true;
    }
  }
  return false;
}

void InventoryProviders::searchConvert() {
  if (p_iFaces.count() > 0) {
    QListIterator<Antiqua::Interface *> it(p_iFaces);
    while (it.hasNext()) {
      Antiqua::Interface *iface = it.next();
      if (iface != nullptr)
        iface->queryMenueEntries();
    }
  }
}

void InventoryProviders::checkCustomer() {
  QMetaObject::invokeMethod(m_pageView->currentWidget(), "checkCustomer",
                            Qt::DirectConnection);
}

bool InventoryProviders::loadInterfaces() {
  p_providerList.clear();
  Antiqua::PluginLoader loader(this);
  QListIterator<Antiqua::Interface *> it(loader.pluginInterfaces(this));
  while (it.hasNext()) {
    Antiqua::Interface *iface = it.next();
    m_listView->addProvider(iface->objectName());
    p_providerList.append(iface->objectName());
    connect(iface, SIGNAL(listResponse(const QJsonDocument &)), this,
            SLOT(readOrderList(const QJsonDocument &)));

    p_iFaces.append(iface);
    iface->queryMenueEntries();
  }
  return true;
}

void InventoryProviders::queryOrder(const QString &provider,
                                    const QString &orderId) {
  if (!p_providerList.contains(provider))
    return;

  if (tabExists(orderId))
    return;

  QListIterator<Antiqua::Interface *> it(p_iFaces);
  while (it.hasNext()) {
    Antiqua::Interface *iFace = it.next();
    if (iFace->objectName() == provider) {
      Antiqua::InterfaceWidget *w = iFace->addWidget(orderId, m_pageView);
      connect(w, SIGNAL(openCustomer(int)), this,
              SLOT(createEditCustomer(int)));
      m_pageView->addTab(w, myIcon("edit_group"), orderId);
      return;
    }
  }
  qDebug() << Q_FUNC_INFO << "Missing:" << provider << orderId;
}

void InventoryProviders::createEditCustomer(int cid) {
  m_toolBar->enableOrderButton(true);
  qDebug() << Q_FUNC_INFO << cid;
  if (cid > 0) {
    customerId = cid;
    emit openEditCustomer(customerId);
  } else {
    customerId = -1;
  }
}

void InventoryProviders::createEditOrders() {
  if (customerId < 1)
    return;

  openEditor(QString::number(customerId));
}

void InventoryProviders::readOrderList(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  QString provider = QJsonValue(doc["provider"]).toString();
  QJsonArray array = QJsonValue(doc["items"]).toArray();
  for (int i = 0; i < array.count(); i++) {
    QJsonObject obj = array[i].toObject();
    QDateTime dt = QDateTime::fromString(obj["datum"].toString(), Qt::ISODate);
    m_listView->addOrder(provider, obj["id"].toString(), dt);
  }
}

void InventoryProviders::onEnterChanged() {
  if (firstStart)
    return;

  firstStart = loadInterfaces();
}
