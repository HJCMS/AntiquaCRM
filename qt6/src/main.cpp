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
  win->setWindowTitle("Tests [*]");
  win->setMinimumSize(600, 400);

  QWidget *w = new QWidget(win);
  QVBoxLayout *layout = new QVBoxLayout(w);
  layout->setContentsMargins(2, 0, 2, 0);

  // QSqlField _fp; // sql test field
  //_fp.setMetaType(QMetaType(QMetaType::Double));
  //_fp.setRequiredStatus(QSqlField::Required);
  //_fp.setLength(80);
  //_fp.setName("a_sell_price");
  //_fp.setDefaultValue(7.00);

  // Price
  AntiquaCRM::PriceEdit *_price = new AntiquaCRM::PriceEdit(w);
  _price->setObjectName("a_sell_price");
  _price->setBuddyLabel("Verkaufspreis");
  _price->setWhatsThisButton("Hier den aktuellen Verkaufspreis einfügen.");
  _price->setStretch();
  _price->setValue(14.56);
  layout->addWidget(_price);

  //  Anzahl
  AntiquaCRM::CrowdEdit *_crowd = new AntiquaCRM::CrowdEdit(w);
  _crowd->setObjectName("a_count");
  _crowd->setBuddyLabel("Anzahl");
  _crowd->setWhatsThisButton("Hier bitte die Artikel bestands menge angeben.");
  _crowd->setStretch();
  _crowd->setValue(1);
  layout->addWidget(_crowd);

  layout->addStretch(1);
  QStatusBar *m_statusbar = new QStatusBar(w);
  layout->addWidget(m_statusbar);

  w->setLayout(layout);
  win->setCentralWidget(w);
  win->show();

  m_statusbar->showMessage("initialed");

  QRegularExpression pattern("^[a-z]{1}_[a-z_]{2,}");
  QList<AntiquaCRM::AbstractInput *> list =
      w->findChildren<AntiquaCRM::AbstractInput *>(pattern);
  for (int i = 0; i < list.size(); i++) {
    AntiquaCRM::AbstractInput *obj = list.at(i);
    if (obj != nullptr) {
      qDebug() << obj->objectName() << obj->getValue();
    }
  }
  return appl.exec();
}
