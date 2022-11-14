// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLEHEADER_H
#define ANTIQUACRM_PURCHASETABLEHEADER_H

#include <QHeaderView>
#include <QObject>
#include <QSqlRecord>
#include <QList>
#include <QWidget>

class PurchaseTableHeader final : public QHeaderView {
  Q_OBJECT

public:
  explicit PurchaseTableHeader(QWidget *parent = nullptr);
  const QString columnTitle(const QString &fieldName);
  const QList<int> editableColumns();
};

#endif // PURCHASETABLEHEADER_H
