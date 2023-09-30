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
  // Publisher
  const QString _publisher(tr("Publisher"));
  layout->addWidget(infoCell(_publisher), 0, 0, 1, 1);
  layout->addWidget(infoCell(":"), 0, 1, 1, 1);
  m_publisher = new QLineEdit(this);
  m_publisher->setPlaceholderText(_publisher);
  layout->addWidget(m_publisher, 0, 2, 1, 1);
  // Location
  const QString _location(tr("Location"));
  layout->addWidget(infoCell(_location), 1, 0, 1, 1);
  layout->addWidget(infoCell(":"), 1, 1, 1, 1);
  m_location = new QLineEdit(this);
  m_location->setPlaceholderText(_location);
  layout->addWidget(m_location, 1, 2, 1, 1);
  // Final
  layout->setColumnStretch(2, 1);
  setLayout(layout);
}

QLabel *PublishersInput::infoCell(const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setIndent(1);
  lb->setMargin(0);
  lb->setTextFormat(Qt::PlainText);
  lb->setTextInteractionFlags(Qt::NoTextInteraction);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return lb;
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
  // _o.replace("&", " "); ??? __TODO__
  return AntiquaCRM::AUtil::trim(_o);
}

void PublishersInput::dataChanged() { setWindowModified(true); }

void PublishersInput::clear() {
  m_publisher->clear();
  m_location->clear();
  m_publisher->setFocus();
  setWindowModified(false);
}

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
