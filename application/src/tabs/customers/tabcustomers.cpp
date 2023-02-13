// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabcustomers.h"
#include "customerseditor.h"
#include "customerssearchbar.h"
#include "customersstatusbar.h"
#include "customerstableview.h"

#include <AntiquaCRM>
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
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  // Signals
  // searchbar
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendRestoreView()), SLOT(setDefaultTableView()));
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  // maintable
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendResultExists(bool)), m_statusBar,
          SLOT(setCreateButtonEnabled(bool)));

  connect(m_table, SIGNAL(sendDeleteEntry(qint64)),
          SLOT(setDeleteCustomer(qint64)));

  // Wird für das erstellen eines Neuen Auftrags verwendet!
  connect(m_table, SIGNAL(sendCurrentId(qint64)), SLOT(createNewOrder(qint64)));

  // editor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));
  connect(m_editorWidget, SIGNAL(sendEditorAction(qint64)),
          SLOT(createNewOrder(qint64)));

  // statusbar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabCustomers::popupWarningTabInEditMode() {
  QString info(tr("Cannot open Customers Editor."));
  info.append("<p>");
  info.append(tr("Because the customer tab is not in mainview mode."));
  info.append("</p><p>");
  info.append(tr("Please save and close the open customer editor first."));
  info.append("</p>");
  QMessageBox::information(this, tr("Customereditor"), info);
}

void TabCustomers::setDefaultTableView() {
  m_searchBar->setClearAndFocus();
  setCurrentIndex(0);
  m_table->setQuery(m_table->defaultWhereClause());
}

void TabCustomers::setDeleteCustomer(qint64 customerId) {
  QString info = tr("Do you really want to delete this customer?");
  info.append("<p>");
  info.append(tr("This process is irreversible!"));
  info.append("</p><p>");
  info.append(tr("In addition, the system will refuse to delete if an order"
                 " exists for this customer!"));
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

void TabCustomers::createNewOrder(qint64 customerId) {
  if (customerId < 1)
    return;

  // Prevent duplicate creation
  QString s("SELECT o_id, o_since FROM inventory_orders ");
  s.append(" WHERE o_customer_id=" + QString::number(customerId));
  s.append(" AND DATE(o_since)=CURRENT_DATE");
  s.append(" ORDER BY o_id DESC LIMIT 1;");
  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery q = sql.query(s);
  if (q.size() > 0) {
    q.next();
    QString str_o_id = QString::number(q.value("o_id").toInt());
    QString str_time = q.value("o_since").toDateTime().toString("HH:mm");
    QString info("<p>");
    info.append(tr("Today at %2, you created a new order for this customer "
                   "with the Order ID %1.")
                    .arg(str_o_id, str_time));
    info.append("</p><p>");
    info.append(tr("Do you really want to create a second new Order entry?"));
    info.append("</p>");
    QString title = tr("Create order, request!");
    int ret = QMessageBox::question(this, title, info);
    if (ret == QMessageBox::No)
      return;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qDebug() << Q_FUNC_INFO << s << sql.lastError();
  }
#endif

  QJsonObject obj;
  obj.insert("window_operation", "new_order");
  obj.insert("tab", "orders_tab");
  obj.insert("new_order", QJsonValue(customerId));
  sendSocketOperation(obj);
}

void TabCustomers::openStartPage() {
  if (m_table->isAutoRefreshEnabled())
    m_table->setReloadView();

  setCurrentIndex(0);
}

void TabCustomers::createSearchQuery(const QString &query) {
  if (!query.isEmpty()) {
    m_table->setQuery(query);
    return;
  }

  QString w_sql = m_searchBar->getSearchStatement();
  // qDebug() << Q_FUNC_INFO << w_sql;
  if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
    m_table->setQuery(w_sql);
  }
}

void TabCustomers::createNewEntry() {
  if (m_editorWidget->createNewEntry()) {
    setCurrentWidget(m_editorPage);
  }
}

void TabCustomers::openEntry(qint64 customerId) {
  if (customerId < 1)
    return;

  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return;
  }

  if (m_editorWidget->openEditEntry(customerId)) {
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

Inventory::ViewIndex TabCustomers::currentView() {
  switch (currentIndex()) {
  case 1:
    return Inventory::ViewIndex::EditorView;

  default:
    return Inventory::ViewIndex::MainView;
  }
}

bool TabCustomers::customAction(const QJsonObject &obj) {
  if (obj.isEmpty() || !obj.contains("window_operation"))
    return false;

  if (!initialed) /**< first call? */
    onEnterChanged();

  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return false;
  }

  QString op = obj.value("window_operation").toString();
  if (!obj.contains(op))
    return false;

  QJsonValue value = obj.value(op);
  QJsonValue::Type type = value.type();
  QString failMessage(tr("Invalid arguments to open Customer!"));

  if (value.isNull()) {
    sendStatusMessage(failMessage);
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << op << value;
#endif
    return false;
  }

  qint64 open_customer_id = -1;
  // Kunde mit Kundennummer öffnen!
  if (op == "open_customer" && type == QJsonValue::Double) {
    open_customer_id = value.toInt();
  }

  // Mit Bestellnummer nach Kunde suchen und öffnen!
  if (op == "order_customer" && type == QJsonValue::Double) {
    qint64 o_id = value.toInt();
    if (o_id > 0)
      open_customer_id = m_editorWidget->queryCustomerByOrderId(o_id);
  }

  if ((open_customer_id > 0) &&
      m_editorWidget->openEditEntry(open_customer_id)) {
    setCurrentWidget(m_editorPage);
    return true;
  }

  sendStatusMessage(failMessage);
  return false;
}
