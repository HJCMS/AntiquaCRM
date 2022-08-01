// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PAYMENTORDERUPDATE_H
#define ANTIQUA_PAYMENTORDERUPDATE_H

#include <QComboBox>
#include <QFrame>
#include <QMap>
#include <QObject>
#include <QWheelEvent>

#include "antiqua_global.h"

namespace Antiqua {

class ANTIQUACORE_EXPORT PaymentOrderUpdate final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  void wheelEvent(QWheelEvent *e) override{
      /* Disable wheel actions to prevent invalid inputs! */
  };

public:
  explicit PaymentOrderUpdate(QWidget *parent = nullptr);
  void setParams(const QMap<Antiqua::PaymentStatus, QString> &);
};
};     // namespace Antiqua
#endif // ANTIQUA_PAYMENTORDERUPDATE_H
