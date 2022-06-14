// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "searchbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QLayout>
#include <QPushButton>
#include <QRegExp>
#include <QToolButton>
#include <QWidget>

SearchBar::SearchBar(QWidget *parent) : QToolBar(parent) {
  setObjectName("SearchBar");
  setOrientation(Qt::Horizontal);
  setFloatable(false);

  QRegExp reg;
  reg.setPattern("^\\S{2}.+");
  m_textValidator = new QRegExpValidator(reg, this);

  m_numValidator = new QDoubleValidator(this);
  m_numValidator->setRange(0, 9999999999999, 0);

  QToolButton *m_tb = new QToolButton(this);
  m_tb->setEnabled(false);
  m_tb->setIcon(myIcon("search"));
  addWidget(m_tb);
  addSeparator();

  m_filterSection = new QComboBox(this);
  QString filterTip = tr("Press CTRL+F, to quickly open this Menu.");
  m_filterSection->setToolTip(filterTip);
  addWidget(m_filterSection);
  addSeparator();

  m_searchLineEdit = new QLineEdit(this);
  m_searchLineEdit->setObjectName("sql_search_statement");
  m_searchLineEdit->setClearButtonEnabled(true);
  QString searchTip = tr("Press CTRL+S, it clears the input for a new search.");
  m_searchLineEdit->setToolTip(searchTip);
  m_searchLineEdit->setStatusTip(
      tr("You can use a wildcard * to broaden the search."));
  m_searchLineEdit->setPlaceholderText(tr("Search for"));
  m_searchLineEdit->setValidator(m_textValidator);
  addWidget(m_searchLineEdit);
  addSeparator();

  QPushButton *searchBtn = new QPushButton(tr("Start search"), this);
  searchBtn->setObjectName("SearchButton");
  searchBtn->setToolTip(tr("Start search"));
  searchBtn->setIcon(myIcon("search"));
  searchBtn->setContentsMargins(5, 0, 15, 0);
  addWidget(searchBtn);
  addSeparator();

  connect(m_searchLineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(beforeTextChanged(const QString &)));

  connect(m_searchLineEdit, SIGNAL(returnPressed()), this,
          SIGNAL(searchClicked()));

  connect(searchBtn, SIGNAL(released()), this, SIGNAL(searchClicked()));

  connect(m_filterSection, SIGNAL(currentIndexChanged(int)), this,
          SLOT(searchFilterChanged(int)));
}

void SearchBar::setFilterFocus() {
  m_filterSection->setFocus();
  m_filterSection->showPopup();
}

void SearchBar::clearAndFocus() {
  m_searchLineEdit->clear();
  m_searchLineEdit->setFocus();
}

void SearchBar::beforeTextChanged() {
  if (m_searchLineEdit->text().length() >= 2)
    emit searchTextChanged(currentSearchText());
}

void SearchBar::beforeTextChanged(const QString &str) {
  if (str.length() >= 2) {
    if (getSearchFilter(currentFilterIndex()).contains("isbn")) {
      if (currentSearchText() < 10)
        return;
    }
    emit searchTextChanged(str);
  }
}

void SearchBar::updateEditPlaceHolder() {
  if (m_filterSection->count() > 0) {
    int i = currentFilterIndex();
    QString p(tr("Search for"));
    p.append(" ");
    p.append(m_filterSection->itemText(i));
    m_searchLineEdit->setPlaceholderText(p);
    emit currentFilterChanged(i);
  }
}

void SearchBar::searchFilterChanged(int) { updateEditPlaceHolder(); }

void SearchBar::addSearchFilters(const QList<SearchFilter> &list) {
  if (list.isEmpty())
    return;

  m_filterSection->clear();
  QListIterator<SearchFilter> i(list);
  while (i.hasNext()) {
    SearchFilter f = i.next();
    m_filterSection->insertItem(f.index, f.title, f.filter);
  }
}

void SearchBar::setSearchFilter(int index) {
  if (m_filterSection->count() > 0) {
    m_filterSection->setCurrentIndex(index);
  }
}

const QString SearchBar::getSearchFilter(int index) {
  return m_filterSection->itemData(index, Qt::UserRole).toString();
}

void SearchBar::setValidation(SearchBar::Validation v) {
  if (v == SearchBar::Pattern) {
    m_searchLineEdit->setValidator(m_textValidator);
  } else if (v == SearchBar::Number) {
    m_searchLineEdit->setValidator(m_numValidator);
  } else {
    m_searchLineEdit->setValidator(m_textValidator);
  }
}

int SearchBar::currentFilterIndex() { return m_filterSection->currentIndex(); }

void SearchBar::setSearchText(const QString &search) {
  if (search.length() > 2)
    m_searchLineEdit->setText(search);
}

const QString SearchBar::currentSearchText() {
  QString buffer = m_searchLineEdit->text();

  QRegExp reg("[\\'\\\"]+");
  buffer = buffer.replace(reg, "");

  reg.setPattern("(\\s|\\t|\\n|\\r)+");
  buffer = buffer.replace(reg, " ");

  return buffer.trimmed();
}
