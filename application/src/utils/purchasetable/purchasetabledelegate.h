// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLEDELEGATE_H
#define ANTIQUACRM_PURCHASETABLEDELEGATE_H

#include <QAbstractItemModel>
#include <QByteArray>
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QStyleOptionViewItem>
#include <QWidget>

/**
 * @brief The PurchaseTableDelegate class
 * @ingroup _purchasetable
 */
class PurchaseTableDelegate final : public QItemDelegate {
  Q_OBJECT

private:
  /**
   * @brief Einstellungen für die eingaben.
   */
  struct EditorProperties {
    qreal minPrice;
    qreal maxPrice;
    int minCount;
    int maxCount;
    int maxInteger;
    QByteArray currency;
  };
  EditorProperties config;

public:
  explicit PurchaseTableDelegate(QObject *parent = nullptr);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;

  void setEditorData(QWidget *editor, const QModelIndex &index) const override;

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
};

#endif // ANTIQUACRM_PURCHASETABLEDELEGATE_H
