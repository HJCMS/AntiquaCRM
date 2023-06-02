// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabssearchbar.h"

#include <ASettings>
#include <AntiquaInput>

namespace AntiquaCRM {

TabsSearchBar::TabsSearchBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setStyleSheet("QToolBar {margin: 5px; spacing:3px;}");

  AntiquaCRM::ASettings cfg(this);
  p_minLength = cfg.value("search/startlength", 5).toInt();
}

void TabsSearchBar::searchPatternChanged(int i) {
  if (i > 4 || i < 0)
    return;

  TabsSearchBar::SearchPattern sp =
      static_cast<TabsSearchBar::SearchPattern>(i);
  emit sendSearchPattern(sp);
}

QPushButton *TabsSearchBar::startSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(this);
  btn->setIcon(getIcon("action-search"));
  btn->setToolTip(tr("Push to start search."));

  if (text.isEmpty())
    btn->setText(tr("start search"));
  else
    btn->setText(text);

  return btn;
}

QCheckBox *TabsSearchBar::stockCheckBox(const QString &text) {
  QCheckBox *m_box = new QCheckBox(this);
  m_box->setObjectName("search_with_stock");
  QString title = (text.isEmpty()) ? tr("Stock") : text;
  m_box->setText(title);
  m_box->setToolTip(tr("Only search with %1").arg(title));
  connect(m_box, SIGNAL(toggled(bool)), SIGNAL(sendStockEnabled(bool)));
  return m_box;
}

QPushButton *TabsSearchBar::customSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(text, this);
  btn->setIcon(getIcon("action-search"));
  btn->setToolTip(tr("Custom queries"));
  btn->setStatusTip(tr("Click to open Dialog for custom queries."));
  connect(btn, SIGNAL(clicked()), SIGNAL(sendOpenCustomSearch()));
  return btn;
}

QComboBox *TabsSearchBar::searchConfines() {
  QString info = tr("Limit search to");
  QIcon icon = getIcon("view-search");
  QComboBox *box = new QComboBox(this);
  box->setObjectName("select_search_confiness");
  box->setToolTip(tr("Select a search confiness"));
  box->setStatusTip(info + "...");
  // Irgendwo im Satz "%suche%wort%" (Standard)
  box->insertItem(TabsSearchBar::SearchPattern::ANYWHERE, tr("default filter"));
  box->setItemData(TabsSearchBar::SearchPattern::ANYWHERE,
                   info + " " + tr("search anywhere") + ".", Qt::ToolTipRole);
  box->setItemData(TabsSearchBar::SearchPattern::ANYWHERE, icon,
                   Qt::DecorationRole);
  // Genauer Wortlaut "suche%wort"
  box->insertItem(TabsSearchBar::SearchPattern::BOUNDARIES, tr("exact"));
  box->setItemData(TabsSearchBar::SearchPattern::BOUNDARIES,
                   info + " " + tr("search exact") + ".", Qt::ToolTipRole);
  box->setItemData(TabsSearchBar::SearchPattern::BOUNDARIES, icon,
                   Qt::DecorationRole);
  // Am Satzanfang "wort%"
  box->insertItem(TabsSearchBar::SearchPattern::BEGINNING, tr("beginning"));
  box->setItemData(TabsSearchBar::SearchPattern::BEGINNING,
                   info + " " + tr("beginning of sentence") + ".",
                   Qt::ToolTipRole);
  box->setItemData(TabsSearchBar::SearchPattern::BEGINNING, icon,
                   Qt::DecorationRole);
  // Am Satzende "%wort"
  box->insertItem(TabsSearchBar::SearchPattern::ENDING, tr("end"));
  box->setItemData(TabsSearchBar::SearchPattern::ENDING,
                   info + " " + tr("end of sentence") + ".", Qt::ToolTipRole);
  box->setItemData(TabsSearchBar::SearchPattern::ENDING, icon,
                   Qt::DecorationRole);

  connect(box, SIGNAL(currentIndexChanged(int)),
          SLOT(searchPatternChanged(int)));
  return box;
}

const QString TabsSearchBar::prepareFieldSearch(const QString &field,
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
  case (TabsSearchBar::SearchPattern::ANYWHERE):
    _prefix = (field + " ILIKE '%");
    _suffix = "%'";
    break;

  case (TabsSearchBar::SearchPattern::BOUNDARIES):
    _prefix = (field + " ILIKE '");
    _suffix = "'";
    break;

  case (TabsSearchBar::SearchPattern::BEGINNING):
    _prefix = (field + " ILIKE '");
    _suffix = "%'";
    break;

  case (TabsSearchBar::SearchPattern::ENDING):
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
  qDebug() << Q_FUNC_INFO << sql.replace(jokerPattern, "%");
#endif

  return sql.replace(jokerPattern, "%");
}

void TabsSearchBar::setMinLength(int l) {
  if (l != p_minLength)
    emit sendMinLengthChanged(l);

  p_minLength = l;
}

const QIcon TabsSearchBar::getIcon(const QString &name) {
  return AntiquaCRM::AntiquaApplIcon(name);
}

TabsSearchBar::SearchPattern TabsSearchBar::searchPattern() const {
  QComboBox *box = findChild<QComboBox *>("select_search_confiness");
  if (box == nullptr)
    return TabsSearchBar::SearchPattern::ANYWHERE;

  if (box->count() == 4 && box->currentIndex() > 0)
    return static_cast<TabsSearchBar::SearchPattern>(box->currentIndex());

  return TabsSearchBar::SearchPattern::ANYWHERE;
}

int TabsSearchBar::getMinLength() { return p_minLength; }

bool TabsSearchBar::withStock() {
  QCheckBox *box = findChild<QCheckBox *>("search_with_stock");
  return (box == nullptr) ? false : box->isChecked();
}

} // namespace AntiquaCRM
