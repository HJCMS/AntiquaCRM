// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLEDELEGATE_H
#define ANTIQUACRM_PURCHASETABLEDELEGATE_H

#include <AntiquaWidgets>
#include <QAbstractItemModel>
#include <QByteArray>
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QStyleOptionViewItem>
#include <QWidget>

struct EditorProperties {
  qreal minPrice = 3.00;
  qreal maxPrice = 999999.00;
  int minCount = 1;
  int maxCount = 10;
  QByteArray currency = QString("€").toLocal8Bit();
};

class PurchaseTableDelegate final : public QItemDelegate {
  Q_OBJECT

private:
  EditorProperties p_config;

public:
  explicit PurchaseTableDelegate(const EditorProperties &config,
                                 QObject *parent = nullptr);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
};

#endif // ANTIQUACRM_PURCHASETABLEDELEGATE_H
