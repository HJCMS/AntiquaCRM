// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabproviders.h"
#include "providersorderpage.h"
#include "providerspageview.h"
#include "providerstreeview.h"

#include <AntiquaCRM>
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
  if (provider.isEmpty() || orderId.isEmpty())
    return;

  if (findPage(provider, orderId))
    return;

  QString sql("SELECT pr_order_data FROM provider_order_history");
  sql.append(" WHERE pr_name='" + provider + "' AND pr_order='");
  sql.append(orderId + "' AND pr_closed IS NULL;");
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
}

void TabProviders::openStartPage() {
  qDebug() << Q_FUNC_INFO << sender()->objectName();
  setCurrentIndex(0);
}

void TabProviders::createSearchQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << sender()->objectName();
}

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
