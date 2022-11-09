// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERBUYERINFO_H
#define ANTIQUACRM_PROVIDERBUYERINFO_H

#include <QJsonObject>
#include <QObject>
#include <QTextEdit>
#include <QWidget>

class ProviderBuyerInfo final : public QWidget {
  Q_OBJECT

private:
  QTextEdit *m_invoice;
  QTextEdit *m_delivery;

public:
  explicit ProviderBuyerInfo(QWidget *parent = nullptr);
  void setAddressData(const QJsonObject &obj);
};

#endif // ANTIQUACRM_PROVIDERBUYERINFO_H
