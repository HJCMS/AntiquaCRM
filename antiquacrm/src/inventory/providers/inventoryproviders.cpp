// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "buchfreund.h"
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

  p_providerList = QStringList({"Buchfreund", "ZVAB", "AbeBooks"});
  m_listView->addProviders(p_providerList);

  // BEGIN TESTING
  Buchfreund *bf = new Buchfreund("BF-2249986", this);
  addTab(bf);
  bf->testContent();
  connect(bf, SIGNAL(openCustomer(int)), this, SLOT(createEditCustomer(int)));
  // END

  connect(m_toolBar, SIGNAL(s_customerView()), this, SLOT(checkCustomer()));
  connect(m_toolBar, SIGNAL(s_refresh()), this, SLOT(initProviders()));
  connect(m_toolBar, SIGNAL(s_createOrder()), this, SLOT(createEditOrders()));
  connect(m_listView, SIGNAL(s_queryOrder(const QString &, const QString &)),
          this, SLOT(queryOrder(const QString &, const QString &)));
}

void InventoryProviders::checkCustomer() {
  QMetaObject::invokeMethod(m_pageView->currentWidget(), "checkCustomer",
                            Qt::DirectConnection);
}

bool InventoryProviders::initProviders() {
  // Buchfreund
  Buchfreund::queryListEntries(this);

  return true;
}

bool InventoryProviders::addTab(QWidget *w) {
  QString identity = w->objectName();
  for (int i = 0; i < m_pageView->count(); i++) {
    if (m_pageView->widget(i)->objectName() == identity) {
      m_pageView->setCurrentIndex(i);
      return false;
    }
  }
  m_pageView->addTab(w, myIcon("edit_group"), identity);
  return true;
}

void InventoryProviders::openEditor(const QString &customerId) {
  qDebug() << Q_FUNC_INFO << "TODO" << customerId;
  /**
   * @todo Abstrakte Klasse erstellen damit ich die Daten neutraler abfangen
   * kann!
   */
}

void InventoryProviders::readBFOrders(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  int errors = QJsonValue(doc["error"]).toBool();
  if (!errors) {
    QJsonArray array = QJsonValue(doc["response"]).toArray();
    for (int i = 0; i < array.count(); i++) {
      QJsonObject obj = array[i].toObject();
      QString id = obj["id"].toString();
      QString ds = obj["datum"].toString();
      QDateTime dt = QDateTime::fromString(ds, BF_DATE_FORMAT);
      m_listView->addOrder("Buchfreund", id, dt);
    }
  }
}

void InventoryProviders::queryOrder(const QString &provider,
                                    const QString &orderId) {
  if (!p_providerList.contains(provider))
    return;

  if (provider == "Buchfreund") {
    Buchfreund *bf = new Buchfreund(orderId, this);
    connect(bf, SIGNAL(openCustomer(int)), this, SLOT(createEditCustomer(int)));
    addTab(bf);
    return;
  }

  qDebug() << Q_FUNC_INFO << "TODO" << provider << orderId;
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

void InventoryProviders::onEnterChanged() {
  if (firstStart)
    return;

  firstStart = true; // initProviders();
}
