// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderswidget.h"
#include "ordersconfig.h"
#include "orderseditor.h"
#include "orderssearchbar.h"
#include "ordersstatusbar.h"
#include "orderstableoverview.h"

#include <AntiquaCRM>
#include <QDate>
#include <QtWidgets>

OrdersWidget::OrdersWidget(QWidget* parent)
    : AntiquaCRM::TabsIndex{ORDERS_INTERFACE_TABID, parent} {
  setObjectName("orders_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("view-financial-transfer"));
  setClosable(false);
  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout* m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  m_searchBar = new OrdersSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);

  m_table = new OrdersTableOverView(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_p1Layout->setStretch(1, 1);

  m_statusBar = new OrdersStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(ViewPage::MainView, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("CDsVinyl_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new OrdersEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(ViewPage::EditorView, m_editorPage);
  // End

  setCurrentIndex(ViewPage::MainView);

  // Signals::OrdersSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar, SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar, SLOT(setFilterFocus()));
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendNotify(QString)), m_statusBar, SLOT(showMessage(QString)));

  // Signals::OrdersTableView
  connect(m_table, SIGNAL(sendQueryReport(QString)), m_statusBar, SLOT(showMessage(QString)));
  connect(m_table, SIGNAL(sendCopyToClibboard(QString)), SLOT(copyToClipboard(QString)));
  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));
  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));
  connect(m_table, SIGNAL(sendSocketOperation(QJsonObject)),
          SLOT(sendSocketOperation(QJsonObject)));
  connect(m_table, SIGNAL(sendStartRefund(qint64)), SLOT(refundEntry(qint64)));
  // Signals::OrdersEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));
  // Signals::OrdersStatusBar
  connect(m_statusBar, SIGNAL(sendHistoryQuery(QString)), SLOT(createSearchQuery(QString)));
  connect(m_statusBar, SIGNAL(sendDefaultView()), SLOT(setDefaultTableView()));
  connect(m_statusBar, SIGNAL(sendReloadView()), m_table, SLOT(setReloadView()));
}

bool OrdersWidget::firstStartOnWorkday() {
  // current day
  const QDate _cd = QDate::currentDate();
  // past day
  const QString _pd = _cd.addDays(-1).toString("yyyy-MM-dd");

  AntiquaCRM::ASettings cfg(this);
  if (cfg.value("lastWorkDay", _pd).toDate() == _cd)
    return false; // nothing todo

  QString _sql("(");
  _sql.append(m_table->defaultWhereClause());
  // remind,admonish and collection procedures
  _sql.append(") OR (o_payment_status IN (2,3,5))");

  m_table->setQuery(_sql);
  cfg.setValue("lastWorkDay", _cd.toString("yyyy-MM-dd"));
  return true;
}

void OrdersWidget::setDefaultTableView() {
  m_searchBar->setClearAndFocus();
  setCurrentIndex(ViewPage::MainView);
  m_table->setQuery(m_table->defaultWhereClause());
}

void OrdersWidget::openStartPage() {
  if (m_table->isAutoRefreshEnabled())
    m_table->setReloadView();

  setCurrentIndex(ViewPage::MainView);
}

void OrdersWidget::createSearchQuery(const QString& history) {
  // Verlaufs und Suchanfrage
  if (history.length() > 10) {
    m_statusBar->startProgress();
    m_table->setQuery(history);
    m_statusBar->finalizeProgress();
    return;
  }
  // Die Standardabfrage wird aufgerufen!
  const QString _sql = m_searchBar->getSearchStatement();
  if (_sql.isEmpty()) {
    qWarning("OrdersWidget::createSearchQuery „length()“, to small!");
    return;
  }
  m_statusBar->startProgress();
  m_table->setQuery(_sql);
  m_statusBar->finalizeProgress();
}

void OrdersWidget::createNewEntry() {
  qInfo("createNewEntry not used in this section!");
}

void OrdersWidget::openEntry(qint64 oid) {
  if (oid < 1)
    return;

  if (currentPage() != ViewPage::MainView) {
    openWarningPopUpPageIndex(windowTitle());
    return;
  }

  if (m_editorWidget->openEditEntry(oid)) {
    setCurrentWidget(m_editorPage);
  }
}

void OrdersWidget::refundEntry(qint64 oid) {
  if (oid < 1)
    return;

  if (currentPage() != ViewPage::MainView) {
    openWarningPopUpPageIndex(windowTitle());
    return;
  }

  if (m_editorWidget->createOrderRefund(oid)) {
    setCurrentWidget(m_editorPage);
  }
}

void OrdersWidget::onEnterChanged() {
  if (!initialed) {
    initialed = firstStartOnWorkday();
    if (!initialed)
      initialed = m_table->setQuery();

    setCurrentIndex(ViewPage::MainView);
    return;
  }
  if (m_table->isAutoRefreshEnabled())
    m_table->setReloadView();
}

const QString OrdersWidget::getTitle() const {
  return tr("Orders");
}

bool OrdersWidget::customAction(const QJsonObject& obj) {
  if (obj.isEmpty() || !obj.contains("ACTION"))
    return false;

  // first call?
  onEnterChanged();

  // get operation
  const QString _action = obj.value("ACTION").toString().toLower();

  // operations that needs the editors page
  if (currentPage() == ViewPage::EditorView) {
    // Add Article to opened order
    if (_action == "add_article") {
      qint64 _aid = obj.value("VALUE").toInt();
      return m_editorWidget->addArticle(_aid);
    } else if (_action == "change_customer") {
      return m_editorWidget->changeOrderToCustomer(obj);
    }
    // if editor is already opened!
    openWarningPopUpPageIndex(windowTitle());
    return false;
  }

  // operations that needs the main view
  if (_action == "open_order") {
    // Open Order
    qint64 _oid = obj.value("VALUE").toInt();
    if (m_editorWidget->openEditEntry(_oid)) {
      setCurrentIndex(ViewPage::EditorView);
      return true;
    }
  } else if (_action == "create_order") {
    // Create New Order with Customer id
    qint64 _cid = obj.value("VALUE").toInt();
    if (_cid < 1)
      return false;

    if (m_editorWidget->createNewOrder(_cid)) {
      setCurrentIndex(ViewPage::EditorView);
      return true;
    }
  } else if (_action == "import_order") {
    // Create Import Orders from Provider data.
    qint64 _cid = obj.value("VALUE").toInt();
    if (_cid < 1)
      return false;

    if (m_editorWidget->createCustomEntry(obj)) {
      setCurrentIndex(ViewPage::EditorView);
      return true;
    }
  }
#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << "Rejected" << obj;
#endif
  return false;
}

const QStringList OrdersWidget::acceptsCustomActions() const {
  return QStringList({"add_article",  /**< Add Article to opened tab */
                      "open_order",   /**< Open Order */
                      "create_order", /**< Create new empty order */
                      "import_order", /**< Import order from Provider */
                      "change_customer"});
}
