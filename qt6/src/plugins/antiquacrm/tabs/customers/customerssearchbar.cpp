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
  m_filter = new CustomersSelectFilter(this);
  addWidget(m_filter);

  m_input = new AntiquaCRM::ALineEdit(this);
  m_input->setPlaceholderText(tr("Default search"));
  addWidget(m_input);

  addWidget(searchConfines());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_filter, SIGNAL(currentIndexChanged(int)), SLOT(setFilter(int)));
  connect(m_input, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));

  setFilter(0);
}

const QString CustomersSearchBar::getSearchString(const QStringList &fields) {
  QString _search = m_input->text();
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
  QString _filter = m_filter->getFilter();
  if (_filter.isEmpty())
    return;

  if (!_filter.contains("c_id") &&
      (m_input->length() <= m_input->getMinLength())) {
    emit sendNotify(tr("Your input is too short, increase your search!"));
    return;
  }

  emit sendSearchClicked();
}

void CustomersSearchBar::setFilter(int index) {
  m_input->clear();
  m_input->setEnabled(true);
  m_input->setToolTip(QString());

  const QString _tip = m_filter->getToolTip(index);
  const QString _filter = m_filter->getFilter(index);

  if (_filter.contains("c_id") || _filter.contains("c_since")) {
    m_input->setValidation(AntiquaCRM::ALineEdit::InputValidator::ARTICLE);
  } else {
    m_input->setValidation(AntiquaCRM::ALineEdit::InputValidator::STRINGS);
  }
  m_input->setToolTip(_tip);
  m_input->setPlaceholderText(_tip);

  emit sendFilterChanged(index);
}

void CustomersSearchBar::setFilterFocus() {
  m_filter->setFocus();
  m_filter->showPopup();
}

void CustomersSearchBar::setClearAndFocus() {
  m_input->clear();
  m_input->setFocus();
}

void CustomersSearchBar::setSearchFocus() { setClearAndFocus(); }

int CustomersSearchBar::searchLength() { return m_input->length(); }

bool CustomersSearchBar::requiredLengthExists() {
  return (searchLength() >= getMinLength());
}

const QString CustomersSearchBar::getSearchStatement() {
  QString _data = m_filter->getFilter();
  QString _input = m_input->text();
  if (_data.isEmpty() || _input.isEmpty()) {
    qWarning("No filter found.");
    emit sendNotify(tr("Invalid search input!"));
    return QString();
  }

  QString _sql;
  QStringList _cols = _data.split(",");
  static const QRegularExpression _no_digits("\\D+");
  if (_cols.contains("c_id")) {
    QString _str = _input;
    _str.replace(_no_digits, ",");
    _sql = QString("c_id IN (" + _str + ")");
    _str.clear();
    return _sql;
  }

  return getSearchString(_cols);
}
