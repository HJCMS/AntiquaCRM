// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabviews.h"
#include "viewstablemodel.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QHeaderView>
#include <QLayout>

TabViews::TabViews(QWidget *parent) : Inventory{parent} {
  setWindowTitle(tr("Views"));
  setWindowIcon(getTabIcon("view_log"));
  setObjectName("antiqua_views_tab");
  setClosable(true);

  // Begin MainPage layout
  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *m_layout = new QVBoxLayout(centralWidget);

  m_label = new QLabel(centralWidget);
  m_label->setText(tr("Views"));
  m_layout->addWidget(m_label);

  m_tableView = new QTableView(centralWidget);
  m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tableView->setCornerButtonEnabled(false);
  m_tableView->setDragEnabled(false);
  m_tableView->setDragDropOverwriteMode(false);
  m_tableView->setWordWrap(false);
  m_tableView->setAlternatingRowColors(true);
  m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  m_tableView->setSortingEnabled(true);

  QHeaderView *header = m_tableView->horizontalHeader();
  header->setDefaultAlignment(Qt::AlignCenter);
  header->setSectionResizeMode(QHeaderView::ResizeToContents);
  header->setStretchLastSection(true);

  m_layout->addWidget(m_tableView);
  centralWidget->setLayout(m_layout);

  insertWidget(0, centralWidget);
  // End

  AntiquaCRM::ASqlSettings sqlconfig(this);
  QSqlDatabase db = QSqlDatabase::database(sqlconfig.connectionName());

  m_model = new ViewsTableModel(db, this);
  m_tableView->setModel(m_model);
}

void TabViews::openStartPage() { setCurrentIndex(0); }

void TabViews::createSearchQuery(const QString &query) {
  m_model->setTable(query);
  m_model->select();
  m_label->setText(m_model->getLabelText());
}

void TabViews::onEnterChanged() {}
