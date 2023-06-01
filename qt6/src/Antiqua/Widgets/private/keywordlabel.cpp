// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlabel.h"
#include "ainputwidget.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QStyleOptionFrame>
#include <QStylePainter>
#include <QToolButton>

namespace AntiquaCRM {

KeywordLabel::KeywordLabel(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);
  setObjectName("keywordlabel");
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
  setFrameShape(QFrame::StyledPanel);
  setFrameShadow(QFrame::Plain);
  setAcceptDrops(false);

  QString color = palette().color(QPalette::Text).name();
  QStringList css;
  css << "padding-left:2px;padding-right:2px;";
  css << "padding-top:1px;padding-bottom:1px;";
  css << "border:1px solid "+ color + ";border-radius:3px;";
  setStyleSheet("QFrame#keywordlabel {" + css.join("") + "}");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setObjectName("keyword::layout");
  layout->setContentsMargins(1, 1, 1, 1);

  m_label = new QLabel(this);
  m_label->setObjectName("keyword::lable");
  m_label->setTextInteractionFlags(Qt::NoTextInteraction);
  m_label->setIndent(1);
  layout->addWidget(m_label);

  QToolButton *m_close = new QToolButton(this);
  m_close->setObjectName("keyword::button");
  m_close->setIcon(AntiquaCRM::AntiquaApplIcon("edit-delete"));
  m_close->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_close->setToolTip(tr("Remove this keyword"));
  layout->addWidget(m_close);

  setLayout(layout);

  connect(m_close, SIGNAL(clicked()), this, SIGNAL(aboutToRemove()));
}

KeywordLabel::KeywordLabel(const QString &keyword, QWidget *parent)
    : KeywordLabel{parent} {
  setText(keyword);
}

void KeywordLabel::setText(const QString &keyword) {
  if (keyword.isEmpty())
    return;

  m_label->setText(keyword);
}

const QString KeywordLabel::text() { return m_label->text(); }

} // namespace AntiquaCRM
