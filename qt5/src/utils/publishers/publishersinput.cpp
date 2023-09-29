// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishersinput.h"

#include <AntiquaCRM>
#include <QLabel>
#include <QLayout>
#include <QRegExp>

PublishersInput::PublishersInput(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  // row1
  QLabel *lb1 = new QLabel(tr("Publisher"), this);
  layout->addWidget(lb1, 0, 0, 1, 1);

  m_publisher = new QLineEdit(this);
  m_publisher->setPlaceholderText(lb1->text());
  layout->addWidget(m_publisher, 0, 1, 1, 1);

  // row2
  QLabel *lb2 = new QLabel(tr("Location"), this);
  layout->addWidget(lb2, 1, 0, 1, 1);

  m_location = new QLineEdit(this);
  m_location->setPlaceholderText(lb2->text());
  layout->addWidget(m_location, 1, 1, 1, 1);

  setLayout(layout);
}

const QString PublishersInput::strip(const QString &str) const {
  QString _o = str.trimmed();
  _o.replace(".", "");
  _o.replace(",", "");
  _o.replace(":", " ");
  _o.replace("-", " ");
  _o.replace("'", "’");
  _o.replace("´", "’");
  _o.replace("`", "’");
  _o.replace("\"", "’");
  // _o.replace("&", " ");
  return AntiquaCRM::AUtil::trim(_o);
}

void PublishersInput::dataChanged() {}

void PublishersInput::setPublisher(const QString &name) {
  m_publisher->setText(name);
}

void PublishersInput::setLocation(const QString &location) {
  m_location->setText(location);
}

const QString PublishersInput::getPublisher() {
  return strip(m_publisher->text());
}

const QString PublishersInput::getLocation() {
  return strip(m_location->text());
}
