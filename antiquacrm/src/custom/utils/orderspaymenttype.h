// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSPAYMENTTYPE_H
#define ORDERSPAYMENTTYPE_H

#include <QComboBox>
#include <QMap>
#include <QObject>
#include <QWidget>

#include <UtilsMain>

class OrdersPaymentTypes : public QMap<int, QString> {
public:
  explicit OrdersPaymentTypes();
};

class OrdersPaymentTypeSelect final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  void wheelEvent(QWheelEvent *e) override{
      /* Disable wheel actions to prevent invalid inputs! */
  };

public:
  OrdersPaymentTypeSelect(QWidget *parent = nullptr);
};

class OrdersPaymentType : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  OrdersPaymentTypeSelect *m_box;

private Q_SLOTS:
  void typeChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &val);

public:
  OrdersPaymentType(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ORDERSPAYMENTTYPE_H
