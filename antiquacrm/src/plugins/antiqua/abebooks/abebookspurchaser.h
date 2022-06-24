// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_PLUGIN_PURCHASER_H
#define ABEBOOKS_PLUGIN_PURCHASER_H

#include <QObject>
#include <QVariant>
#include <QTextEdit>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class ANTIQUACORE_EXPORT AbeBooksPurchaser final
    : public Antiqua::PurchaserWidget {
  Q_OBJECT

public Q_SLOTS:
  void setCustomerId(int customerId);
  void setValue(const QString &objName, const QVariant &value);

public:
  QTextEdit *rechnungsadresse;
  QTextEdit *lieferadresse;
  explicit AbeBooksPurchaser(QWidget *parent = nullptr);
  const QVariant getValue(const QString &objName);
};

#endif // ABEBOOKS_PLUGIN_PURCHASER_H