// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "providerselectcustomer.h"
#include "providerspageview.h"
#include "providersstatements.h"
#include "providerstoolbar.h"
#include "providerstreeview.h"
// Utils
#include "eucountries.h"
#include "genderbox.h"

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
  m_splitter->insertWidget(1, m_listView);

  m_toolBar = new ProvidersToolBar(this);
  layout->addWidget(m_toolBar);

  m_splitter->setStretchFactor(0, 65);
  m_splitter->setStretchFactor(1, 35);
  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_toolBar, SIGNAL(s_refresh()), this, SLOT(searchConvert()));
  connect(m_toolBar, SIGNAL(s_createOrder()), this, SLOT(createEditOrders()));

  connect(m_listView, SIGNAL(s_queryOrder(const QString &, const QString &)),
          this, SLOT(queryOrder(const QString &, const QString &)));

  // Alte Nachrichten vor dem Wechsel entfernen.
  connect(m_pageView, SIGNAL(orderPageChanged()), m_toolBar,
          SLOT(clearStatusMessage()));
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

bool InventoryProviders::createStatusArticle(int articleId, int count) {
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
    return true;
  } else {
    m_toolBar->enableOrderButton(false);
    info.append(tr("is not available!"));
    QMessageBox::warning(this, tr("Article"), info);
    return false;
  }
  return false;
}

