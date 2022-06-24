// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WHSOFTPURCHASER_PLUGIN_H
#define WHSOFTPURCHASER_PLUGIN_H

#include <QObject>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

/**
 * @brief The WHSoftPurchaser class
 * @ingroup Whsoft Plugin
 */
class ANTIQUACORE_EXPORT WHSoftPurchaser final
    : public Antiqua::PurchaserWidget {
  Q_OBJECT

public Q_SLOTS:
  void setCustomerId(int customerId);
  void setValue(const QString &objName, const QVariant &value);

public:
  QTextEdit *rechnungsadresse;
  QTextEdit *lieferadresse;
  explicit WHSoftPurchaser(QWidget *parent = nullptr);
  const QVariant getValue(const QString &objName);
};

#endif // WHSOFTPURCHASER_PLUGIN_H
