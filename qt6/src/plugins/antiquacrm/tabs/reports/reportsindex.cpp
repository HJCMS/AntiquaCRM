// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportsindex.h"

ReportsIndex::ReportsIndex(QWidget *parent)
    : AntiquaCRM::TabsIndex{"reports_tab", parent},
      p_date{QDate::currentDate()} {
  setObjectName("reports_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("x-office-spreadsheet"));
  setWindowTitle(getTitle());
  setClosable(true);

  QWidget *_dummy = new QWidget(this);
  insertWidget(0, _dummy);
  setCurrentIndex(ViewPage::MainView);
}

void ReportsIndex::setDefaultTableView() {}

void ReportsIndex::onEnterChanged() {}

const QString ReportsIndex::getTitle() const { return tr("Reports"); }

bool ReportsIndex::customAction(const QJsonObject &) { return false; }

const QStringList ReportsIndex::acceptsCustomActions() const {
  return QStringList();
}