bool InventoryProviders::validInterfaceProperties(
    Antiqua::InterfaceWidget *ifw) {
  if (ifw == nullptr)
    return false;

  if (ifw->getOrderId().isEmpty() || ifw->getProviderName().isEmpty()) {
    m_toolBar->statusMessage(tr("Missing required Shipping Properties!"));
    return false;
  }
  return true;
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

void InventoryProviders::queryOrder(const QString &provider,
                                    const QString &orderId) {
  if (p_providerList.count() > 0 && !p_providerList.contains(provider))
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
      connect(w, SIGNAL(checkArticleIds(QList<int> &)), this,
              SLOT(checkArticleExists(QList<int> &)));
      connect(w, SIGNAL(errorResponse(int, const QString &)), this,
              SLOT(pluginError(int, const QString &)));
      connect(w, SIGNAL(openArticle(int)), this,
              SLOT(checkOpenEditArticle(int)));

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

  EUCountries countries;
  QJsonObject obj = doc.object();
  QSqlRecord rec = m_sql->record("customers");
  QJsonObject::iterator it; // Iterator
  for (it = obj.begin(); it != obj.end(); ++it) {
    QJsonValue val = it.value();
    if (rec.contains(it.key()) && !val.toString().isEmpty()) {
      QSqlField field = rec.field(it.key());
      // Ein Land ist zwingend erforderlich!
      if (field.name() == "c_country") {
        params.append(field.name());
        // ISO-2 Country Code
        QString cc(val.toString().trimmed());
        if (cc.length() == 2) {
          QString cName = countries.name(cc);
          values.append("'" + (cName.isEmpty() ? tr("Europe") : cName) + "'");
        } else {
          values.append("'" + tr("Europe") + "'");
        }
        continue;
      } else if (field.name() == "c_country_name" &&
                 !params.contains("c_country")) {
        QString cName(val.toString().trimmed());
        if (!cName.isEmpty()) {
          QString cc = countries.countryCode(cName);
          params.append("c_country");
          if (!cc.isEmpty()) {
            values.append("'" + cName + "'");
          } else {
            values.append("'" + tr("Europe") + "'");
          }
        }
        continue;
      }
      // Suche Geschlecht
      if (field.name() == "c_gender") {
        params.append(field.name());
        int i = Gender::indexByString(val.toString());
        values.append(QString::number(i));
        continue;
      }
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
        m_toolBar->statusMessage(tr("new customer created!"));
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
  if (!tabExists(orderid)) {
    qWarning("Missing OrderId");
    return;
  }

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
    // Es wurde kein Kunde gefunden, dann jetzt anlegen ...
    openTableView();
    return;
  }

  /**
   * Wenn mehr als ein Kunde gefunden wurde, eine Auswahlmöglichkeit anbieten!
   */
  if (cidList.size() > 1) {
    ProviderSelectCustomer *dialog = new ProviderSelectCustomer(this);
    if (dialog->exec(cidList) == QDialog::Accepted) {
      selected_cid = dialog->getSelectedCustomer();
    } else {
      qInfo("aboart costumer selection");
      return;
    }
    dialog->deleteLater();
  } else {
    selected_cid = cidList.first();
  }
  cidList.clear();

  /**
   * Die Id des ausgewählten Kunden einfügen!
   */
  if (selected_cid > 0) {
    Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
    if (tab != nullptr) {
      current_cid = selected_cid;
      tab->setCustomerId(selected_cid);
      m_toolBar->statusMessage(tr("customer found in database!"));
    }
    // aussteigen
    return;
  }
}

void InventoryProviders::checkArticleExists(QList<int> &list) {
  /**
   * Versuche heraus zu finden ob eine Bestellung mit diesen Daten bereits
   * existiert!
   * Wenn ja, dann mit setOrderExists(o_id) die inventory_orders Id setzen.
   */
  Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
  if (!validInterfaceProperties(tab))
    return;

  // Nehme aktuelle Provider Daten
  ProviderOrder pd = tab->getProviderOrder();
  // Ist eine Kundennummer vorhanden, dann sofort eine Prüfung auf bestehende
  // Aufträge erstellen.
  int c_id = pd.customerId();
  if (c_id > 0) {
    QString sql = queryFindExistingOrders(pd.provider(), pd.providerId(), c_id);
    QSqlQuery q = m_sql->query(sql);
    int o_check_id = 0;
    if (q.size() > 0) {
      while (q.next()) {
        o_check_id = 0;
        if (q.value("id").toInt() > 0) {
          o_check_id = q.value("id").toInt();
          break;
        }
      }
    }
    if (o_check_id > 0) {
      QString info_id = QString::number(o_check_id);
      QMessageBox::warning(
          this, tr("Article Check"),
          tr("Order witdh Id: %1 already exists!").arg(info_id));
      m_toolBar->enableOrderButton(false);
      return;
    }
  }

  // Jetzt die Artikel Liste prüfen
  for (int i = 0; i < list.size(); i++) {
    int aid = list[i];
    QSqlQuery q = m_sql->query(queryArticleCount(aid));
    if (q.size() > 0) {
      q.next();
      int count = q.value("count").toInt();
      if (!createStatusArticle(aid, count)) {
        // Wenn nicht verfügbar kann hier ausgestiegen werden!
        return;
      }
    }
  }

  // Wenn die Kunden Nummer nicht gesetzt oder nicht Identisch ist,
  // hier abbrechen!
  if (c_id < 1 || c_id != current_cid) {
    QString info = tr("Buyer was not found in the customer register or the "
                      "current customer number does not match. The system "
                      "requires a valid customer number for a smooth process.");
    QMessageBox::warning(this, tr("Customer"), info);
    return;
  }

  // Alles ok ...
  m_toolBar->enableOrderButton(true);
}

void InventoryProviders::checkOpenEditArticle(int aid) {
  if (aid < 1)
    return;

  QSqlQuery q = m_sql->query(queryFindArticleSection(aid));
  if (q.size() > 0) {
    q.next();
    QString section = q.value("section").toString();
    if (!section.isEmpty())
      emit openEditArticle(aid, section);
  }
}

void InventoryProviders::createEditOrders() {
  if (current_cid < 1)
    return;

  Antiqua::InterfaceWidget *tab = m_pageView->currentPage();
  if (!validInterfaceProperties(tab))
    return;

  // Besitzt dieser Auftrag schon eine "inventory_orders:o_id" ?
  if (tab->getOrderExists() > 0) {
    QMessageBox::warning(this, tr("Notice"),
                         tr("An Order with this Entries already exists!"));
    return;
  }

  if (tab->getCustomerId() != current_cid) {
    m_toolBar->statusMessage(tr("Invalid Customer Id!"));
    m_toolBar->enableOrderButton(false);
    return;
  }

  ProviderOrder pData = tab->getProviderOrder();
  if (pData.customerId() > 0 && pData.customerId() == current_cid) {
    // Um Doppelte Einträge zu vermeiden, abschalten!
    m_toolBar->enableOrderButton(false);
    emit createOrder(pData);
  } else {
    m_toolBar->statusMessage(tr("Invalid Shipping Properties!"));
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

  int count = m_listView->ordersCount();
  if (count > 0) {
    QString info(tr("Current order count ") + ": ");
    info.append(QString::number(count));
    m_toolBar->statusMessage(info);
  }
}

void InventoryProviders::hasResponsed(bool errors) {
  if (errors) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "InventoryProviders - plugin answered with an error!";
#endif
    m_toolBar->warningMessage(tr("an error occurred"));
  } else {
    m_toolBar->statusMessage(tr("successfully"));
  }
}

void InventoryProviders::pluginError(int code, const QString &msg) {
  if (code == 0)
    m_toolBar->statusMessage(msg);

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << code << msg;
#endif
}

void InventoryProviders::onEnterChanged() {
  if (firstStart)
    return;

  firstStart = loadInterfaces();
}

bool InventoryProviders::updateArticleCount(int articleId, int count) {
#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Update Article Count disabled in Developement:\n"
        " InventoryProviders::updateArticleCount(%s,%s)",
        qPrintable(QString::number(articleId).toLocal8Bit()),
        qPrintable(QString::number(count).toLocal8Bit()));
  return false;
#endif

  if (!firstStart)
    onEnterChanged();

  if (p_iFaces.count() < 1)
    return false;

  QListIterator<Antiqua::Interface *> it(p_iFaces);
  while (it.hasNext()) {
    Antiqua::Interface *iface = it.next();
    if (iface != nullptr) {
      iface->updateArticleCount(articleId, count);
    }
  }
  return true;
}

bool InventoryProviders::updateProviderImage(int articleId) {
#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Update Provider Image disabled in Developement:\n"
        " InventoryProviders::updateProviderImage(%s)",
        qPrintable(QString::number(articleId).toLocal8Bit()));
  return false;
#endif

  if (!firstStart)
    onEnterChanged();

  if (p_iFaces.count() < 1)
    return false;

  QString imgData;
  QString sql = queryUploadImageData(articleId);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    if (q.value("id").toInt() == articleId) {
      imgData = q.value("data").toString();
    }
  } else {
    if (!m_sql->lastError().isEmpty())
      qDebug() << Q_FUNC_INFO << m_sql->lastError();

    imgData.clear();
    return false;
  }

  if (imgData.isEmpty())
    return false;

  int count = 0;
  QListIterator<Antiqua::Interface *> it(p_iFaces);
  while (it.hasNext()) {
    Antiqua::Interface *iface = it.next();
    if (iface != nullptr) {
      iface->uploadArticleImage(articleId, imgData);
      count++;
    }
  }
  return (count > 0);
}
