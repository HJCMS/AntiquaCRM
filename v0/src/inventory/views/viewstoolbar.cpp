// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewstoolbar.h"
#include "myicontheme.h"

#include <QDebug>
#include <QSizePolicy>

ViewsToolBar::ViewsToolBar(QWidget *parent) : QToolBar{parent} {
  setObjectName("inventory_views_toolbar");
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  m_sql = new HJCMS::SqlCore(this);

  QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

  m_comboBox = new QComboBox(this);
  int i = 0;
  m_comboBox->insertItem(i++, tr("select your operation"), QVariant());
  if (initViews())
    m_comboBox->setSizePolicy(sp);

  addWidget(m_comboBox);

  addSeparator();

  m_query = addAction(tr("start query"));
  m_query->setIcon(myIcon("search"));

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(operationChanged(int)));
  connect(m_query, SIGNAL(triggered()), this, SLOT(refresh()));
}

bool ViewsToolBar::initViews() {
  QString sql("SELECT table_name,");
  sql.append(" obj_description(table_name::regclass) AS comment");
  sql.append(" FROM information_schema.tables");
  sql.append(" WHERE table_schema = current_schema AND table_type='VIEW';");

  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      if (q.value("comment").toString().isEmpty())
        continue;

      m_comboBox->addItem(q.value("comment").toString(),
                          q.value("table_name").toString());
    }
    return true;
  }
  return false;
}

void ViewsToolBar::refresh() {
  int index = m_comboBox->currentIndex();
  if (index > 0)
    operationChanged(index);
}

void ViewsToolBar::operationChanged(int index) {
  QVariant data = m_comboBox->itemData(index, Qt::UserRole);
  if (!data.isNull())
    emit s_queryTableView(data.toString());
}
