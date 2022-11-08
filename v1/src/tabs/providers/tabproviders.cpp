// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabproviders.h"
#include "providersorderpage.h"
#include "providerspageview.h"
#include "providerstreeview.h"

#include <QIcon>
#include <QLayout>
#include <QMessageBox>
#include <QCache>

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
  pageOneLayout->setContentsMargins(2, 2, 2, 2);
  // Create Splitter
  m_splitter = new QSplitter(m_mainPage);
  m_splitter->setObjectName("inventory_providers_splitter");
  m_splitter->setOrientation(Qt::Horizontal);
  // Order pages
  m_pages = new ProvidersPageView(m_mainPage);
  m_pages->setTabPosition(QTabWidget::South);
  m_splitter->insertWidget(0, m_pages);
  // Provider Orders list
  m_treeWidget = new ProvidersTreeView(m_mainPage);
  m_splitter->insertWidget(1, m_treeWidget);
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

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("providers_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  // m_editorWidget = new ProviderEditor(m_editorPage);
  // m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  // Tree
  connect(m_treeWidget,
          SIGNAL(sendQueryOrder(const QString &, const QString &)),
          SLOT(openOrderPage(const QString &, const QString &)));
  // Pages
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
  if (provider.isEmpty() || orderId.isEmpty() || findPage(provider, orderId))
    return;

  AntiquaCRM::AProviderOrder order(provider, "28049149");
  AntiquaCRM::ASharedCacheFiles cache;
  QJsonObject jObj = cache.getTempJson(order.md5sum());
  if(jObj.isEmpty())
    return;

  ProvidersOrderPage *page = new ProvidersOrderPage(jObj, m_pages);
  if (page->loadOrderDataset()) {
    m_pages->addPage(page, order.id());
    return;
  }

  qWarning("Can't load Provider Order data for %s:%s", qPrintable(provider),
           qPrintable(orderId));

  page->deleteLater();
}

void TabProviders::openStartPage() {
  qDebug() << Q_FUNC_INFO;
  setCurrentIndex(0);
}

void TabProviders::createSearchQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO;
}

void TabProviders::createNewEntry() { qDebug() << Q_FUNC_INFO; }

void TabProviders::openEntry(qint64 customerId) {
  if (customerId < 1)
    return;

  if (currentIndex() != 0) {
    QString info(tr("Cannot open this provider.") + "<br>");
    info.append(tr("Because the provider tab is not in overview mode."));
    info.append("<p>");
    info.append(tr("Please save and close all open provider first."));
    info.append("</p>");
    QMessageBox::information(this, tr("Providereditor"), info);
    return;
  }

  //  if (m_editorWidget->openEditEntry(articleId)) {
  //    setCurrentWidget(m_editorPage);
  //  }
  qDebug() << Q_FUNC_INFO << customerId;
}

void TabProviders::onEnterChanged() {
  if (!initialed) {
    setCurrentWidget(m_mainPage);
  }
}
