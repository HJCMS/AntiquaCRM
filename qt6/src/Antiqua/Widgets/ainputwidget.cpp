// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ainputwidget.h"
#include "alabel.h"
#include "awhatsthisbutton.h"

namespace AntiquaCRM {

// BEGIN::AInputWidget
AInputWidget::AInputWidget(QWidget *parent) : QWidget{parent}, required{false} {
  config = new AntiquaCRM::ASettings(this);
  displayToolTips =
      config->groupValue("window_behavior", "display_tooltip_buttons", true)
          .toBool();
  mouseWheelEvents =
      config->groupValue("window_behavior", "mouse_wheel_support", false)
          .toBool();

  setContentsMargins(0, 0, 0, 0);
  layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
  layout->setObjectName("antiqua_input_layout");
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}

AInputWidget::~AInputWidget() {}

void AInputWidget::focusOutEvent(QFocusEvent *e) {
  if (e->type() == QEvent::FocusOut)
    emit sendLeaveInput();

  QWidget::focusOutEvent(e);
}

void AInputWidget::setRequired(bool b) { required = b; }

AntiquaCRM::ALabel *AInputWidget::addTitleLabel(const QString &title) {
  ALabel *m_lb = new ALabel(title, this);
  m_lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m_lb->setTextInteractionFlags(Qt::NoTextInteraction);
  layout->insertWidget(0, m_lb);
  return m_lb;
}

bool AInputWidget::isRequired() { return required; }

void AInputWidget::setWhatsThisText(const QString &text) {
  if (text.isEmpty())
    return;

  if (displayToolTips) {
    AWhatsThisButton *m_tbn = new AWhatsThisButton(text, this);
    m_tbn->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(m_tbn);
  }
}

void AInputWidget::appendStretch(int expanding) {
  layout->addStretch(expanding);
}
// END::AInputWidget

} // namespace AntiquaCRM
