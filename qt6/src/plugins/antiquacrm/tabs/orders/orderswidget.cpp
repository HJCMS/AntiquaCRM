// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderswidget.h"
#include "ordersconfig.h"
#include "orderstableview.h"

#include <AntiquaCRM>
#include <QtWidgets>

OrdersWidget::OrdersWidget(QWidget *parent)
    : AntiquaCRM::TabsIndex{ORDERS_INTERFACE_TABID, parent} {
  setObjectName("orders_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("kjournal"));
  setClosable(false);
  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  m_p1Layout->addWidget(new QLabel("TODO", this));

  m_table = new OrdersTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);

  m_p1Layout->addWidget(new QLabel("TODO", this));

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("CDsVinyl_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  QWidget *m_editorWidget = new QWidget(this);
  // m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(0);

  // Signals::OrdersTableView
  // connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
  //       SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendSocketOperation(const QJsonObject &)),
          SLOT(sendSocketOperation(const QJsonObject &)));
}

void OrdersWidget::popupWarningTabInEditMode() {
  QString info(tr("Can't open this Order"));
  info.append("<p>");
  info.append(tr("Because Orders tab is in edit mode."));
  info.append("</p><p>");
  info.append(tr("You need to save and close the editor first."));
  info.append("</p>");
  QMessageBox::information(this, windowTitle(), info);
}

void OrdersWidget::setDefaultTableView() {
  // m_searchBar->setClearAndFocus();
  setCurrentIndex(0);
  m_table->setQuery(m_table->defaultWhereClause());
}

void OrdersWidget::openStartPage() {
  if (m_table->isAutoRefreshEnabled())
    m_table->setReloadView();

  setCurrentIndex(0);
}

void OrdersWidget::createSearchQuery(const QString &history) {
  if (!history.isEmpty()) {
    m_table->setQuery(history);
    return;
  }

  //  QString w_sql = m_searchBar->getSearchStatement();
  //  if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
  //    m_table->setQuery(w_sql);
  //  }
}

void OrdersWidget::createNewEntry() { qDebug() << Q_FUNC_INFO << "__TODO__"; }

void OrdersWidget::openEntry(qint64 articleId) {
  qDebug() << Q_FUNC_INFO << "__TODO__" << articleId;
}

void OrdersWidget::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    setCurrentIndex(0);
  }
}

const QString OrdersWidget::getTitle() const { return tr("Orders"); }

bool OrdersWidget::customAction(const QJsonObject &obj) {
  qDebug() << Q_FUNC_INFO << "__TODO__" << obj;
  return false;
}
