// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlistview.h"
#include "keywordlabel.h"

namespace AntiquaCRM {

KeywordListView::KeywordListView(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}

void KeywordListView::setKeywordList(const QStringList &list) {
  foreach (QString _k, list) {
    if (p_uniqList.contains(_k) || _k.isEmpty())
      continue;

    KeywordLabel *klb = new KeywordLabel(_k, this);
    layout->addWidget(klb);
    connect(klb, SIGNAL(aboutToRemove()), SLOT(removeKeyword()));
    p_uniqList << _k;
  }
  setToolTip(getToolTip());
}

void KeywordListView::insertKeyword(const QString &keyword) {
  p_uniqList.clear();
  QString _k = keyword.trimmed();
  if (p_uniqList.contains(_k) || _k.isEmpty())
    return;

  const QString _keyword = keyword.trimmed();
  p_uniqList << _keyword;
  KeywordLabel *klb = new KeywordLabel(_keyword, this);
  layout->addWidget(klb);
  connect(klb, SIGNAL(aboutToRemove()), SLOT(removeKeyword()));
  setToolTip(getToolTip());
  emit keywordsChanged();
}

void KeywordListView::removeKeyword() {
  KeywordLabel *klb = qobject_cast<KeywordLabel *>(sender());
  if (klb != nullptr) {
    QString key = klb->text();
    for (int i = 0; i < p_uniqList.count(); i++) {
      if (p_uniqList.at(i) == key)
        p_uniqList.removeAt(i);
    }
    layout->removeWidget(klb);
    klb->deleteLater();
    setToolTip(getToolTip());
    emit keywordsChanged();
  }
}

void KeywordListView::clearKeywords() {
  QList<KeywordLabel *> _list = findChildren<KeywordLabel *>(QString());
  if (_list.size() > 0) {
    QListIterator<KeywordLabel *> it(_list);
    while (it.hasNext()) {
      KeywordLabel *lb = it.next();
      layout->removeWidget(lb);
      lb->deleteLater();
    }
    _list.clear();
    p_uniqList.clear();
    emit keywordsChanged();
  }
}

const QString KeywordListView::getKeywords() {
  QStringList _keys;
  QList<KeywordLabel *> _list = findChildren<KeywordLabel *>(QString());
  if (_list.size() > 0) {
    QListIterator<KeywordLabel *> it(_list);
    while (it.hasNext()) {
      KeywordLabel *lb = it.next();
      _keys << lb->text();
    }
    _list.clear();
  }
  return _keys.join(",");
}

const QString KeywordListView::getToolTip() {
  QString _tip = tr("Current Keywords length %1 from allowed %2 used.")
                     .arg(curLength())
                     .arg(maxLength());
  return _tip;
}

bool KeywordListView::isValid() {
  int _len = curLength();
  return (_len >= minLength() && _len <= maxLength());
}

int KeywordListView::curLength() { return getKeywords().size(); }

int KeywordListView::minLength() { return 5; }

int KeywordListView::maxLength() { return 60; }

} // namespace AntiquaCRM
