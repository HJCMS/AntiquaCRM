// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "myicontheme.h"
#include "providerspageview.h"
#include "providersstatements.h"
#include "providerstoolbar.h"
#include "providerstreeview.h"

#include <QDebug>
#include <QMessageBox>
#include <QSplitter>
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

  m_sql = new HJCMS::SqlCore(this);
  m_sql->setObjectName("sql_inventory_providers");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_providers_layout");
  layout->setContentsMargins(2, 2, 2, 2);

  QSplitter *m_splitter = new QSplitter(this);
  m_splitter->setObjectName("inventory_providers_splitter");
  m_splitter->setOrientation(Qt::Horizontal);
  layout->addWidget(m_splitter);

  m_pageView = new ProvidersPageView(m_splitter);
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

  connect(m_toolBar, SIGNAL(s_customerAction()), this, SLOT(openTableView()));
  connect(m_toolBar, SIGNAL(s_refresh()), this, SLOT(searchConvert()));
  connect(m_toolBar, SIGNAL(s_createOrder()), this, SLOT(createEditOrders()));
  connect(m_listView, SIGNAL(s_queryProvider(const QString &)), this,
          SLOT(queryProviderPage(const QString &)));
  connect(m_listView, SIGNAL(s_queryOrder(const QString &, const QString &)),
          this, SLOT(queryOrder(const QString &, const QString &)));
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

void InventoryProviders::openTableView() {
  int cid = -1;
  Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
  if (tab != nullptr) {
    cid = tab->getCustomerId();
    if (cid > 0) {
      createEditCustomer(cid);
      return;
    }
    tab->createCustomerDocument();
  }
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

void InventoryProviders::queryProviderPage(const QString &provider)
{
  if (tabExists(provider))
    return;

  QListIterator<Antiqua::Interface *> it(p_iFaces);
  while (it.hasNext()) {
    Antiqua::Interface *iFace = it.next();
    if (iFace->objectName() == provider) {
      Antiqua::ProviderWidget *w = iFace->providerWidget(provider, m_pageView);
      m_pageView->addMainPage(w, provider);
      return;
    }
  }
  qDebug() << Q_FUNC_INFO << "TODO" << provider;
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
      connect(w, SIGNAL(checkCustomer(const QJsonDocument &)), this,
              SLOT(createQueryCustomer(const QJsonDocument &)));
      connect(w, SIGNAL(createCustomer(const QJsonDocument &)), this,
              SLOT(createNewCustomer(const QJsonDocument &)));
      connect(w, SIGNAL(s_checkArticles(QList<int> &)), this,
              SLOT(checkArticleExists(QList<int> &)));
      m_pageView->addPage(w, orderId);
      /**
       * @warning tabExists() @b MUSS UNBEDINGT vor createOrderRequest()
       * aufgerufen werden! Ansonsten wird die Antwort an den Slot
       * createQueryCustomer() das falsche Tab abrufen und die Daten falsch
       * einfügen!
       */
      if (tabExists(orderId)) {
        w->createOrderRequest(orderId);
      }
    }
  }
}

void InventoryProviders::createEditCustomer(int cid) {
  m_toolBar->enableOrderButton(true);
  if (cid > 0) {
    customerId = cid;
    emit openEditCustomer(customerId);
  } else {
    customerId = -1;
  }
}

void InventoryProviders::createNewCustomer(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  QStringList params;
  QStringList values;

  QJsonObject obj = doc.object();
  QSqlRecord rec = m_sql->record("customers");
  QJsonObject::iterator it; // Iterator
  for (it = obj.begin(); it != obj.end(); ++it) {
    QJsonValue val = it.value();
    if (rec.contains(it.key()) && !val.toString().isEmpty()) {
      QSqlField field = rec.field(it.key());
      params.append(field.name());
      if (field.type() == QVariant::Int) {
        values.append(val.toString());
      } else {
        values.append("'" + val.toString() + "'");
      }
    }
  }

  QString sql("INSERT INTO customers (");
  sql.append(params.join(","));
  sql.append(") VALUES (");
  sql.append(values.join(","));
  sql.append(") RETURNING c_id;");

  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sql;
  }

  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    if (q.value("c_id").toInt() > 0) {
      customerId = q.value("c_id").toInt();
      Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
      if (tab != nullptr) {
        tab->setCustomerId(customerId);
        m_toolBar->enableOrderButton(true);
        emit openEditCustomer(customerId);
      }
    }
  } else {
    QString errors = m_sql->lastError();
    if (!errors.isEmpty()) {
      qDebug() << Q_FUNC_INFO << errors;
    }
  }
}

void InventoryProviders::createQueryCustomer(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  QString sql = queryCustomerExists(QJsonValue(doc["c_firstname"]).toString(),
                                    QJsonValue(doc["c_lastname"]).toString(),
                                    QJsonValue(doc["c_postalcode"]).toString(),
                                    QJsonValue(doc["c_location"]).toString());

  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sql << doc;
  }

  QString orderid = doc.object().value("orderid").toString();
  if (!tabExists(orderid))
    return;

  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    if (q.value("c_id").toInt() > 0) {
      customerId = q.value("c_id").toInt();
      Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
      if (tab != nullptr) {
        qDebug() << "Customer Found:" << customerId << tab->objectName();
        tab->setCustomerId(customerId);
        m_toolBar->statusMessage(tr("customer found in database!"));
        m_toolBar->enableOrderButton(true);
      }
    }
  } else {
    QString errors = m_sql->lastError();
    if (!errors.isEmpty()) {
      qDebug() << Q_FUNC_INFO << errors;
    }
    m_toolBar->enableOrderButton(false);
    m_toolBar->statusMessage(tr("customer not exits!"));
  }
}

void InventoryProviders::checkArticleExists(QList<int> &list) {
  for (int i = 0; i < list.size(); i++) {
    QString aid = QString::number(list[i]);
    QSqlQuery q = m_sql->query(queryArticleExists(aid));
    if (q.size() > 0) {
      q.next();
      qDebug() << Q_FUNC_INFO << "Found Article:" << q.value(0);
      m_toolBar->statusMessage(tr("article exits!"));
    } else {
      QString info(tr("this"));
      info.append(" " + aid + " ");
      info.append(tr("article is not available!"));
      QMessageBox::warning(this, tr("Article"), info);
    }
  }
}

void InventoryProviders::createEditOrders() {
  if (customerId < 1)
    return;

  Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
  if (tab != nullptr && tab->getCustomerId() == customerId) {
    m_toolBar->statusMessage(tr("open order editor"));
    emit createOrder(tab->getProviderOrder());
  } else {
    m_toolBar->statusMessage(tr("current tab and customer id not equal"));
  }
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
