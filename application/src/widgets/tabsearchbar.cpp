// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsearchbar.h"

#ifndef SEARCH_PATTERN_BOX
#define SEARCH_PATTERN_BOX QString("tab_search_bar_search_confiness")
#endif

#include <QIcon>

TabSearchBar::TabSearchBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setContentsMargins(0, 0, 0, 0);
}

void TabSearchBar::setSearchStockEnabled(bool b) {
  SearchWithStock = b;
  emit sendStockEnabled(b);
}

void TabSearchBar::setSearchPatternChanged(int i) {
  if (i > 4 || i < 0)
    return;

  TabSearchBar::SearchPattern sp = static_cast<TabSearchBar::SearchPattern>(i);
  emit sendMatchChanged(sp);
}

QPushButton *TabSearchBar::startSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(this);
  btn->setIcon(QIcon(":icons/action_search.png"));
  btn->setToolTip(tr("Push to start search."));

  if (text.isEmpty())
    btn->setText(tr("start search"));
  else
    btn->setText(text);

  return btn;
}

QCheckBox *TabSearchBar::stockCheckBox(const QString &text) {
  QCheckBox *m_box = new QCheckBox(this);
  QString title = (text.isEmpty()) ? tr("Stock") : text;
  m_box->setText(title);
  m_box->setToolTip(tr("Only search with %1").arg(title));
  connect(m_box, SIGNAL(toggled(bool)), SLOT(setSearchStockEnabled(bool)));
  return m_box;
}

QPushButton *TabSearchBar::customSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(text, this);
  btn->setIcon(QIcon("://icons/action_search.png"));
  btn->setToolTip(tr("Custom queries"));
  btn->setStatusTip(tr("Click to open Dialog for custom queries."));
  connect(btn, SIGNAL(clicked()), SIGNAL(sendOpenCustomSearch()));
  return btn;
}

QComboBox *TabSearchBar::searchConfines() {
  QComboBox *box = new QComboBox(this);
  box->setObjectName(SEARCH_PATTERN_BOX);
  box->setToolTip(tr("Limit search to"));
  box->setStatusTip(box->toolTip());
  box->insertItem(TabSearchBar::SearchPattern::ANYWHERE, tr("search anywhere"));
  box->insertItem(TabSearchBar::SearchPattern::BOUNDARIES, tr("search exact"));
  box->insertItem(TabSearchBar::SearchPattern::BEGINNING,
                  tr("search at the beginning"));
  box->insertItem(TabSearchBar::SearchPattern::ENDING, tr("search at the end"));
  connect(box, SIGNAL(currentIndexChanged(int)),
          SLOT(setSearchPatternChanged(int)));
  return box;
}

const QString TabSearchBar::prepareFieldSet(const QString &fieldname,
                                            const QString &search) {
  QString sf(search.trimmed());
  sf = sf.replace(jokerPattern, "%");

  if (sf.length() < getMinLength())
    return QString();

  QString _prefix, _suffix;
  switch (currentSearchSyntax()) {
  case (TabSearchBar::SearchPattern::ANYWHERE):
    _prefix = (fieldname + " ILIKE '%");
    _suffix = "%'";
    break;

  case (TabSearchBar::SearchPattern::BOUNDARIES):
    _prefix = (fieldname + " ILIKE '");
    _suffix = "'";
    break;

  case (TabSearchBar::SearchPattern::BEGINNING):
    _prefix = (fieldname + " ILIKE '");
    _suffix = "%'";
    break;

  case (TabSearchBar::SearchPattern::ENDING):
    _prefix = (fieldname + " ILIKE '%");
    _suffix = "'";
    break;

  default:
    break;
  };

  QStringList words = search.trimmed().split(" ");
  QString sql;
  if (words.count() == 2) {
    // start
    sql.append(_prefix);
    sql.append(words.first());
    sql.append("%");
    sql.append(words.last());
    sql.append(_suffix);
    sql.append(" OR " + _prefix);
    sql.append(words.last());
    sql.append("%");
    sql.append(words.first());
    sql.append(_suffix);
  } else {
    sql.append(_prefix);
    sql.append(sf);
    sql.append(_suffix);
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("SEARCH: (%s)", qPrintable(sql));
#endif
  return sql;
}

void TabSearchBar::setMinLength(int l) {
  if (l != minLength)
    emit sendMinLengthChanged(l);

  minLength = l;
}

TabSearchBar::SearchPattern TabSearchBar::currentSearchSyntax() const {
  QComboBox *box = findChild<QComboBox *>(SEARCH_PATTERN_BOX);
  if (box == nullptr)
    return TabSearchBar::SearchPattern::ANYWHERE;

  if (box->count() == 4 && box->currentIndex() > 0)
    return static_cast<TabSearchBar::SearchPattern>(box->currentIndex());

  return TabSearchBar::SearchPattern::ANYWHERE;
}

int TabSearchBar::getMinLength() { return minLength; }
