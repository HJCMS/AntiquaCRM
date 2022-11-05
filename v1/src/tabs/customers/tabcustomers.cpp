// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabcustomers.h"
#include "customerseditor.h"
#include "customerssearchbar.h"
#include "customersstatusbar.h"
#include "customerstableview.h"

#include <QIcon>
#include <QLayout>
#include <QMessageBox>

TabCustomers::TabCustomers(QWidget *parent)
    : Inventory{"customers_tab", parent} {
  setObjectName("inventory_customers");
  setWindowTitle(tr("Customers"));
  setWindowIcon(getTabIcon("groups"));

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);
  m_searchBar = new CustomersSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);
  m_table = new CustomersTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_statusBar = new CustomersStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("customers_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new CustomersEditor(m_editorPage);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  // Signals
  // maintable
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCurrentId(qint64)),
          SIGNAL(sendIdToOrder(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendResultExists(bool)), m_statusBar,
          SLOT(setCreateButtonEnabled(bool)));

  connect(m_table, SIGNAL(sendDeleteEntry(qint64)),
          SLOT(setDeleteCustomer(qint64)));

  // editor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // searchbar
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));

  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  // statusbar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabCustomers::setDeleteCustomer(qint64 customerId) {
  QString info = tr("Do you really want to delete this customer?");
  info.append("<p>");
  info.append(tr("This process is irreversible!"));
  info.append("</p><p>");
  info.append(tr("In addition, the system will refuse to delete if an order "
                 "exists for this customer!"));
  info.append("</p>");

  QString title = tr("Delete Customer");
  int ret = QMessageBox::question(this, title, info);
  if (ret == QMessageBox::No)
    return;

  QString c_id = QString::number(customerId);
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString sql("DELETE FROM customers WHERE c_id=" + c_id + ";");
  m_sql->query(sql);
  if (m_sql->lastError().isEmpty()) {
    m_table->setReloadView();
    return;
  }

  QMessageBox *d = new QMessageBox(this);
  d->setWindowTitle(title);
  d->setIcon(QMessageBox::Critical);
  d->setText(
      tr("There are purchase order relationships that cannot be resolved."));
  d->setDetailedText(m_sql->lastError());
  d->setDefaultButton(QMessageBox::Ok);
  d->exec();
}

void TabCustomers::openStartPage() {
  m_editorPage->setEnabled(false);
  if (m_table->rowCount() > 0 && m_table->rowCount() < 20)
    m_table->setReloadView();

  setCurrentIndex(0);
}

void TabCustomers::createSearchQuery(const QString &query) {
  if (!query.isEmpty()) {
    m_table->setQuery(query);
    return;
  }

  QString w_sql = m_searchBar->getSearchStatement();
  if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
    m_table->setQuery(w_sql);
  }
}

void TabCustomers::createNewEntry() {
  if (m_editorWidget->createNewEntry()) {
    m_editorPage->setEnabled(true);
    setCurrentWidget(m_editorPage);
  }
}

void TabCustomers::openEntry(qint64 customerId) {
  if (customerId < 1)
    return;

  if (currentIndex() != 0) {
    QString info(tr("Cannot open this customer.") + "<br>");
    info.append(tr("Because the customer tab is not in overview mode."));
    info.append("<p>");
    info.append(tr("Please save and close all open customers first."));
    info.append("</p>");
    QMessageBox::information(this, tr("Customereditor"), info);
    return;
  }

  if (m_editorWidget->openEditEntry(customerId)) {
    m_editorPage->setEnabled(true);
    setCurrentWidget(m_editorPage);
  }
}

void TabCustomers::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_searchBar->setFilter(0);
    setCurrentIndex(0);
  }
}
