// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "varioussearchbar.h"

#include <QIcon>
#include <QJsonObject>
#include <QRegularExpression>
#include <QToolButton>

VariousSearchBar::VariousSearchBar(QWidget *parent)
    : AntiquaCRM::TabsSearchBar{parent} {

  QToolButton *m_icontb = new QToolButton(this);
  m_icontb->setEnabled(false);
  m_icontb->setIcon(getIcon("action-search"));
  addWidget(m_icontb);

  m_searchLine = new AntiquaCRM::ALineEdit(this);
  m_searchLine->setPlaceholderText(tr("Default search"));
  addWidget(m_searchLine);

  addWidget(stockCheckBox());

  m_searchButton = startSearchButton();
  addWidget(m_searchButton);

  connect(m_searchLine, SIGNAL(returnPressed()), SLOT(setSearch()));
  connect(m_searchButton, SIGNAL(clicked()), SLOT(setSearch()));
}

void VariousSearchBar::setSearch() {
  if (searchLength() >= getMinLength())
    emit sendSearchClicked();
}

void VariousSearchBar::setFilter(int) {}

void VariousSearchBar::setFilterFocus() { m_searchLine->setFocus(); }

void VariousSearchBar::setClearAndFocus() {
  m_searchLine->clear();
  m_searchLine->setFocus();
}

void VariousSearchBar::setSearchFocus() { setClearAndFocus(); }

int VariousSearchBar::searchLength() { return m_searchLine->text().length(); }

const QString VariousSearchBar::getSearchStatement() {
  QString _input = m_searchLine->text().trimmed();
  if (_input.isEmpty())
    return QString("va_changed=CURRENT_DATE AND va_count>0");

  QString _sql;
  _sql.append("va_title ILIKE '%" + _input + "%' OR ");
  _sql.append("va_title_extended ILIKE '%" + _input + "%' OR ");
  _sql.append("va_description ILIKE '%" + _input + "%'");
  if (withStock())
    _sql.append(" AND va_count>0");

  return _sql;
}
