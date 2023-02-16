// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICE_H
#define DELIVERSERVICE_H

#include <AntiquaWidgets>
#include <QComboBox>
#include <QLabel>
#include <QObject>
#include <QPair>
#include <QWidget>

class DeliverPackageBox;
class DeliverServiceBox;

class DeliverService final : public InputEdit {
  Q_OBJECT

private:
  QString p_currency;
  AntiquaCRM::ASettings *m_cfg;
  DeliverServiceBox *m_serviceBox;
  DeliverPackageBox *m_packageBox;
  QLabel *m_priceInfo;

private Q_SLOTS:
  void getPriceOnDemand(bool);
  void currentServiceChanged(int);
  void packageChanged(int);

Q_SIGNALS:
  void sendDeliveryService(int did);
  void sendDeliveryPackage(int cid);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();
  void setDeliveryService(int did);
  void setDeliveryPackage(int cid);

public:
  explicit DeliverService(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  void loadDataset();
  const QVariant value();
  int getDeliveryPackage();
  bool isInternational();
  qreal getPackagePrice();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();

  /**
   * @brief Paketdienst aus Auftrag setzen!
   */
  void setDeliveryService(const QPair<int, int> &);

  /**
   * @brief Standard Paketdienst
   */
  const QPair<int, int> defaultDeliveryService();
};

#endif // DELIVERSERVICE_H
