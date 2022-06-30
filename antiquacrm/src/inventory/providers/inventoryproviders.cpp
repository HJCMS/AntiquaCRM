// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "myicontheme.h"
#include "providerselectcustomer.h"
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
#ifndef Q_OS_WIN
  // Developement
  connect(m_listView, SIGNAL(s_queryProvider(const QString &)), this,
          SLOT(queryProviderPage(const QString &)));
#endif
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

void InventoryProviders::statusMessageArticle(int articleId, int count) {
  QString info(tr("Article"));
  info.append(" '");
  info.append(QString::number(articleId));
  info.append("' ");
  if (count > 0) {
    info.append(tr("exists with count"));
    info.append(" '");
    info.append(QString::number(count));
    info.append("'.");
    m_toolBar->statusMessage(info);
  } else {
    info.append(tr("is not available!"));
    QMessageBox::warning(this, tr("Article"), info);
  }
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
    connect(iface, SIGNAL(s_queryResponse(bool)), this,
            SLOT(hasResponsed(bool)));

    iface->queryMenueEntries();
    p_iFaces.append(iface);
  }
  return true;
}

void InventoryProviders::queryProviderPage(const QString &provider) {
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

  // qDebug() << Q_FUNC_INFO << provider << orderId;

  QListIterator<Antiqua::Interface *> it(p_iFaces);
  while (it.hasNext()) {
    Antiqua::Interface *iFace = it.next();
    if (iFace->objectName() == provider) {
      Antiqua::InterfaceWidget *w = iFace->addWidget(orderId, m_pageView);
      connect(w, SIGNAL(checkCustomer(const QJsonDocument &)), this,
              SLOT(createQueryCustomer(const QJsonDocument &)));
      connect(w, SIGNAL(createCustomer(const QJsonDocument &)), this,
              SLOT(createNewCustomer(const QJsonDocument &)));
      connect(w, SIGNAL(checkArticleIds(QList<int> &)), this,
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
  if (cid > 0) {
    current_cid = cid;
    emit openEditCustomer(current_cid);
  } else {
    current_cid = -1;
    m_toolBar->enableOrderButton(false);
  }
}

void InventoryProviders::createNewCustomer(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  QStringList params;
  QStringList values;

  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << doc;
  }

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
      current_cid = q.value("c_id").toInt();
      Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
      if (tab != nullptr) {
        tab->setCustomerId(current_cid);
        emit openEditCustomer(current_cid);
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

  int selected_cid = -1;
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

  QList<int> cidList;
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int id = q.value("c_id").toInt();
      if (id > 0) {
        cidList.append(id);
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

  /**
   * Wenn mehr als ein Kunde gefunden wurde, eine Auswahlmöglichkeit anbieten!
   */
  if (cidList.size() > 1) {
    ProviderSelectCustomer *dialog = new ProviderSelectCustomer(this);
    if (dialog->exec(cidList) == QDialog::Accepted) {
      QPair<int, QString> pair = dialog->getSelectedCustomer();
      selected_cid = pair.first;
    } else {
      qInfo("aboart costumer selection");
      return;
    }
    dialog->deleteLater();
  } else {
    selected_cid = cidList.first();
  }
  cidList.clear();

  /** Die Id des ausgewählten Kunden einfügen! */
  if (selected_cid > 0) {
    Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
    if (tab != nullptr) {
      current_cid = selected_cid;
      tab->setCustomerId(selected_cid);
      m_toolBar->statusMessage(tr("customer found in database!"));
    }
  }
}

void InventoryProviders::checkArticleExists(QList<int> &list) {
  bool exists = true;
  for (int i = 0; i < list.size(); i++) {
    int aid = list[i];
    QSqlQuery q = m_sql->query(queryArticleCount(aid));
    if (q.size() > 0) {
      q.next();
      int count = q.value("count").toInt();
      statusMessageArticle(aid, count);
      exists = (count > 0 && exists) ? true : false;
    }
  }
  m_toolBar->enableOrderButton(exists);
}

void InventoryProviders::createEditOrders() {
  if (current_cid < 1)
    return;

  Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
  if (tab != nullptr && tab->getCustomerId() == current_cid) {
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

void InventoryProviders::hasResponsed(bool errors) {
  if (errors) {
    qWarning("InventoryProviders - plugin answered with an error!");
    m_toolBar->statusMessage(tr("an error occurred"));
  } else {
    m_toolBar->statusMessage(tr("successfully"));
  }
}

void InventoryProviders::onEnterChanged() {
  if (firstStart)
    return;

  firstStart = loadInterfaces();
}

bool InventoryProviders::updateArticleCount(int articleId, int count) {
  if (!firstStart)
    onEnterChanged();

  if (p_iFaces.count() < 1)
    return false;

  QListIterator<Antiqua::Interface *> it(p_iFaces);
  while (it.hasNext()) {
    Antiqua::Interface *iface = it.next();
    if (iface != nullptr) {
      // qDebug() << "Update:" << iface->provider() << articleId << count;
      iface->updateArticleCount(articleId, count);
    }
  }
  return true;
}
