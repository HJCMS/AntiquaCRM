// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PURCHASEPAYMENTINFO_H
#define ANTIQUA_PURCHASEPAYMENTINFO_H

#include <QComboBox>
#include <QMap>
#include <QObject>
#include <QLineEdit>
#include <QString>
#include <QVariant>
#include <QWidget>

#include "antiqua_global.h"
#include "paymenttypeset.h"

namespace Antiqua {

class PaymentMethodSelect : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  PaymentTypeSet p_typeSet;

protected:
  void wheelEvent(QWheelEvent *e) override{
      /* Disable wheel actions to prevent invalid inputs! */
  };

public:
  explicit PaymentMethodSelect(QWidget *parent = nullptr);
  int findIndex(int id);
  int findIndex(const QString &name);
  int getPaymentMethod(const QString &name);
  const QString getPaymentMethod(int id);
};

/**
 * @brief Zahlungsinformationen
 * @class PurchasePaymentInfo
 * @ingroup Antiqua Plugin Interface
 */
class ANTIQUACORE_EXPORT PurchasePaymentInfo : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  PaymentMethodSelect *o_payment_method;
  QLineEdit *m_paymentTransactionId;
  QLineEdit *m_deliveryCost;

public:
  explicit PurchasePaymentInfo(QWidget *parent = nullptr);
  void setData(const QString &objName, const QVariant &value);
  QMap<QString, QVariant> getAll();
};

}; // namespace Antiqua

#endif // ANTIQUA_PURCHASEPAYMENTINFO_H
