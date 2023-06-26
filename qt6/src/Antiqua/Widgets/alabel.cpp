// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alabel.h"

namespace AntiquaCRM {

ALabel::ALabel(QWidget *parent) : QLabel{parent} {
  setObjectName("ALabel");
  setIndent(2);
  setTextFormat(Qt::PlainText);
  setTextInteractionFlags(Qt::NoTextInteraction);
}

ALabel::ALabel(const QString &text, QWidget *parent) : ALabel{parent} {
  setText(text);
}

ALabel::ALabel(const QString &text, ALabel::Align align, QWidget *parent)
    : ALabel{parent} {
  setAlignText(align, text);
}

void ALabel::setAlignText(ALabel::Align align, const QString &text) {
  switch (align) {
  case (ALabel::Align::TopLeft):
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    break;

  case (ALabel::Align::MiddleLeft):
    setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    break;

  case (ALabel::Align::TopRight):
    setAlignment(Qt::AlignRight | Qt::AlignTop);
    break;

  case (ALabel::Align::MiddleRight):
    setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    break;

  case (ALabel::Align::MiddleCenter):
    setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    break;

  default:
    break;
  };

  setText(text);
}

} // namespace AntiquaCRM
