// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabellist.h"
#include "keywordlabel.h"

#include <QDebug>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QSizePolicy>

static const QString lableStyle() {
  QStringList css;
  css << "padding-left:5px;padding-right:5px;";
  return "KeywordLabel {" + css.join(" ") + "}";
}

KeywordLabelList::KeywordLabelList(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);
  setStyleSheet(lableStyle());
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(2, 2, 2, 2);
  setLayout(m_layout);
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

  KeywordLabel *lb = new KeywordLabel(key, this);
  if (lb != nullptr) {
    m_layout->addWidget(lb, Qt::AlignLeft);
    p_uniqList << key;
  }
}

void KeywordLabelList::addKeywords(const QStringList &keywords) {
  clear();
  foreach (QString k, keywords) {
    addKeyword(k);
  }
}

const QStringList KeywordLabelList::keywords() { return p_uniqList; }
