// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "searchbar.h"

#include <QIcon>

SearchBar::SearchBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setContentsMargins(0, 0, 0, 0);
}

QPushButton *SearchBar::clickSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(this);
  btn->setIcon(QIcon(":icons/action_search.png"));

  if (text.isEmpty())
    btn->setText(tr("Start search"));
  else
    btn->setText(text);

  return btn;
}
