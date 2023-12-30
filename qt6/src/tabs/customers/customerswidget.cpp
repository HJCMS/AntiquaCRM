// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerswidget.h"
#include "customersconfig.h"
#include "customerseditor.h"
#include "customerssearchbar.h"
#include "customersstatusbar.h"
#include "customerstableview.h"

#include <QDebug>
#include <QLayout>
#include <QMessageBox>

CustomersWidget::CustomersWidget(QWidget *parent)
    : AntiquaCRM::TabsIndex{CUSTOMERS_INTERFACE_TABID, parent} {
  setObjectName("customers_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("system-users"));
  setClosable(false);

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  // Searchbar
  m_searchBar = new CustomersSearchBar(m_mainPage);
  m_searchBar->setMinLength(3);
  m_p1Layout->addWidget(m_searchBar);

  // TableView
  m_table = new CustomersTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);

  // StatusBar
  m_statusBar = new CustomersStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("book_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new CustomersEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(ViewPage::MainView);

  // Signals::CustomersSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendNotify(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  // Signals::CustomersTableView
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendDeleteEntry(qint64)),
          SLOT(setDeleteCustomer(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendSocketOperation(const QJsonObject &)),
          SLOT(sendSocketOperation(const QJsonObject &)));

  // Signals::CustomersEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));
  connect(m_editorWidget, SIGNAL(sendEditorAction(qint64)),
          SLOT(setCreateNewOrder(qint64)));

  // Signals::CustomerstatusBar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendDefaultView()), SLOT(setDefaultTableView()));
  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void CustomersWidget::setDefaultTableView() {
  if (currentPage() != ViewPage::MainView)
    return;

  m_searchBar->setClearAndFocus();
  m_table->setQuery(m_table->defaultWhereClause());
  m_statusBar->setCreateButtonEnabled(false);
}

void CustomersWidget::setDeleteCustomer(qint64 customerId) {
  if (customerId < 1)
    return;

  QString _title = tr("Delete Customer");
  QString _text = tr("Do you really want to delete this customer?");
  _text.append("<p>" + tr("This process is irreversible!") + "</p><p>");
  _text.append(tr("In addition, the system will refuse to delete if an order"
                  " exists for this customer!"));
  _text.append("</p>");

  int _ret = QMessageBox::question(this, _title, _text);
  if (_ret == QMessageBox::No)
    return;

  QString _id = QString::number(customerId);
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString _sql("DELETE FROM customers WHERE c_id=" + _id + ";");
  m_sql->query(_sql);
  if (m_sql->lastError().isEmpty()) {
    m_table->setReloadView();
    return;
  }

  QMessageBox *m_d = new QMessageBox(this);
  m_d->setWindowTitle(_title);
  m_d->setIcon(QMessageBox::Critical);
  m_d->setText(
      tr("There are purchase order relationships that cannot be resolved."));
  m_d->setDetailedText(m_sql->lastError());
  m_d->setDefaultButton(QMessageBox::Ok);
  m_d->exec();
}

void CustomersWidget::setCreateNewOrder(qint64 customerId) {
  if (customerId < 1)
    return;

  // Prevent duplicate orders
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
    info.append(tr("Today at %2, you created a order for this customer with "
                   "the Order ID %1.")
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
  obj.insert("ACTION", "create_order");
  obj.insert("TARGET", "orders_tab");
  obj.insert("VALUE", QJsonValue(customerId));
  sendSocketOperation(obj);
}

void CustomersWidget::openStartPage() {
  setCurrentIndex(ViewPage::MainView);
  if (m_table->isAutoRefreshEnabled()) {
    m_statusBar->setCreateButtonEnabled(false);
    m_table->setReloadView();
  }

#ifdef ANTIQUA_DEVELOPEMENT
  if (isWindowModified())
    qDebug() << Q_FUNC_INFO << isWindowModified();
#endif
}

void CustomersWidget::createSearchQuery(const QString &history) {
  // Verlaufs und Suchanfrage
  if (history.length() > 10) {
    m_statusBar->startProgress();
    m_table->setQuery(history);
    // Nur Aktivieren wenn eine Suche ausgeführt wurde.
    m_statusBar->setCreateButtonEnabled(false);
    m_statusBar->finalizeProgress();
    return;
  }
  // Die Standardabfrage wird aufgerufen!
  const QString _sql = m_searchBar->getSearchStatement();
  if (_sql.isEmpty()) {
    qWarning("CustomersWidget::createSearchQuery „length()“, to small!");
    return;
  }
  // qDebug() << "Customers:Search" << _sql;
  m_statusBar->startProgress();
  m_table->setQuery(_sql);
  m_statusBar->setCreateButtonEnabled(true);
  m_statusBar->finalizeProgress();
}

void CustomersWidget::createNewEntry() {
  if (currentPage() == ViewPage::MainView && m_editorWidget->createNewEntry()) {
    setCurrentIndex(ViewPage::EditorView);
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qWarning("Reject CustomersWidget::createNewEntry - no main page view!");
  }
#endif
}

void CustomersWidget::openEntry(qint64 articleId) {
  if (articleId < 1)
    return;

  if (currentPage() != ViewPage::MainView) {
    openWarningPopUpPageIndex(windowTitle());
    return;
  }

  if (m_editorWidget->openEditEntry(articleId)) {
    setCurrentIndex(ViewPage::EditorView);
  }
}

void CustomersWidget::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_searchBar->setFilter(0);
  }
}

const QString CustomersWidget::getTitle() const { return tr("Customers"); }

bool CustomersWidget::customAction(const QJsonObject &obj) {
  if (obj.isEmpty() || !obj.contains("ACTION"))
    return false;

  // first call?
  onEnterChanged();

  if (currentPage() != ViewPage::MainView) {
    openWarningPopUpPageIndex(windowTitle());
    return false;
  }

  const QString _op = obj.value("ACTION").toString().toLower();
  if (!acceptsCustomActions().contains(_op))
    return false;

  qint64 _cid = obj.value("VALUE").toInt();
  if (_cid < 1)
    return false;

  if (m_editorWidget->openEditEntry(_cid)) {
    setCurrentIndex(ViewPage::EditorView);
    return true;
  }

  return false;
}

const QStringList CustomersWidget::acceptsCustomActions() const {
  return QStringList({"open_customer"});
}
