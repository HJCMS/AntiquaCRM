// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsearchbar.h"

#include <QDebug>
#include <QIcon>

TabSearchBar::TabSearchBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setContentsMargins(0, 0, 0, 0);
}

void TabSearchBar::setSearchStockEnabled(bool b) {
  SearchWithStock = b;
  emit sendStockEnabled(b);
}

QPushButton *TabSearchBar::startSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(this);
  btn->setIcon(QIcon(":icons/action_search.png"));
  btn->setToolTip(tr("Push to start search."));

  if (text.isEmpty())
    btn->setText(tr("Start search"));
  else
    btn->setText(text);

  return btn;
}

QCheckBox *TabSearchBar::stockCheckBox(const QString &text) {
  QCheckBox *m_box = new QCheckBox(this);
  QString title = (text.isEmpty()) ? tr("Stock") : text;
  m_box->setText(title);
  m_box->setToolTip(tr("Only search with %1").arg(title));
  connect(m_box, SIGNAL(toggled(bool)), SLOT(setSearchStockEnabled(bool)));
  return m_box;
}

QPushButton *TabSearchBar::defaultViewButton(const QString &text) {
  QPushButton *btn = new QPushButton(this);
  btn->setIcon(QIcon("://icons/spreadsheet.png"));
  btn->setToolTip(tr("Push to load the Standard view."));
  btn->setStatusTip(btn->toolTip());
  if (text.isEmpty())
    btn->setText(tr("Default view"));
  else
    btn->setText(text);

  connect(btn, SIGNAL(clicked()), SIGNAL(sendRestoreView()));
  return btn;
}

QPushButton *TabSearchBar::customSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(text, this);
  btn->setIcon(QIcon("://icons/action_search.png"));
  btn->setToolTip(tr("Custom queries"));
  btn->setStatusTip(tr("Click to open Dialog for custom queries."));
  connect(btn, SIGNAL(clicked()), SIGNAL(sendOpenCustomSearch()));
  return btn;
}
