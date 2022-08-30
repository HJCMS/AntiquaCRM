// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abecatalog.h"
#include "homebasebuilder.h"
#include "homebasereader.h"

#include <QtCore>
#include <QtWidgets>

AbeCatalog::AbeCatalog(QWidget *parent) : QDialog{parent} {
  setWindowTitle("AbeBooks [*]");
  setMinimumSize(QSize(500, 450));
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_stackedWidget = new QStackedWidget(this);
  layout->addWidget(m_stackedWidget);

  m_btnBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_btnBox);

  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void AbeCatalog::testBuilder() {
  HomeBaseBuilder xmlBuilder;
  QString dest(QDir::homePath() + "/Downloads");
  if (xmlBuilder.createDocument())
    xmlBuilder.writeToFile(dest);
}

void AbeCatalog::testReader() {
  QString xmlFile(QDir::homePath());
  xmlFile.append(
      "/Developement/antiqua/database/tmp/AbeBooks/abebooks-report.xml");
  HomeBaseReader xml;
  if (xml.open(xmlFile)) {
    QFile fp(QDir::homePath() + "/Downloads/abebooks_homebase3.xml");
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream out(&fp);
      out.setCodec(HOMEBASE3_DEFAULT_CODEC);
      out << xml.toString(1);
      fp.close();
      qDebug() << "written to:" << fp.fileName();
    }
  }
}

int AbeCatalog::exec() {
  testBuilder();
  return QDialog::exec();
}
