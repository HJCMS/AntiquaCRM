// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdvstatusbar.h"

#include <QDebug>

CDVStatusBar::CDVStatusBar(QWidget *parent) : TabStatusBar{parent} {}

void CDVStatusBar::setHistoryMenu() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CDVStatusBar::setCreateButtonEnabled(bool b) {
  qDebug() << Q_FUNC_INFO << "TODO" << b;
}

void CDVStatusBar::setHistoryAction(int hist) {
  qDebug() << Q_FUNC_INFO << "TODO" << hist;
}
