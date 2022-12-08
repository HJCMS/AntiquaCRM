// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsearchbar.h"

#include <QIcon>

TabSearchBar::TabSearchBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setContentsMargins(0, 0, 0, 0);
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

QPushButton *TabSearchBar::defaultViewButton(const QString &text) {
  QPushButton *btn = new QPushButton(this);
  btn->setIcon(QIcon("://icons/spreadsheet.png"));
  btn->setToolTip(tr("Push to load the Standard view."));

  if (text.isEmpty())
    btn->setText(tr("Default view"));
  else
    btn->setText(text);

  return btn;
}
