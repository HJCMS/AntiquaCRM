// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsearchbar.h"

#include <ASettings>
#include <QIcon>

TabSearchBar::TabSearchBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setContentsMargins(0, 0, 0, 0);

  AntiquaCRM::ASettings cfg(this);
  p_minLength = cfg.value("search/startlength", 5).toInt();
}

void TabSearchBar::searchPatternChanged(int i) {
  if (i > 4 || i < 0)
    return;

  TabSearchBar::SearchPattern sp = static_cast<TabSearchBar::SearchPattern>(i);
  emit sendSearchPattern(sp);
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
  m_box->setObjectName("search_with_stock");
  QString title = (text.isEmpty()) ? tr("Stock") : text;
  m_box->setText(title);
  m_box->setToolTip(tr("Only search with %1").arg(title));
  connect(m_box, SIGNAL(toggled(bool)), SIGNAL(sendStockEnabled(bool)));
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
  QString info = tr("Limit search to");
  QIcon icon("://icons/view_search.png");
  QComboBox *box = new QComboBox(this);
  box->setObjectName("select_search_confiness");
  box->setToolTip(tr("Select a search confiness"));
  box->setStatusTip(info + "...");
  // Irgendwo im Satz "%suche%wort%" (Standard)
  box->insertItem(TabSearchBar::SearchPattern::ANYWHERE, tr("default filter"));
  box->setItemData(TabSearchBar::SearchPattern::ANYWHERE,
                   info + " " + tr("search anywhere") + ".", Qt::ToolTipRole);
  box->setItemData(TabSearchBar::SearchPattern::ANYWHERE, icon,
                   Qt::DecorationRole);
  // Genauer Wortlaut "suche%wort"
  box->insertItem(TabSearchBar::SearchPattern::BOUNDARIES, tr("exact"));
  box->setItemData(TabSearchBar::SearchPattern::BOUNDARIES,
                   info + " " + tr("search exact") + ".", Qt::ToolTipRole);
  box->setItemData(TabSearchBar::SearchPattern::BOUNDARIES, icon,
                   Qt::DecorationRole);
  // Am Satzanfang "wort%"
  box->insertItem(TabSearchBar::SearchPattern::BEGINNING, tr("beginning"));
  box->setItemData(TabSearchBar::SearchPattern::BEGINNING,
                   info + " " + tr("beginning of sentence") + ".",
                   Qt::ToolTipRole);
  box->setItemData(TabSearchBar::SearchPattern::BEGINNING, icon,
                   Qt::DecorationRole);
  // Am Satzende "%wort"
  box->insertItem(TabSearchBar::SearchPattern::ENDING, tr("end"));
  box->setItemData(TabSearchBar::SearchPattern::ENDING,
                   info + " " + tr("end of sentence") + ".", Qt::ToolTipRole);
  box->setItemData(TabSearchBar::SearchPattern::ENDING, icon,
                   Qt::DecorationRole);

  connect(box, SIGNAL(currentIndexChanged(int)),
          SLOT(searchPatternChanged(int)));
  return box;
}

const QString TabSearchBar::prepareFieldSearch(const QString &field,
                                               const QString &search) {
  QString sf(search.trimmed());
  if (sf.length() < getMinLength()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("INVALID_QUERY_STATEMENT:(%s='%s')", qPrintable(field),
             qPrintable(search));
#endif
    if (sf.isEmpty())
      return QString(field + "='INVALID_QUERY_STATEMENT'");
    else
      return QString(field + "='" + sf + "'");
  }

  QString _prefix, _suffix;
  QStringList forward, backward, query;

  switch (searchPattern()) {
  case (TabSearchBar::SearchPattern::ANYWHERE):
    _prefix = (field + " ILIKE '%");
    _suffix = "%'";
    break;

  case (TabSearchBar::SearchPattern::BOUNDARIES):
    _prefix = (field + " ILIKE '");
    _suffix = "'";
    break;

  case (TabSearchBar::SearchPattern::BEGINNING):
    _prefix = (field + " ILIKE '");
    _suffix = "%'";
    break;

  case (TabSearchBar::SearchPattern::ENDING):
    _prefix = (field + " ILIKE '%");
    _suffix = "'";
    break;

  default:
    break;
  };

  QString sql;
  query = search.trimmed().split(" ");

  if (query.count() >= 2) {
    foreach (QString w, query) {
      forward.append(w.trimmed());
      backward.prepend(w.trimmed());
    }
    // prepare
    sql.append(_prefix);
    sql.append(forward.join("%"));
    sql.append(_suffix);
    sql.append(" OR ");
    sql.append(_prefix);
    sql.append(backward.join("%"));
    sql.append(_suffix);
  } else {
    sql.append(_prefix);
    sql.append(sf);
    sql.append(_suffix);
  }

  // clear buffer
  query.clear();
  forward.clear();
  backward.clear();

#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("SEARCH:(%s)", qPrintable(sql.replace(jokerPattern, "%")));
#endif

  return sql.replace(jokerPattern, "%");
}

void TabSearchBar::setMinLength(int l) {
  if (l != p_minLength)
    emit sendMinLengthChanged(l);

  p_minLength = l;
}

TabSearchBar::SearchPattern TabSearchBar::searchPattern() const {
  QComboBox *box = findChild<QComboBox *>("select_search_confiness");
  if (box == nullptr)
    return TabSearchBar::SearchPattern::ANYWHERE;

  if (box->count() == 4 && box->currentIndex() > 0)
    return static_cast<TabSearchBar::SearchPattern>(box->currentIndex());

  return TabSearchBar::SearchPattern::ANYWHERE;
}

int TabSearchBar::getMinLength() { return p_minLength; }

bool TabSearchBar::withStock() {
  QCheckBox *box = findChild<QCheckBox *>("search_with_stock");
  return (box == nullptr) ? false : box->isChecked();
}
