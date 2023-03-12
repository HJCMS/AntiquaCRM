// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabellist.h"
#include "keywordlabel.h"

#include <QDebug>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QSizePolicy>

KeywordLabelList::KeywordLabelList(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(2, 2, 2, 2);
  setLayout(m_layout);
}

void KeywordLabelList::removeKeyword() {
  KeywordLabel *lb = qobject_cast<KeywordLabel *>(sender());
  if (lb != nullptr) {
    QString key = lb->text();
    m_layout->removeWidget(lb);
    lb->deleteLater();
    for (int i = 0; i < p_uniqList.count(); i++) {
      if (p_uniqList.at(i) == key)
        p_uniqList.removeAt(i);
    }
    emit sendModified(true);
  }
}

void KeywordLabelList::clear() {
  QList<KeywordLabel *> l = findChildren<KeywordLabel *>(QString());
  for (int i = 0; i < l.count(); i++) {
    KeywordLabel *lb = l.at(i);
    m_layout->removeWidget(lb);
    lb->deleteLater();
  }
  update();
  p_uniqList.clear();
}

void KeywordLabelList::addKeyword(const QString &keyword) {
  QString key = keyword.trimmed();
  if (p_uniqList.contains(key))
    return;

  KeywordLabel *lb = new KeywordLabel(this);
  if (lb != nullptr) {
    lb->setText(key);
    connect(lb, SIGNAL(aboutToRemove()), SLOT(removeKeyword()));
    m_layout->addWidget(lb, Qt::AlignLeft);
    p_uniqList << key;
  }
}

void KeywordLabelList::addKeywords(const QStringList &keywords) {
  clear();
  foreach (QString k, keywords) {
    addKeyword(k.trimmed());
  }
}

const QStringList KeywordLabelList::keywords() { return p_uniqList; }
