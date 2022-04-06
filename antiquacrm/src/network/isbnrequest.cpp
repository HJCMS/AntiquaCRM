// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnrequest.h"
#include "network.h"

#include <QtCore/QDebug>

IsbnRequest::IsbnRequest(QObject *parent)
    : QObject{parent} {
  setObjectName("IsbnRequest");

  connect(this, SIGNAL(finished()), this, SLOT(urlReplyFinished()));
}

void IsbnRequest::setISBN(const QString &q) {
}
