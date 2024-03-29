// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabssearchbar.h"
#include "antiquaicon.h"

#include <ASettings>

namespace AntiquaCRM {

TabsSearchBar::TabsSearchBar(QWidget *parent) : QToolBar{parent} {
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setStyleSheet("QToolBar {margin: 5px; spacing:3px;}");

  AntiquaCRM::ASettings cfg(this);
  p_minLength = cfg.value("search/startlength", 5).toInt();
}

void TabsSearchBar::searchPatternChanged(int index) {
  if (index > 4 || index < 0)
    return;

  TabsSearchBar::SearchPattern sp =
      static_cast<TabsSearchBar::SearchPattern>(index);
  emit sendSearchPattern(sp);
}

void TabsSearchBar::imageFilterChanged(int index) {
  AntiquaCRM::AComboBox *m_box =
      findChild<AntiquaCRM::AComboBox *>("search_image_filter");
  if (m_box == nullptr)
    return;

  int _index = m_box->itemData(index, Qt::UserRole).toInt();
  if (_index >= 0)
    emit sendImageFilter(_index);
}

QPushButton *TabsSearchBar::startSearchButton(const QString &text) {
  QPushButton *btn = new QPushButton(this);
  btn->setIcon(AntiquaCRM::antiquaIcon("system-search"));
  btn->setToolTip(tr("Push to start search."));

  if (text.isEmpty())
    btn->setText(tr("Search"));
  else
    btn->setText(text);

  return btn;
}

QCheckBox *TabsSearchBar::stockCheckBox(const QString &title) {
  QCheckBox *m_box = new QCheckBox(this);
  m_box->setObjectName("search_with_stock");
  QString _t = (title.isEmpty()) ? tr("Stock") : title.trimmed();
  m_box->setText(_t);
  m_box->setToolTip(tr("Display with %1 only.").arg(title));
  connect(m_box, SIGNAL(toggled(bool)), SIGNAL(sendWithStockEnabled(bool)));
  return m_box;
}

AntiquaCRM::AComboBox *TabsSearchBar::searchImageOptions() {
  const QIcon _icon = AntiquaCRM::antiquaIcon("view-image");
  AntiquaCRM::AComboBox *m_box = new AntiquaCRM::AComboBox(this);
  m_box->setObjectName("search_image_filter");
  m_box->addItem(_icon, tr("Image Filter"), 0);
  m_box->addItem(_icon, tr("Only with image"), 1);
  m_box->addItem(_icon, tr("Without images"), 2);
  m_box->setToolTip(tr("Search only entries with Images."));
  connect(m_box, SIGNAL(currentIndexChanged(int)),
          SLOT(imageFilterChanged(int)));
  return m_box;
}

QPushButton *TabsSearchBar::customSearchButton(const QString &title) {
  QPushButton *btn = new QPushButton(title, this);
  btn->setIcon(AntiquaCRM::antiquaIcon("system-search"));
  btn->setToolTip(tr("Custom queries"));
  btn->setStatusTip(tr("Click to open Dialog for custom queries."));
  connect(btn, SIGNAL(clicked()), SIGNAL(sendOpenCustomSearch()));
  return btn;
}

QComboBox *TabsSearchBar::searchConfines() {
  QString info = tr("Limit output");
  QIcon icon = AntiquaCRM::antiquaIcon("view-search");
  QComboBox *box = new QComboBox(this);
  box->setObjectName("select_search_confiness");
  box->setToolTip(tr("Search boundaries"));
  box->setStatusTip(info + "...");
  // Irgendwo im Satz "%suche%wort%" (Standard)
  box->insertItem(TabsSearchBar::SearchPattern::ANYWHERE, box->toolTip());
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
  const QString _search(search.trimmed());
  if (_search.length() < getMinLength()) {
    qWarning("INVALID_QUERY_STATEMENT:(%s='%s')", qPrintable(field),
             qPrintable(search));
    if (_search.isEmpty())
      return QString(field + "='INVALID_QUERY_STATEMENT'");

    // strict fallback
    return QString(field + " ILIKE '" + _search + "'");
  }

  // search boundaries
  QString _sb_left, _sb_right;
  // buffer lists
  QStringList _b_first, _b_last, _query;

  switch (searchPattern()) {
  case (TabsSearchBar::SearchPattern::ANYWHERE):
    _sb_left = (field + " ILIKE '%");
    _sb_right = "%'";
    break;

  case (TabsSearchBar::SearchPattern::BOUNDARIES):
    _sb_left = (field + " ILIKE '");
    _sb_right = "'";
    break;

  case (TabsSearchBar::SearchPattern::BEGINNING):
    _sb_left = (field + " ILIKE '");
    _sb_right = "%'";
    break;

  case (TabsSearchBar::SearchPattern::ENDING):
    _sb_left = (field + " ILIKE '%");
    _sb_right = "'";
    break;

  default:
    break;
  };

  QString _sql;
  _query = search.trimmed().split(" ");

  if (_query.count() >= 2) {
    foreach (QString w, _query) {
      _b_first.append(w.trimmed());
      _b_last.prepend(w.trimmed());
    }
    // prepare
    _sql.append(_sb_left);
    _sql.append(_b_first.join("%"));
    _sql.append(_sb_right);
    _sql.append(" OR ");
    _sql.append(_sb_left);
    _sql.append(_b_last.join("%"));
    _sql.append(_sb_right);
  } else {
    _sql.append(_sb_left);
    _sql.append(_search);
    _sql.append(_sb_right);
  }

  // clear buffers
  _query.clear();
  _b_first.clear();
  _b_last.clear();

  return _sql.replace(jokerPattern, "%");
}

void TabsSearchBar::setMinLength(int l) {
  if (l != p_minLength)
    emit sendMinLengthChanged(l);

  p_minLength = l;
}

TabsSearchBar::SearchPattern TabsSearchBar::searchPattern() const {
  QComboBox *box = findChild<QComboBox *>("select_search_confiness");
  if ((box == nullptr) || (box->currentIndex() < 1))
    return TabsSearchBar::SearchPattern::ANYWHERE;

  return static_cast<TabsSearchBar::SearchPattern>(box->currentIndex());
}

const QString TabsSearchBar::imageFilter() {
  QString _filter;
  AntiquaCRM::AComboBox *m_box =
      findChild<AntiquaCRM::AComboBox *>("search_image_filter");
  if (m_box == nullptr)
    return _filter;

  int _index = m_box->itemData(m_box->currentIndex(), Qt::UserRole).toInt();
  switch (static_cast<TabsSearchBar::ImageFilter>(_index)) {
  case (TabsSearchBar::ImageFilter::WITH_IMAGE): {
    _filter = QString("im_id IS NOT NULL");
  } break;

  case (TabsSearchBar::ImageFilter::WITHOUT_IMAGE): {
    _filter = QString("im_id IS NULL");
  } break;

  default:
    _filter = QString();
    break;
  }
  return _filter;
}

int TabsSearchBar::getMinLength() { return p_minLength; }

bool TabsSearchBar::withStock() {
  QCheckBox *box = findChild<QCheckBox *>("search_with_stock");
  return (box == nullptr) ? false : box->isChecked();
}

} // namespace AntiquaCRM
