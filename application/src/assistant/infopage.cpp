// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "infopage.h"

#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>

InfoPage::InfoPage(QWidget *parent) : QWidget{parent} {

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 4, 4, 4);

  m_text = new QLabel(this);
  m_text->setWordWrap(true);
  layout->addWidget(m_text);

  layout->addStretch(1);
  setLayout(layout);
}

void InfoPage::setContent(const QString &key) {
  QFile fp(":/documents/" + key + "_de.txt");
  if (fp.open(QFile::ReadOnly)) {
    QString buffer;
    QTextStream in(&fp);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
      buffer.append(in.readLine());
    }
    m_text->setText(buffer);
  }
  fp.close();
}
