// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorybooks.h"
#include "bookstableview.h"
#include "pmetatypes.h"
#include "searchbookbar.h"
#include "statsbookbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

InventoryBooks::InventoryBooks(QWidget *parent) : QWidget{parent} {
  setObjectName("InventoryBooks");

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new SearchBookBar(this);
  layout->addWidget(m_toolBar);

  m_tableView = new BooksTableView(this);
  layout->addWidget(m_tableView);

  m_statsBookBar = new StatsBookBar(this);
  layout->addWidget(m_statsBookBar);

  setLayout(layout);

  // Such anfragen
  connect(m_toolBar, SIGNAL(s_sqlQueryChanged(const SearchStatement &)),
          m_tableView, SLOT(queryStatement(const SearchStatement &)));
  // Verlaufs abfragen
  connect(m_statsBookBar, SIGNAL(s_queryHistory(const QString &)),
          m_tableView, SLOT(queryHistory(const QString &)));
  // ID in Tabelle ausgewählt
  connect(m_tableView, SIGNAL(articleIdSelected(const QHash<QString, QString> &)),
          this, SLOT(selectArticleId(const QHash<QString, QString> &)));
}

void InventoryBooks::selectArticleId(const QHash<QString,QString> &c) {
  if (c.isEmpty())
    return;

  QHashIterator<QString,QString> i(c);
  while (i.hasNext()) {
    i.next();
    QString s("ib_id=");
    s.append(i.key());
    s.append(" AND ib_title ILIKE '");
    s.append(i.value());
    s.append("'");
    qDebug() << s;
    break;
  }
}
