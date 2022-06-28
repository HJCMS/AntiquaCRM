// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICE_H
#define DELIVERSERVICE_H

#include <QComboBox>
#include <QLabel>
#include <QObject>
#include <QWidget>
#include <UtilsMain>

class DeliverPackageBox;
class DeliverServiceBox;

class DeliverService final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int p_packageid;
  QString p_currency;
  DeliverServiceBox *m_serviceBox;
  DeliverPackageBox *m_packageBox;
  QLabel *m_priceInfo;

private Q_SLOTS:
  void packageChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit DeliverService(QWidget *parent = nullptr);
  void loadSqlDataset();
  const QVariant value();
  void setServicePackage(int);
  int getServicePackage();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // DELIVERSERVICE_H
