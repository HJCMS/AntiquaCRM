// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdsvinylsearchbar.h"
#include "cdsvinylselectfilter.h"
#include "cdsvinylconfig.h"

#include <QIcon>
#include <QJsonObject>
#include <QRegularExpression>
#include <QToolButton>

CDsVinylSearchBar::CDsVinylSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {
  m_selectFilter = new CDsVinylSelectFilter(this);
  addWidget(m_selectFilter);

  // _input
  m_searchInput = new AntiquaCRM::ALineEdit(this);
  m_searchInput->setPlaceholderText(tr("Default search"));
  addWidget(m_searchInput);

  addWidget(searchImageOptions());
  addWidget(stockCheckBox());

  m_searchBtn = startSearchButton();
  addWidget(m_searchBtn);

  connect(m_selectFilter, SIGNAL(currentIndexChanged(int)),
          SLOT(setFilter(int)));

  connect(m_searchInput, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchBtn, SIGNAL(clicked()), SLOT(setSearch()));

  setFilter(0);
}

const QString CDsVinylSearchBar::getSearchString(const QStringList &fields) {
  QString _sql;
  QString _input = m_searchInput->text().trimmed();
  if (_input.isEmpty() || fields.count() < 1) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "INVALID_CDVINYL_SEARCH_INPUT";
#endif
    return QString("cv_title='INVALID_CDVINYL_SEARCH_INPUT'");
  }

  // Standard Suchfeld
  if (_input.length() >= getMinLength()) {
    QStringList _bufLeft;
    foreach (QString f, fields) {
      QString fset = prepareFieldSearch(f, _input);
      if (fset.isEmpty())
        continue;

      _bufLeft << fset;
    }
    if (_bufLeft.count() > 0) {
      _sql.append("(");
      _sql.append(_bufLeft.join(" OR "));
      _sql.append(")");
    }
    _bufLeft.clear();
  }

  return _sql;
}

bool CDsVinylSearchBar::lineInputsEnabled() {
  return m_searchInput->isEnabled();
}

void CDsVinylSearchBar::setSearch() {
  QString _filter = m_selectFilter->currentFilter();
  if (_filter.isEmpty())
    return;

  if (_filter.contains(CDSVINYL_INDEX_COLUMN)) {
    emit sendSearchClicked();
  } else if (lineInputsEnabled() && requiredLengthExists()) {
    emit sendSearchClicked();
  } else {
    emit sendNotify(tr("Your input is too short, increase your search!"));
  }
}

void CDsVinylSearchBar::setFilter(int index) {
  m_searchInput->clear();
  m_searchInput->setEnabled(true);
  m_searchInput->setToolTip(QString());

  const QString _tip = m_selectFilter->currentToolTip(index);
  const QString _filter = m_selectFilter->currentFilter(index);

  if (_filter.contains(CDSVINYL_INDEX_COLUMN) || _filter.contains("cv_since")) {
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

void CDsVinylSearchBar::setFilterFocus() {
  m_selectFilter->setFocus();
  m_selectFilter->showPopup();
}

void CDsVinylSearchBar::setClearAndFocus() {
  m_searchInput->clear();
  m_searchInput->setFocus();
}

void CDsVinylSearchBar::setSearchFocus() { setClearAndFocus(); }

int CDsVinylSearchBar::searchLength() { return m_searchInput->length(); }

bool CDsVinylSearchBar::requiredLengthExists() {
  return (searchLength() >= getMinLength());
}

const QString CDsVinylSearchBar::getSearchStatement() {
  const QString _data = m_selectFilter->currentFilter();
  const QString _input = m_searchInput->text().trimmed();
  if (_data.isEmpty() || _input.isEmpty()) {
    qWarning("No filter found.");
    emit sendNotify(tr("Invalid search input!"));
    return QString();
  }

  // start SqlClause with or without stock
  QString _sql(withStock() ? "cv_count>0 AND " : "");

  // im_id Image filter
  if (!imageFilter().isEmpty()) {
    _sql.append(imageFilter() + " AND ");
  }

  // table columns from filter
  QStringList _cols = _data.split(",");

  // default string search patterns
  if (_cols.contains("cv_title") || _cols.contains("cv_author")) {
    _sql.append(getSearchString(_cols));
    return _sql;
  }

  // article search e.g.(104820,82490,43310)
  static const QRegularExpression numberPattern("[\\D]+");
  if (_cols.contains(CDSVINYL_INDEX_COLUMN)) {
    QString _str = _input;
    _str.replace(numberPattern, ",");
    if (_str.isEmpty())
      return QString();

    _sql.append("cv_id IN (" + _str + ")");
    _str.clear();
    return _sql;
  }

  // year search - since and modified
  if (_cols.contains("cv_year")) {
    QStringList _buf;
    QString _str = _input;
    _str.replace(numberPattern, ",");
    if (_str.isEmpty())
      return QString();

    _buf << "DATE_PART('YEAR',cv_since) IN (" + _str + ")";
    _buf << "DATE_PART('YEAR',cv_changed) IN (" + _str + ")";
    _sql.append("(" + _buf.join(" OR ") + ")");
    _str.clear();
    return _sql;
  }

  // genre - keyword search
  if (_data.contains("cv_keyword")) {
    QString _str = _input;
    _str.replace(jokerPattern, "%");
    _str = prepareFieldSearch("cv_keyword", _str);
    _sql.append("(" + _str + ")");
    _str.clear();
    return _sql;
  }

  qWarning("No defined Search (%s)", qPrintable(_data));
  return QString();
}


