// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportaction.h"
#include <AntiquaWidgets>

ReportAction::ReportAction(const QDate& date, const QString& title, QObject* parent)
    : QAction{AntiquaCRM::antiquaIcon("view-calendar-timeline"), title, parent}, p_date{date} {
  connect(this, SIGNAL(triggered()), SLOT(clicked()));
}

void ReportAction::clicked() {
  emit signalClicked(p_date);
}
