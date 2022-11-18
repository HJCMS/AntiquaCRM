// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabproviders.h"
#include "providersorderpage.h"
#include "providerspageview.h"
#include "providerstreeview.h"

#include <AntiquaCRM>
#include <QChar>
#include <QDataStream>
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
  m_rightFrame->setMaximumWidth(380);
  m_rightFrame->setMinimumWidth(150);
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
  m_splitter->setStretchFactor(0, 65);
  m_splitter->setCollapsible(1, false);
  m_splitter->setStretchFactor(1, 35);
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
  counter = 0;
  timerId = startTimer(1000, Qt::PreciseTimer);
}

void TabProviders::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timerId)
    return;

  --counter;
  if (counter <= 0) {
    m_treeWidget->loadUpdate();
    counter = resetCounter;
    return;
  }

  QTime ct(0, 0, 0);
  QString t_text = tr("Update (%1)").arg(ct.addSecs(counter).toString("m:ss"));
  btn_refreshTree->setText(t_text);
}

bool TabProviders::findPage(const QString &provider, const QString &orderId) {
  for (int p = 0; p < m_pages->count(); p++) {
    if (m_pages->widget(p)->windowTitle() == orderId) {
      m_pages->setCurrentIndex(p);
      return true;
    }
  }
  return false;
}

void TabProviders::openOrderPage(const QString &provider,
                                 const QString &orderId) {
  if (provider.isEmpty() || orderId.isEmpty())
    return;

  if (findPage(provider, orderId))
    return;

  QString sql("SELECT pr_order_data FROM provider_order_history");
  sql.append(" WHERE pr_name='" + provider + "' AND pr_order='");
  sql.append(orderId + "';");
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() == 1) {
    q.next();
    QByteArray order = q.value("pr_order_data").toString().toLocal8Bit();
    QJsonDocument doc = QJsonDocument::fromJson(order);
    if (doc.isEmpty() || doc.object().isEmpty()) {
      qWarning("Can't load Provider Order data for %s:%s", qPrintable(provider),
               qPrintable(orderId));
      return;
    }
    QJsonObject jObj = doc.object();
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

void TabProviders::openStartPage() { setCurrentIndex(0); }

void TabProviders::createSearchQuery(const QString &query) { Q_UNUSED(query); }

void TabProviders::createNewEntry(){/* unused */};

void TabProviders::openEntry(qint64 customerId) { Q_UNUSED(customerId); };

void TabProviders::onEnterChanged() { openStartPage(); }

TabProviders::~TabProviders() { killTimer(timerId); }
