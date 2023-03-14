// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabellist.h"
#include "keywordlabel.h"

#include <QList>

KeywordLabelList::KeywordLabelList(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  setAcceptDrops(false);
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(1, 1, 1, 1);
  setLayout(m_layout);
}

void KeywordLabelList::removeKeyword() {
  KeywordLabel *klb = qobject_cast<KeywordLabel *>(sender());
  if (klb != nullptr) {
    QString key = klb->text();
    for (int i = 0; i < p_uniqList.count(); i++) {
      if (p_uniqList.at(i) == key)
        p_uniqList.removeAt(i);
    }
    m_layout->removeWidget(klb);
    klb->deleteLater();
    updateToolTip();
    emit sendModified(true);
  }
}

void KeywordLabelList::updateToolTip() {
  QStringList list = keywords();
  QString tip = tr("Current Keywords length %1 from allowed %2 used.")
                    .arg(list.join(",").size())
                    .arg(p_maxLength);
  setToolTip(tip);
}

void KeywordLabelList::clear() {
  QList<KeywordLabel *> l = findChildren<KeywordLabel *>(QString());
  for (int i = 0; i < l.count(); i++) {
    KeywordLabel *klb = l.at(i);
    m_layout->removeWidget(klb);
    klb->deleteLater();
  }
  update();
  p_uniqList.clear();
  updateToolTip();
}

void KeywordLabelList::setMaxLength(int i) {
  if (i > 1)
    p_maxLength = i;
}

void KeywordLabelList::addKeyword(const QString &keyword) {
  QString key = keyword.trimmed();
  if (p_uniqList.contains(key))
    return;

  KeywordLabel *klb = new KeywordLabel(key, this);
  m_layout->addWidget(klb, Qt::AlignLeft);
  connect(klb, SIGNAL(aboutToRemove()), SLOT(removeKeyword()));
  p_uniqList << key;
  updateToolTip();
  emit sendModified(true);
}

void KeywordLabelList::addKeywords(const QStringList &keywords) {
  clear();
  foreach (QString k, keywords) {
    addKeyword(k.trimmed());
  }
}

int KeywordLabelList::maxLength() { return p_maxLength; }

const QStringList KeywordLabelList::keywords() { return p_uniqList; }
