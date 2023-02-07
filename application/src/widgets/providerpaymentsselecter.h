// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDER_PAYMENTS_SELECTER_H
#define ANTIQUACRM_PROVIDER_PAYMENTS_SELECTER_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QMap>
#include <QPair>

class ProviderPaymentsSelecter final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void sendPaymentStatusChanged(AntiquaCRM::ProviderPaymentStatus);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit ProviderPaymentsSelecter(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;

  static const QMap<AntiquaCRM::ProviderPaymentStatus, QString> getStatus();
};

#endif // ANTIQUACRM_PROVIDER_PAYMENTS_SELECTER_H
