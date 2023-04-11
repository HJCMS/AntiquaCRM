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
  setWindowModified(false);
}

void KeywordListView::insertKeyword(const QString &keyword) {
  p_uniqList.clear();
  QString _k = keyword.trimmed();
  if (p_uniqList.contains(_k) || _k.isEmpty())
    return;

  KeywordLabel *klb = new KeywordLabel(_k, this);
  layout->addWidget(klb);
  connect(klb, SIGNAL(aboutToRemove()), SLOT(removeKeyword()));
  p_uniqList << _k;
  setToolTip(getToolTip());
  emit valueChanged();
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
    emit valueChanged();
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
    emit valueChanged();
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
                     .arg(length())
                     .arg(maxLength());
  return _tip;
}

bool KeywordListView::isValid() {
  return (length() >= p_minLength && length() <= p_maxLength);
}

qsizetype KeywordListView::length() { return p_uniqList.join(",").size(); }

qsizetype KeywordListView::maxLength() { return p_maxLength; }

} // namespace AntiquaCRM
