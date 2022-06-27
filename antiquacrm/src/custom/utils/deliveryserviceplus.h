// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERYSERVICEPLUS_UTILS_H
#define DELIVERYSERVICEPLUS_UTILS_H

#include <QComboBox>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include <SqlCore>
#include <UtilsMain>

class DeliveryServicePlus final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  int p_service = 0;
  int p_package = 0;
  QComboBox *m_serviceBox;
  QComboBox *m_packageBox;
  QLabel *m_price;
  void loadSqlPackage(int id);

private Q_SLOTS:
  void changedService(int);
  void changedPackage(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit DeliveryServicePlus(QWidget *parent = nullptr);
  void loadSqlDataset();
  int packageId();
  int serviceId();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // DELIVERYSERVICEPLUS_UTILS_H
