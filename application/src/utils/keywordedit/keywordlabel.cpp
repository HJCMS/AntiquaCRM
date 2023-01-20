// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabel.h"

#include <QDebug>
#include <QFontMetricsF>
#include <QHBoxLayout>
#include <QPalette>
#include <QPixmap>
#include <QSizePolicy>

KeywordLabel::KeywordLabel(const QString &txt, QWidget *parent)
    : QFrame{parent} {
  setObjectName(txt);
  setContentsMargins(0, 0, 0, 0);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Plain);
  setStyleSheet(lableStyleSheet());
  setToolTip(txt);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  m_text = new QLabel(this);
  m_text->setText(txt);
  layout->addWidget(m_text);

  m_close = new QToolButton(this);
  m_close->setContentsMargins(0, 0, 0, 0);
  m_close->setIcon(QIcon(":icons/action_cancel.png"));
  m_close->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_close->setToolTip(tr("Remove this keyword"));
  layout->addWidget(m_close);

  setLayout(layout);
  connect(m_close, SIGNAL(clicked()), this, SLOT(removeClicked()));
}

const QString KeywordLabel::lableStyleSheet() const {
  QString color = palette().color(QPalette::Text).name();
  QStringList css;
  css << "padding-left:2px;padding-right:2px;";
  css << "padding-top:1px;padding-bottom:1px;";
  css << "border:1px solid " + color + ";border-radius:3px;";
  return "KeywordLabel {" + css.join("") + "}";
}

void KeywordLabel::removeClicked() { emit sendPleaseRemove(this); }

const QString KeywordLabel::text() { return m_text->text(); }
