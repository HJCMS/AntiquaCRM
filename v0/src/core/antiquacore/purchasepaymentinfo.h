// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PURCHASEPAYMENTINFO_H
#define ANTIQUA_PURCHASEPAYMENTINFO_H

#include <QComboBox>
#include <QLineEdit>
#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>

#include "antiqua_global.h"
#include "paymenttypeset.h"

namespace Antiqua {

/**
 * @brief Zahlungs-Methode anzeigen
 * @class PaymentMethodSelect
 * @ingroup Antiqua Plugin Interface
 */
class PaymentMethodSelect final : public QComboBox {
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
 * @brief Zahlungsdaten Anzeige
 * @class PLineRead
 * @ingroup Antiqua Plugin Interface
 */
class PLineRead final : public QLineEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit PLineRead(QWidget *parent = nullptr);
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
  PLineRead *m_paymentTransactionId;
  PLineRead *m_paymentConfirmed;
  PLineRead *m_deliveryCost;
  PLineRead *c_iban;
  PLineRead *c_swift_bic;
  PLineRead *c_tax_id;

public:
  explicit PurchasePaymentInfo(QWidget *parent = nullptr);
  void setData(const QString &objName, const QVariant &value);
  QMap<QString, QVariant> getAll();
};

}; // namespace Antiqua

#endif // ANTIQUA_PURCHASEPAYMENTINFO_H
