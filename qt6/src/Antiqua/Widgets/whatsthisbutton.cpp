// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whatsthisbutton.h"

#include <QIcon>
#include <QWhatsThis>

namespace AntiquaCRM {

WhatsThisButton::WhatsThisButton(const QString &help, QWidget *parent)
    : QPushButton{parent}, p_text{help} {
  setContentsMargins(0, 0, 0, 0);
  QIcon fallback("://icons/help-contents.png");
  setIcon(QIcon::fromTheme("help-contents", fallback));
  setToolTip(tr("Open tooltip for this input edit."));
  setStatusTip(toolTip());
  setStyleSheet("QPushButton {border:none;margin:0px;}");
  connect(this, SIGNAL(clicked()), SLOT(showWhatsThis()));
}

void WhatsThisButton::showWhatsThis() {
  if (p_text.isEmpty())
    return;

  QWhatsThis::showText(pos(), p_text, this);
}

bool WhatsThisButton::event(QEvent *e) {
  if (e->type() == QEvent::HoverEnter) {
    setCursor(Qt::WhatsThisCursor);
  } else if (e->type() == QEvent::HoverLeave) {
    setCursor(Qt::ArrowCursor);
  }
  return QPushButton::event(e);
}

} // namespace AntiquaCRM
