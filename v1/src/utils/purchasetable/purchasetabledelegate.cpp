// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetabledelegate.h"

#include <QComboBox>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>

PurchaseTableDelegate::PurchaseTableDelegate(const EditorProperties &config,
                                             QObject *parent)
    : QItemDelegate{parent}, p_config{config} {}

QWidget *PurchaseTableDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const {
  Q_UNUSED(option);
  if (!index.isValid()) {
    qDebug() << "createEditor invalid:" << index.row() << index.column();
    return parent;
  }

  QVariant value = index.model()->data(index, Qt::EditRole);
  if (index.column() == 4) { // Count
    QSpinBox *m_spinBox = new QSpinBox(parent);
    m_spinBox->setMinimum(p_config.minCount);
    m_spinBox->setMaximum(p_config.maxCount);
    m_spinBox->setValue(value.toInt());
    return m_spinBox;
  }

  if (index.column() == 6) { // Price
    QDoubleSpinBox *m_price = new QDoubleSpinBox(parent);
    m_price->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_price->setReadOnly(true);
    m_price->setSuffix(p_config.currency);
    m_price->setRange(p_config.minPrice, p_config.maxPrice);
    m_price->setValue(value.toDouble());
    return m_price;
  }

  if (index.column() == 7) { // Sell Price
    QDoubleSpinBox *m_sell_price = new QDoubleSpinBox(parent);
    m_sell_price->setSuffix(p_config.currency);
    m_sell_price->setRange(p_config.minPrice, p_config.maxPrice);
    m_sell_price->setValue(value.toDouble());
    return m_sell_price;
  }

  if (index.column() == 10) { // Media Type
    SelectArticleType *m_select = new SelectArticleType(parent);
    m_select->setValue(value.toInt());
    return m_select;
  }

  QLineEdit *m_lineEdit = new QLineEdit(parent);
  m_lineEdit->setFrame(false);
  m_lineEdit->setReadOnly(true);
  m_lineEdit->setText(value.toString());
  return m_lineEdit;
}

void PurchaseTableDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const {
  if (!index.isValid())
    return;

  QVariant value = index.model()->data(index, Qt::EditRole);
  if (value.isNull())
    return;

  if (index.column() == 4) { // Count
    QSpinBox *m_spinBox = qobject_cast<QSpinBox *>(editor);
    m_spinBox->setMinimum(1);
    m_spinBox->setValue(p_config.minCount);
    m_spinBox->setMaximum(p_config.maxCount);
    return;
  }

  if (index.column() == 6) { // Price
    QDoubleSpinBox *p = qobject_cast<QDoubleSpinBox *>(editor);
    p->setButtonSymbols(QAbstractSpinBox::NoButtons);
    p->setReadOnly(true);
    p->setSuffix(p_config.currency);
    p->setRange(p_config.minPrice, p_config.maxPrice);
    p->setValue(value.toDouble());
    return;
  }

  if (index.column() == 7) { // Sell Price
    QDoubleSpinBox *p = qobject_cast<QDoubleSpinBox *>(editor);
    p->setSuffix(p_config.currency);
    p->setRange(p_config.minPrice, p_config.maxPrice);
    p->setValue(value.toDouble());
    return;
  }

  if (index.column() == 10) { // Type
    SelectArticleType *m_select = qobject_cast<SelectArticleType *>(editor);
    m_select->setValue(value.toInt());
    return;
  }

  QLineEdit *m_lineEdit = qobject_cast<QLineEdit *>(editor);
  m_lineEdit->setFrame(false);
  m_lineEdit->setReadOnly(true);
  m_lineEdit->setText(value.toString());
}

void PurchaseTableDelegate::setModelData(QWidget *editor,
                                         QAbstractItemModel *model,
                                         const QModelIndex &index) const {
  if (!index.isValid())
    return;

  if (index.column() == 4) { // Count
    QSpinBox *sp = qobject_cast<QSpinBox *>(editor);
    model->setData(index, sp->value(), Qt::EditRole);
    return;
  }

  if (index.column() == 6) { // Price
    QDoubleSpinBox *dsp = qobject_cast<QDoubleSpinBox *>(editor);
    model->setData(index, dsp->value(), Qt::EditRole);
    return;
  }

  if (index.column() == 7) { // Sell Price
    QDoubleSpinBox *dsp = qobject_cast<QDoubleSpinBox *>(editor);
    model->setData(index, dsp->value(), Qt::EditRole);
    return;
  }

  if (index.column() == 10) { // Type
    SelectArticleType *m_select = qobject_cast<SelectArticleType *>(editor);
    model->setData(index, m_select->value().toInt(), Qt::EditRole);
    return;
  }
}
