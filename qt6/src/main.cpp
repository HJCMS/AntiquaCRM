// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

int main(int argc, char *argv[]) {
  QApplication appl(argc, argv);
  appl.setApplicationName(ANTIQUACRM_NAME);
  appl.setApplicationVersion(ANTIQUACRM_VERSION);
  appl.setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  appl.setStyle(QStyleFactory::create("Fusion"));
  QStringList customCSS;
  customCSS << "QTabBar::tab:selected {color: palette(highlight);}";
  customCSS << "QPushButton:hover {color:palette(highlight);}";
  customCSS << "QRadioButton:checked {color:palette(highlight);}";
  customCSS << "QGroupBox::title{padding-right:10px;}";
  appl.setStyleSheet(customCSS.join("\n"));

  QMainWindow *win = new QMainWindow;
  win->setWindowTitle("Tests");
  win->setMinimumSize(600, 400);

  QWidget *w = new QWidget(win);
  QVBoxLayout *layout = new QVBoxLayout(w);
  layout->setContentsMargins(2, 0, 2, 0);

  QSqlField _f; // sql test field
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  _f.setName("a_last_changed");
  _f.setDefaultValue(QDateTime::currentDateTime().toString());

//  AntiquaCRM::ADateInfo *_date = new AntiquaCRM::ADateInfo(w);
//  _date->setObjectName("inp_date_info");
//  layout->addWidget(_date);

  layout->addStretch(1);
  w->setLayout(layout);
  win->setCentralWidget(w);
  win->show();

//QRegularExpression pattern("^inp_[a-z_]{2,}");
//QList<AntiquaCRM::AbstractInput *> list =
//    w->findChildren<AntiquaCRM::AbstractInput *>(pattern);
//for (int i = 0; i < list.size(); i++) {
//  AntiquaCRM::AbstractInput *obj = list.at(i);
//  if (obj != nullptr) {
//    obj->setValue(QDateTime::currentDateTime());
//  }
//}
  return appl.exec();
}
