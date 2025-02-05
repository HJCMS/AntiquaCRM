// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publisheredit.h"

#include <QLabel>
#include <QLayout>

PublisherEdit::PublisherEdit(QWidget* parent) : QWidget{parent} {
  setContentsMargins(2, 2, 2, 2);
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(contentsMargins());

  QHBoxLayout* xlayout = new QHBoxLayout();

  QLabel* m_lb = new QLabel(tr("Publisher"), this);
  m_lb->setWordWrap(false);
  m_lb->setTextInteractionFlags(Qt::NoTextInteraction);
  m_lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  xlayout->addWidget(m_lb);

  m_publisher = new QLineEdit(this);
  xlayout->addWidget(m_publisher);
  layout->addLayout(xlayout);

  m_location = new AntiquaCRM::TextLine(this);
  m_location->setBuddyLabel(tr("Location"));
  m_location->appendStretch(0);
  layout->addWidget(m_location);

  setLayout(layout);

  connect(m_publisher, SIGNAL(textChanged(QString)), SLOT(searchOnEdit(QString)));
  connect(m_publisher, SIGNAL(editingFinished()), SIGNAL(sendDataChanged()));
  connect(m_location, SIGNAL(sendInputChanged()), SIGNAL(sendDataChanged()));
}

void PublisherEdit::searchOnEdit(const QString& str) {
  QString _find = str.trimmed();
  if (_find.length() > 2)
    emit sendFindPublisher(_find);
}

void PublisherEdit::setData(const QString& publisher, const QString& location) {
  blockSignals(true);
  m_publisher->setText(publisher);
  m_location->setValue(location);
  blockSignals(false);
}

const QString PublisherEdit::getPublisher() {
  QString _txt = m_publisher->text().trimmed();
  _txt.replace("`", "’");
  _txt.replace("´", "’");
  return _txt.replace("'", "’");
}

const QString PublisherEdit::getLocation() {
  return m_location->getValue().toString().trimmed();
}
