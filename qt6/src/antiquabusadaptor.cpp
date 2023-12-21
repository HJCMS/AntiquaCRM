// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquabusadaptor.h"

AntiquaBusAdaptor::AntiquaBusAdaptor(QObject *parent)
    : QDBusAbstractAdaptor{parent}, p_message{QString()} {}

void AntiquaBusAdaptor::toggle() { emit sendToggleView(); }

void AntiquaBusAdaptor::quit() { emit sendAboutQuit(); }

const QString AntiquaBusAdaptor::message() const { return p_message; }

void AntiquaBusAdaptor::pushMessage(const QString &msg) {
  p_message = msg.trimmed();
  emit received();
  emit sendMessage(p_message);
}
