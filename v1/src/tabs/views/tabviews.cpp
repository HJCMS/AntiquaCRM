// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabviews.h"

#include <AntiquaCRM>
#include <QHeaderView>
#include <QDebug>

TabViews::TabViews(QWidget *parent) : Inventory{parent} {
  setWindowTitle(tr("Views"));
  setWindowIcon(getTabIcon("view_log"));
  setClosable(true);

  // Begin MainPage layout
  m_mainPage = new QTableView(this);
  m_mainPage->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_mainPage->setCornerButtonEnabled(false);
  m_mainPage->setDragEnabled(false);
  m_mainPage->setDragDropOverwriteMode(false);
  m_mainPage->setWordWrap(false);
  m_mainPage->setAlternatingRowColors(true);
  m_mainPage->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_mainPage->setSelectionMode(QAbstractItemView::SingleSelection);
  m_mainPage->setSortingEnabled(true);

  QHeaderView *header = m_mainPage->horizontalHeader();
  header->setDefaultAlignment(Qt::AlignCenter);
  header->setSectionResizeMode(QHeaderView::ResizeToContents);
  header->setStretchLastSection(true);

  insertWidget(0, m_mainPage);
  // End

  AntiquaCRM::ASqlSettings sqlconfig(this);
  QSqlDatabase db = QSqlDatabase::database(sqlconfig.connectionName());
  m_model = new QSqlTableModel(this, db);
  m_mainPage->setModel(m_model);
}

void TabViews::openStartPage() { setCurrentIndex(0); }

void TabViews::createSearchQuery(const QString &query) {
  m_model->setTable(query);
  m_model->select();
}

void TabViews::onEnterChanged() {}
