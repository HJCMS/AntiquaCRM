// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "searchbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtWidgets/QAction>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

SearchBar::SearchBar(QWidget *parent) : QToolBar(parent) {
  setObjectName("SearchBar");
  setOrientation(Qt::Horizontal);
  setFloatable(false);

  QRegExp reg;
  reg.setPattern("^\\S{2}.+");
  m_textValidator = new QRegExpValidator(reg, this);

  m_numValidator = new QDoubleValidator(this);
  m_numValidator->setRange(0, 9999999999999);

  QToolButton *m_tb = new QToolButton(this);
  m_tb->setEnabled(false);
  m_tb->setIcon(myIcon("search"));
  addWidget(m_tb);
  addSeparator();

  m_filterSection = new QComboBox(this);
  addWidget(m_filterSection);
  addSeparator();

  m_searchLineEdit = new QLineEdit(this);
  m_searchLineEdit->setObjectName("SearchLineEdit");
  m_searchLineEdit->setStatusTip(tr("You can use a wildcard * to broaden the search."));
  m_searchLineEdit->setPlaceholderText(tr("Search for"));
  m_searchLineEdit->setValidator(m_textValidator);
  addWidget(m_searchLineEdit);
  addSeparator();

  QAction *m_clear = addAction(tr("Clear"));
  m_clear->setIcon(myIcon("clear_left"));
  m_clear->setToolTip(tr("Clear searchinput"));

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

  connect(m_clear, SIGNAL(triggered()), m_searchLineEdit, SLOT(clear()));
  connect(searchBtn, SIGNAL(released()), this, SIGNAL(searchClicked()));

  connect(m_filterSection, SIGNAL(currentIndexChanged(int)), this,
          SLOT(searchFilterChanged(int)));
}

void SearchBar::beforeTextChanged() {
  if (m_searchLineEdit->text().length() >= 2)
    emit searchTextChanged(m_searchLineEdit->text());
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

const QString SearchBar::currentSearchText() {
  return m_searchLineEdit->text();
}
