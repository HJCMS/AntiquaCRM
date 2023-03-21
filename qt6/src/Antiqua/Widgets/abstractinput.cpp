// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abstractinput.h"
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
  setContentsMargins(0, 0, 0, 0);
  config = new AntiquaCRM::ASettings(parent);
  layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
  layout->setObjectName("antiqua_input_layout");
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
  d->init(this);
}

AbstractInput::~AbstractInput() {}

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

void AbstractInput::setWhatsThisButton(const QString &text) {
  if (text.isEmpty())
    return;

  WhatsThisButton *m_tbn = new WhatsThisButton(text, this);
  layout->addWidget(m_tbn);
}

void AbstractInput::setStretch() { layout->addStretch(1); }

} // namespace AntiquaCRM
