// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PAYMENTTYPESET_ANTIQUACORE_H
#define PAYMENTTYPESET_ANTIQUACORE_H

#include <QtGlobal>
#include <QMap>

class PaymentTypeSet final : public QMap<int, QString>
{
public:
  explicit PaymentTypeSet();
  int findKey(const QString &name);
};

#endif // PAYMENTTYPESET_ANTIQUACORE_H
