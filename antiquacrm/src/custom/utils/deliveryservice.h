// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERYSERVICE_UTILS_H
#define DELIVERYSERVICE_UTILS_H

#include <QComboBox>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include <UtilsMain>

class DeliveryService final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int p_value = 0;
  QLabel *m_info;
  QComboBox *m_box;
  const QString sqlTable = QString("ref_delivery_service");

private Q_SLOTS:
  void indexChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit DeliveryService(QWidget *parent = nullptr);
  void loadSqlDataset();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // DELIVERYSERVICE_UTILS_H
