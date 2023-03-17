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

  AntiquaCRM::AComboBox *_box = new AntiquaCRM::AComboBox(w);
  _box->setWithoutDisclosures(0);
  for(int i = 1; i < 10; i++) {
    _box->insertItem(i, AntiquaCRM::AUtil::zerofill(i, 3), i);
  }
  layout->addWidget(_box);

  QSqlField _f; // sql test field
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  _f.setName("a_last_changed");
  _f.setDefaultValue(QDateTime::currentDateTime().toString());

  AntiquaCRM::ADateInfo *_date = new AntiquaCRM::ADateInfo(w);
  _date->setValue(QDateTime::currentDateTime());
  _date->setRestrictions(_f);
  layout->addWidget(_date);

  AntiquaCRM::ALabel *_label = new AntiquaCRM::ALabel(w);
  _label->setRestrictions(_f);
  _label->setValue(QDateTime::currentDateTime());
  layout->addWidget(_label);

  AntiquaCRM::ALineEdit *_line = new AntiquaCRM::ALineEdit(w);
  _line->setRestrictions(_f);
  layout->addWidget(_line);

  layout->addStretch(1);
  w->setLayout(layout);
  win->setCentralWidget(w);
  win->show();

  return appl.exec();
}
