// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publisheredit.h"

#include <QLayout>

PublisherEdit::PublisherEdit(QWidget *parent) : QWidget{parent} {
  setContentsMargins(2, 2, 2, 2);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(contentsMargins());

  m_publisher = new AntiquaCRM::TextLine(this);
  m_publisher->setBuddyLabel(tr("Publisher"));
  m_publisher->appendStretch(0);
  layout->addWidget(m_publisher);

  m_location = new AntiquaCRM::TextLine(this);
  m_location->setBuddyLabel(tr("Location"));
  m_location->appendStretch(0);
  layout->addWidget(m_location);

  setLayout(layout);

  connect(m_publisher, SIGNAL(sendInputChanged()), SIGNAL(sendDataChanged()));
  connect(m_location, SIGNAL(sendInputChanged()), SIGNAL(sendDataChanged()));
}

void PublisherEdit::setData(const QString &publisher, const QString &location) {
  blockSignals(true);
  m_publisher->setValue(publisher);
  m_location->setValue(location);
  blockSignals(false);
}

const QString PublisherEdit::getPublisher() {
  return m_publisher->getValue().toString();
}

const QString PublisherEdit::getLocation() {
  return m_location->getValue().toString();
}
