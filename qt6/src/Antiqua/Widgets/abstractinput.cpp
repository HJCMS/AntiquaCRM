// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abstractinput.h"
#include "alabel.h"
#include "private/abstractinput_p.h"
#include "whatsthisbutton.h"

namespace AntiquaCRM {

AbstractInputPrivate::AbstractInputPrivate() {
  // TODO
}

void AbstractInputPrivate::init(QWidget *parent) {
  // TODO
  Q_CHECK_PTR(parent);
}

AbstractInput::AbstractInput(QWidget *parent) : QWidget{parent} {
  Q_D(AbstractInput);
  config = new AntiquaCRM::ASettings(this);
  displayToolTips = windowBehavior("display_tooltip_buttons", true);
  mouseWheelEvents = windowBehavior("mouse_wheel_support", false);

  setContentsMargins(0, 0, 0, 0);
  layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
  layout->setObjectName("antiqua_input_layout");
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
  d->init(this);
}

AbstractInput::~AbstractInput() {}

const QIcon AbstractInput::getIcon(const QString &name) {
  QIcon fallback("://icons/" + name + ".png");
  return QIcon::fromTheme(name, fallback);
}

bool AbstractInput::windowBehavior(const QString &key, bool standard) {
  if (key.isEmpty())
    return false;

  bool b = false;
  config->beginGroup("window_behavior");
  b = config->value(key, standard).toBool();
  config->endGroup();
  return b;
}

void AbstractInput::setRequired(bool b) {
  Q_D(AbstractInput);
  d->required_status = b;
}

void AbstractInput::focusOutEvent(QFocusEvent *e) {
  if (e->type() == QEvent::FocusOut)
    emit sendLeaveInput();

  QWidget::focusOutEvent(e);
}

bool AbstractInput::isRequired() {
  Q_D(AbstractInput);
  return d->required_status;
}

AntiquaCRM::ALabel *AbstractInput::addTitleLabel(const QString &title) {
  ALabel *m_lb = new ALabel(title, this);
  m_lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m_lb->setTextInteractionFlags(Qt::NoTextInteraction);
  layout->insertWidget(0, m_lb);
  return m_lb;
}

void AbstractInput::setWhatsThisText(const QString &text) {
  if (text.isEmpty())
    return;

  if (displayToolTips) {
    WhatsThisButton *m_tbn = new WhatsThisButton(text, this);
    layout->addWidget(m_tbn);
  }
}

void AbstractInput::appendStretch(int expanding) {
  layout->addStretch(expanding);
}

} // namespace AntiquaCRM
