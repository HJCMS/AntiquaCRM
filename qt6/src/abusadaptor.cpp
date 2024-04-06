// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abusadaptor.h"

ABusAdaptor::ABusAdaptor(QObject* parent)
    : QDBusAbstractAdaptor{parent}
    , p_message{QString()} {
}

void ABusAdaptor::toggle() {
  emit sendToggleView();
}

void ABusAdaptor::quit() {
  emit sendAboutQuit();
}

const QString ABusAdaptor::message() const {
  return p_message;
}

void ABusAdaptor::pushMessage(const QString& msg) {
  p_message = msg.trimmed();
  emit received();
  emit sendMessage(p_message);
}
