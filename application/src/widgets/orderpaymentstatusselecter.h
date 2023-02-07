// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERPAYMENT_STATUSSELECTER_H
#define ANTIQUACRM_ORDERPAYMENT_STATUSSELECTER_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QAbstractItemModel>
#include <QIcon>
#include <QObject>

class OrderPaymentStatusModel final : public QAbstractItemModel {
  Q_OBJECT

private:
  struct StatusItem {
    AntiquaCRM::OrderPayment status;
    QString title;
    QString toolTip;
    QIcon decoration;
  };
  QList<StatusItem> statusList;
  StatusItem createItem(AntiquaCRM::OrderPayment);
  QVariant getRoleData(const StatusItem &item,
                       int role = Qt::DisplayRole) const;

public:
  explicit OrderPaymentStatusModel(QWidget *parent = nullptr);

  QModelIndex parent(const QModelIndex &index) const override;

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;
};

class OrderPaymentStatusSelecter final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit OrderPaymentStatusSelecter(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_ORDERPAYMENT_STATUSSELECTER_H
