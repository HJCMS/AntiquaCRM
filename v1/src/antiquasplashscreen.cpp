// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasplashscreen.h"

static const QPixmap splash() {
  QPixmap pixmap(QString(":icons/splash.png"));
  return pixmap;
}

AntiquaSplashScreen::AntiquaSplashScreen(QMainWindow *parent)
    : QSplashScreen{parent->screen(),splash()} {}

void AntiquaSplashScreen::setMessage(const QString &message) {
  showMessage(message, Qt::AlignCenter, Qt::black);
}
