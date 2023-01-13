// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "varioussearchbar.h"
#include "varioussearchedit.h"

#include <QDebug>
#include <QHash>
#include <QPushButton>

VariousSearchBar::VariousSearchBar(QWidget *parent) : TabSearchBar{parent} {
  setObjectName("various_search_bar");

  m_searchEdit = new VariousSearchEdit(this);
  m_searchEdit->setPlaceholderText(tr("Search Title"));
  addWidget(m_searchEdit);

  m_searchBtn = startSearchButton(tr("Search"));
  addWidget(m_searchBtn);

  addSeparator();

  addWidget(defaultViewButton());

  connect(m_searchEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

void VariousSearchBar::setSearch() {
  if (m_searchEdit->getLength() <= minLength)
    return;

  p_search = QString();
  QString left = m_searchEdit->getSearch();
  left = left.replace(quotePattern, "");
  left = left.replace(trimPattern, " ");
  p_search = left.trimmed();
  emit sendSearchClicked();
}

void VariousSearchBar::setFilter(int index) {
  Q_UNUSED(index);
  // unused in this area
}

void VariousSearchBar::setFilterFocus() {
  // unused in this area
}

void VariousSearchBar::setClearAndFocus() {
  // unused in this area
}

void VariousSearchBar::setSearchFocus() {
  m_searchEdit->clear();
  m_searchEdit->setFocus();
}

int VariousSearchBar::searchLength() {
  return m_searchEdit->getSearch().length();
}

const QString VariousSearchBar::getSearchStatement() {
  QString search = m_searchEdit->getSearch();
  search.replace(QRegExp("\'"), "");
  QStringList cmd;
  cmd.append("va_id LIKE '" + search + "%'");
  cmd.append("va_title ILIKE '%" + search + "%'");
  cmd.append("va_title_extended ILIKE '%" + search + "%'");
  cmd.append("va_description ILIKE '%" + search + "%'");
  return cmd.join(" OR ");
}
