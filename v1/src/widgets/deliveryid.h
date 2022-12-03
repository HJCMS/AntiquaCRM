// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_DELIVERYID_H
#define ANTIQUACRM_DELIVERYID_H

#include <AntiquaInputEdit>
#include <QObject>
#include <QWidget>

class DeliveryId final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  void loadDataset();

private Q_SLOTS:
  void inputChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit DeliveryId(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  void setInfo(const QString &);
  const QString info();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // ANTIQUACRM_DELIVERYID_H
