// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "alabel.h"

namespace AntiquaCRM {

ALabel::ALabel(QWidget *parent) : QLabel{parent} {
  setIndent(2);
  setTextFormat(Qt::PlainText);
  setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                          Qt::TextSelectableByMouse);
}

ALabel::ALabel(const QString &text, QWidget *parent) : ALabel{parent} {
  setText(text);
}

} // namespace AntiquaCRM
