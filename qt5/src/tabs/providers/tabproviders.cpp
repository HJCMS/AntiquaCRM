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
      // Fehler Meldungen
      connect(mpl,
              SIGNAL(sendErrorResponse(QMessageBox::Icon, const QString &)),
              SLOT(pluginErrorResponse(QMessageBox::Icon, const QString &)));
      // QueryFinished
      connect(mpl, SIGNAL(sendQueryFinished()), SLOT(pluginQueryFinished()));
      plugins.append(mpl);
    }
  }
  return (plugins.size() > 0);
}

void TabProviders::createProviderAction() {
  QString orderId = sender()->objectName();

  QString sql("SELECT pr_name, pr_order_data FROM provider_orders");
  sql.append(" WHERE pr_order='" + orderId + "';");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() != 1) {
    qWarning("Missing data for OrderId:(%s).", qPrintable(orderId));
    return;
  }

  q.next();
  QString provider = q.value("pr_name").toString();
  QByteArray data = q.value("pr_order_data").toByteArray();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  QJsonObject obj = doc.object();
  if (obj.isEmpty()) {
    qWarning("Missing Json for createProviderAction");
    return;
  }

  // Plugin Update Dialogeingabe suchen.
  QListIterator<AntiquaCRM::APluginInterface *> it(plugins);
  while (it.hasNext()) {
    AntiquaCRM::APluginInterface *m_pr = it.next();
    if (m_pr != nullptr) {
      if (m_pr->configProvider() == provider.toLower()) {
        AntiquaCRM::UpdateDialog *dialog =
            qobject_cast<AntiquaCRM::UpdateDialog *>(m_pr->updateDialog(this));
        connect(dialog, SIGNAL(sendPluginAction(const QJsonObject &)), m_pr,
                SLOT(orderUpdateAction(const QJsonObject &)));
        connect(m_pr, SIGNAL(sendActionFinished(const QString &)), dialog,
                SLOT(statusMessage(const QString &)));
        dialog->exec(orderId, obj);
        break;
      }
    }
  }
}

void TabProviders::pluginErrorResponse(QMessageBox::Icon type,
                                       const QString &message) {
  QStringList info;
  QString provider = sender()->objectName();
  switch (type) {
  case QMessageBox::Warning: // Warnung wird ausgegeben!
    info << "WARNING" << provider << message;
    break;

  case QMessageBox::Critical: // Schwehrwiegender Fehler ist aufgetreten!
    info << "FATAL" << provider << message;
    break;

  case QMessageBox::Information: // Nur für das Protokollieren vorgesehen!
    info << "LOGGING" << provider << message;
    break;

  default: // Standard Info
    info << "NORMAL" << provider << message;
    break;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "TODO" << info.join(" ");
#else
  Q_UNUSED(info);
#endif
}

void TabProviders::pluginQueryFinished() {
  QString info(tr("Request from") + " ");
  info.append(sender()->objectName());
  info.append(tr("finished."));
  sendStatusMessage(info);
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
    //#ifdef ANTIQUA_DEVELOPEMENT
    //  qDebug() << Q_FUNC_INFO << sql << Qt::endl << jObj;
    //#endif
    ProvidersOrderPage *page = new ProvidersOrderPage(jObj, m_pages);
    connect(page, SIGNAL(sendOpenProviderDialog()),
            SLOT(createProviderAction()));
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
  firstStart = loadPlugins();
}

void TabProviders::createSearchQuery(const QString &query) { Q_UNUSED(query); }

void TabProviders::createNewEntry(){/* unused */};

void TabProviders::openEntry(qint64 customerId) { Q_UNUSED(customerId); };

void TabProviders::onEnterChanged() {
  if (!firstStart && (plugins.size() < 1))
    openStartPage();

  m_treeWidget->loadUpdate();
}

TabProviders::~TabProviders() {
  if (m_pages->count() > 0)
    m_pages->clear();

  if (plugins.size() > 0) {
    for (int i = 0; i < plugins.size(); i++) {
      AntiquaCRM::APluginInterface *page =
          qobject_cast<AntiquaCRM::APluginInterface *>(plugins.at(i));
      page->deleteLater();
    }
    plugins.clear();
  }
}
