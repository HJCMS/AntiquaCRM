// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDER_PAYMENTS_SELECTER_H
#define ANTIQUACRM_PROVIDER_PAYMENTS_SELECTER_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QWidget>

class ProviderPaymentsSelecter final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void sendPaymentStatusChanged(AntiquaCRM::ProviderPaymentStatus);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit ProviderPaymentsSelecter(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
  static const QMap<AntiquaCRM::ProviderPaymentStatus, QString> getStatus();
};

#endif // ANTIQUACRM_PROVIDER_PAYMENTS_SELECTER_H
