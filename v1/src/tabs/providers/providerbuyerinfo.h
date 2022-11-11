// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERBUYERINFO_H
#define ANTIQUACRM_PROVIDERBUYERINFO_H

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

class ProviderBuyerInfo final : public QWidget {
  Q_OBJECT

private:
  TextField *m_invoice;
  TextField *m_delivery;

public:
  explicit ProviderBuyerInfo(QWidget *parent = nullptr);
  void setAddressData(const QJsonObject &obj);
};

#endif // ANTIQUACRM_PROVIDERBUYERINFO_H
