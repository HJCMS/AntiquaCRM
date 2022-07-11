// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PURCHASEDEBUGTABLE_H
#define ANTIQUA_PURCHASEDEBUGTABLE_H

#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

#include "antiqua_global.h"

namespace Antiqua {

class ANTIQUACORE_EXPORT PurchaseDebugTable final : public QTableWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit PurchaseDebugTable(QWidget *parent = nullptr);
  QTableWidgetItem *createItem(const QVariant &value) const;
};

};     // namespace Antiqua
#endif // ANTIQUA_PURCHASEDEBUGTABLE_H
