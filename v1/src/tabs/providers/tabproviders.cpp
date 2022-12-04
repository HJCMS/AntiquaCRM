// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabproviders.h"
#include "providersorderpage.h"
#include "providerspageview.h"
#include "providerstreeview.h"

#include <QIcon>
#include <QLayout>
#include <QMessageBox>

TabProviders::TabProviders(QWidget *parent)
    : Inventory{"providers_tab", parent} {
  setObjectName("inventory_providers");
  setWindowTitle(tr("Providers"));
  setWindowIcon(getTabIcon("view_list"));

  // Begin "MainPage"
  m_mainPage = new QWidget(this);
  m_mainPage->setObjectName("providers_start_page");
  // SearchBar
  QVBoxLayout *pageOneLayout = new QVBoxLayout(m_mainPage);
  pageOneLayout->setContentsMargins(2, 2, 2, 0);
  // Create Splitter
  m_splitter = new QSplitter(m_mainPage);
  m_splitter->setObjectName("inventory_providers_splitter");
  m_splitter->setOrientation(Qt::Horizontal);
  // Order pages
  m_pages = new ProvidersPageView(m_mainPage);
  m_pages->setTabPosition(QTabWidget::South);
  m_splitter->insertWidget(0, m_pages);
  // Provider Orders list
  QFrame *m_rightFrame = new QFrame(m_mainPage);
  m_rightFrame->setMaximumWidth(420);
  m_rightFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  QVBoxLayout *m_rightFrameLayout = new QVBoxLayout(m_rightFrame);
  m_rightFrameLayout->setContentsMargins(0, 0, 0, 0);
  m_treeWidget = new ProvidersTreeView(m_rightFrame);
  m_rightFrameLayout->addWidget(m_treeWidget);
  btn_refreshTree = new QPushButton(m_rightFrame);
  btn_refreshTree->setText(tr("Update"));
  btn_refreshTree->setToolTip(tr("Button for update, Provider Tree."));
  btn_refreshTree->setIcon(QIcon("://icons/action_reload.png"));
  m_rightFrameLayout->addWidget(btn_refreshTree);
  m_rightFrameLayout->setStretch(0, 1);
  m_rightFrame->setLayout(m_rightFrameLayout);
  m_splitter->insertWidget(1, m_rightFrame);
  // Splitter settings
  m_splitter->setCollapsible(0, false);
  m_splitter->setStretchFactor(0, 80);
  m_splitter->setCollapsible(1, false);
  m_splitter->setStretchFactor(1, 20);
  pageOneLayout->addWidget(m_splitter);
  // Actionsbar
  QFrame *m_statusBar = new QFrame(m_mainPage);
  pageOneLayout->addWidget(m_statusBar);
  // close layout settings
  pageOneLayout->setStretch(0, 1);
  m_mainPage->setLayout(pageOneLayout);
  insertWidget(0, m_mainPage);
  // End "MainPage"

  // Signals:ProvidersTreeView
  connect(m_treeWidget,
          SIGNAL(sendQueryOrder(const QString &, const QString &)),
          SLOT(openOrderPage(const QString &, const QString &)));

  // Signals:RefreshButton
  connect(btn_refreshTree, SIGNAL(clicked()), m_treeWidget, SLOT(loadUpdate()));

  setCurrentIndex(0);
}

bool TabProviders::findPage(const QString &provider, const QString &orderId) {
  Q_UNUSED(provider);
  for (int p = 0; p < m_pages->count(); p++) {
    if (m_pages->widget(p)->windowTitle() == orderId) {
      m_pages->setCurrentIndex(p);
      return true;
    }
  }
  return false;
}

bool TabProviders::loadPlugins() {
  AntiquaCRM::APluginLoader *m_loader = new AntiquaCRM::APluginLoader(this);
  QListIterator<AntiquaCRM::APluginInterface *> it(
      m_loader->pluginInterfaces(this));
  while (it.hasNext()) {
    AntiquaCRM::APluginInterface *mpl = it.next();
    if (mpl != nullptr) {
      // OrderUpdateActions
      connect(this, SIGNAL(sendPluginOperation(const QJsonObject &)), mpl,
              SLOT(orderUpdateAction(const QJsonObject &)));
      // Fehler Meldungen
      connect(mpl,
              SIGNAL(sendErrorResponse(AntiquaCRM::Message, const QString &)),
              SLOT(pluginErrorResponse(AntiquaCRM::Message, const QString &)));
      // QueryFinished
      connect(mpl, SIGNAL(sendQueryFinished()), SLOT(pluginQueryFinished()));
      plugins.append(mpl);
    }
  }
  return (plugins.size() > 0);
}

void TabProviders::pluginErrorResponse(AntiquaCRM::Message, const QString &) {
  qDebug() << Q_FUNC_INFO << sender()->objectName();
}

void TabProviders::pluginQueryFinished() {
  qDebug() << Q_FUNC_INFO << sender()->objectName();
}

void TabProviders::openOrderPage(const QString &provider,
                                 const QString &orderId) {
  if (provider.isEmpty() || orderId.isEmpty())
    return;

  if (findPage(provider, orderId))
    return;

  qint64 customerId = -1;
  QString sql("SELECT pr_customer_id, pr_order_data FROM provider_orders");
  sql.append(" WHERE pr_name='" + provider + "' AND pr_order='");
  sql.append(orderId + "';");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() == 1) {
    q.next();
    customerId = q.value("pr_customer_id").toInt();
    QByteArray order = q.value("pr_order_data").toString().toLocal8Bit();
    QJsonDocument doc = QJsonDocument::fromJson(order);
    if (doc.isEmpty() || doc.object().isEmpty()) {
      qWarning("Can't load Provider Order data for %s:%s", qPrintable(provider),
               qPrintable(orderId));
      return;
    }
    QJsonObject jObj = doc.object();
    jObj.insert("c_id", customerId);
    ProvidersOrderPage *page = new ProvidersOrderPage(jObj, m_pages);
    if (page->loadOrderDataset()) {
      int index = m_pages->addPage(page, orderId);
      m_pages->setCurrentIndex(index);
    }
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }
#endif
}

void TabProviders::openStartPage() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  setCurrentIndex(0);
  m_treeWidget->loadUpdate();

  if (plugins.size() < 1)
    firstStart = loadPlugins();
}

void TabProviders::createSearchQuery(const QString &query) { Q_UNUSED(query); }

void TabProviders::createNewEntry(){/* unused */};

void TabProviders::openEntry(qint64 customerId) { Q_UNUSED(customerId); };

void TabProviders::onEnterChanged() {
  if (firstStart)
    return;

  openStartPage();
}
