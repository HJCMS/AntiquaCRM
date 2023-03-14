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

  addWidget(searchConfines());
  addSeparator();
  addWidget(stockCheckBox());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_searchEdit, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));
}

void VariousSearchBar::setSearch() {
  if (m_searchEdit->getLength() < getMinLength()) {
    emit sendNotify(tr("Your input is too short, increase your search!"));
    return;
  }

  p_search = QString();
  QString left = m_searchEdit->getSearch();
  left = left.replace(quotePattern, "");
  left = left.replace(trimPattern, " ");
  p_search = left.trimmed();
  emit sendSearchClicked();
}

void VariousSearchBar::setFilter(int index) {
  Q_UNUSED(index);
  setSearchFocus();
}

void VariousSearchBar::setFilterFocus() { setSearchFocus(); }

void VariousSearchBar::setClearAndFocus() { setSearchFocus(); }

void VariousSearchBar::setSearchFocus() {
  m_searchEdit->clear();
  m_searchEdit->setFocus();
}

int VariousSearchBar::searchLength() {
  return m_searchEdit->getSearch().length();
}

const QString VariousSearchBar::getSearchStatement() {
  if (!m_searchEdit->isValid(getMinLength()))
    return QString();

  QString search = m_searchEdit->getSearch();
  search.replace(QRegExp("\'"), "");
  QString sql(withStock() ? "va_count>0 AND " : "");
  QStringList buffer;
  if (search.contains(articlePattern)) {
    return QString("va_id=" + search);
  }
  buffer.append("(" + prepareFieldSearch("va_title", search) + ")");
  buffer.append("(" + prepareFieldSearch("va_title_extended", search) + ")");
  buffer.append("(" + prepareFieldSearch("va_description", search) + ")");
  sql.append("(" + buffer.join(" OR ") + ")");
  return sql;
}
