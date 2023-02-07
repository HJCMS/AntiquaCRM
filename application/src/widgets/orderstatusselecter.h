// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSTATUSSELECTER_H
#define ANTIQUACRM_ORDERSTATUSSELECTER_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QObject>
#include <QWidget>

class OrderStatusSelecter final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void sendOrderStatusChanged(AntiquaCRM::OrderStatus);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit OrderStatusSelecter(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_ORDERSTATUSSELECTER_H
