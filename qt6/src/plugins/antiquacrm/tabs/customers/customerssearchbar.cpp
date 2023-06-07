// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerssearchbar.h"
#include "customersselectfilter.h"

#include <QIcon>
#include <QJsonObject>
#include <QRegularExpression>
#include <QToolButton>

CustomersSearchBar::CustomersSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {
  m_selectFilter = new CustomersSelectFilter(this);
  addWidget(m_selectFilter);

  m_searchInput = new AntiquaCRM::ALineEdit(this);
  m_searchInput->setPlaceholderText(tr("Default search"));
  addWidget(m_searchInput);

  addWidget(searchConfines());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));

  connect(m_searchInput, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));

  setFilter(0);
}

const QString CustomersSearchBar::getSearchString(const QStringList &fields) {
  QString _search = m_searchInput->text();
  if (_search.length() < 1) {
    // NOTE prevent empty where clauses
    qWarning("INVALID_CUSTOMER_SEARCH");
    return QString("c_lastname='INVALID_CUSTOMER_SEARCH'");
  }

  if (_search.length() < getMinLength())
    return QString("c_lastname='INVALID_CUSTOMER_SEARCH'");

  QString _query;
  QStringList _list;
  foreach (QString f, fields) {
    QString fset = prepareFieldSearch(f, _search);
    if (fset.isEmpty())
      continue;

    _list << fset;
  }

  if (_list.count() > 0) {
    _query.append("(");
    _query.append(_list.join(" OR "));
    _query.append(")");
  }
  _list.clear();

  return _query;
}

void CustomersSearchBar::setSearch() {
  QString _filter = m_selectFilter->currentFilter();
  if (_filter.isEmpty())
    return;

  if (!_filter.contains("c_id") &&
      (m_searchInput->length() <= m_searchInput->getMinLength())) {
    emit sendNotify(tr("Your input is too short, increase your search!"));
    return;
  }

  emit sendSearchClicked();
}

void CustomersSearchBar::setFilter(int index) {
  m_searchInput->clear();
  m_searchInput->setEnabled(true);
  m_searchInput->setToolTip(QString());

  const QString _tip = m_selectFilter->currentToolTip(index);
  const QString _filter = m_selectFilter->currentFilter(index);

  if (_filter.contains("c_id") || _filter.contains("c_since")) {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
  } else {
    m_searchInput->setValidation(
        AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  }
  m_searchInput->setToolTip(_tip);
  m_searchInput->setPlaceholderText(_tip);

  emit sendFilterChanged(index);
}

void CustomersSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void CustomersSearchBar::setClearAndFocus() {
  m_searchInput->clear();
  m_searchInput->setFocus();
}

void CustomersSearchBar::setSearchFocus() { setClearAndFocus(); }

int CustomersSearchBar::searchLength() { return m_searchInput->length(); }

bool CustomersSearchBar::requiredLengthExists() {
  return (searchLength() >= getMinLength());
}

const QString CustomersSearchBar::getSearchStatement() {
  QString _data = m_selectFilter->currentFilter();
  QString _input = m_searchInput->text();
  if (_data.isEmpty() || _input.isEmpty()) {
    qWarning("No filter found.");
    emit sendNotify(tr("Invalid search input!"));
    return QString();
  }

  QString _sql;
  QStringList _cols = _data.split(",");
  static const QRegularExpression numberPattern("[\\D]+");
  if (_cols.contains("c_id")) {
    QString _str = _input;
    _str.replace(numberPattern, ",");
    _sql = QString("c_id IN (" + _str + ")");
    _str.clear();
    return _sql;
  }

  return getSearchString(_cols);
}
