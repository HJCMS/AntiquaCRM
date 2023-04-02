// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "awhatsthisbutton.h"
#include "abstractinput.h"

#include <QIcon>
#include <QWhatsThis>

namespace AntiquaCRM {

AWhatsThisButton::AWhatsThisButton(const QString &help, QWidget *parent)
    : QPushButton{parent}, p_text{help} {
  setContentsMargins(0, 0, 0, 0);
  setIcon(AntiquaApplIcon("help-contents"));
  setToolTip(tr("Click to open Tooltip."));
  setStatusTip(toolTip());
  setStyleSheet("QPushButton {border:none;margin:0px;}");
  connect(this, SIGNAL(clicked()), SLOT(showWhatsThis()));
}

void AWhatsThisButton::showWhatsThis() {
  if (p_text.isEmpty())
    return;

  QWhatsThis::showText(pos(), p_text, this);
}

bool AWhatsThisButton::event(QEvent *e) {
  if (e->type() == QEvent::HoverEnter) {
    setCursor(Qt::WhatsThisCursor);
  } else if (e->type() == QEvent::HoverLeave) {
    setCursor(Qt::ArrowCursor);
  }
  return QPushButton::event(e);
}

} // namespace AntiquaCRM
