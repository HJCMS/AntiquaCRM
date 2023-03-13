// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabel.h"
#include "keywordlabellist.h"

#include <QDebug>
#include <QMimeData>
#include <QHBoxLayout>
#include <QPalette>
#include <QSizePolicy>
#include <QToolButton>

KeywordLabel::KeywordLabel(KeywordLabelList *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Plain);
  setStyleSheet(styleSheet());
  setAcceptDrops(false);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setObjectName("keyword::layout");
  layout->setContentsMargins(0, 0, 0, 0);

  lable = new QLabel(this);
  lable->setObjectName("keyword::lable");
  lable->setTextInteractionFlags(Qt::NoTextInteraction);
  layout->addWidget(lable);

  QToolButton *m_close = new QToolButton(this);
  m_close->setObjectName("keyword::button");
  m_close->setIcon(QIcon(":/icons/action_cancel.png"));
  m_close->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_close->setToolTip(tr("Remove this keyword"));
  layout->addWidget(m_close);

  setLayout(layout);
  connect(m_close, SIGNAL(clicked()), this, SIGNAL(aboutToRemove()));
}

KeywordLabel::KeywordLabel(const QString &text, KeywordLabelList *parent)
    : KeywordLabel{parent} {
  setText(text);
}

const QString KeywordLabel::styleSheet() const {
  QString color = palette().color(QPalette::Text).name();
  QStringList css;
  css << "padding-left:2px;padding-right:2px;";
  css << "padding-top:1px;padding-bottom:1px;";
  css << "border:1px solid " + color + ";border-radius:3px;";
  return "KeywordLabel {" + css.join("") + "}";
}

void KeywordLabel::setText(const QString &txt) {
  lable->setText(txt);
  emit textChanged();
}

const QString KeywordLabel::text() { return lable->text(); }
