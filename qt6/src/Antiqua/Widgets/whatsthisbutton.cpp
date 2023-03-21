// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whatsthisbutton.h"

#include <QIcon>
#include <QWhatsThis>
#include <QDebug>
#include <QApplication>

namespace AntiquaCRM {

WhatsThisButton::WhatsThisButton(const QString &help, QWidget *parent)
    : QToolButton{parent}, p_text{help} {
  QIcon fallback("://icons/filetype.png");
  QIcon icon = QIcon::fromTheme("help-contents", fallback);

  setContentsMargins(0, 0, 0, 0);
  setIcon(icon);
  setToolTip(tr("Open tooltip for this input edit."));
  setStatusTip(toolTip());
  setToolButtonStyle(Qt::ToolButtonIconOnly);

  connect(this, SIGNAL(clicked()), SLOT(showWhatsThis()));
}

void WhatsThisButton::showWhatsThis() {
  if (!p_text.isEmpty())
    QWhatsThis::showText(pos(), p_text, this);
}

bool WhatsThisButton::event(QEvent *e) {
  if (e->type() == QEvent::HoverEnter) {
    setCursor(Qt::WhatsThisCursor);
  } else if (e->type() == QEvent::HoverLeave) {
    setCursor(Qt::ArrowCursor);
  }
  return QToolButton::event(e);
}

} // namespace AntiquaCRM
