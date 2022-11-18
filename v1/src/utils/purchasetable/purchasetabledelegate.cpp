// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetabledelegate.h"
#include "purchasetablecolumn.h"
#include "purchasetablemodel.h"

#include <AntiquaCRM>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

PurchaseTableDelegate::PurchaseTableDelegate(QObject *parent)
    : QItemDelegate{parent} {
  AntiquaCRM::ASettings cfg(this);
  config.minPrice = cfg.value("payment/min_price", 5.00).toDouble();
  config.maxPrice = cfg.value("payment/max_price", 999999.00).toDouble();
  config.minCount = cfg.value("payment/min_count", 1).toInt();
  config.maxCount = cfg.value("payment/max_count", 10).toInt();
  config.currency = cfg.value("payment/currency", "€").toByteArray();
  config.maxInteger = cfg.value("payment/max_integer", 9999999).toInt();
}

QWidget *PurchaseTableDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const {
  Q_UNUSED(option);
  if (!index.isValid())
    return parent;

  PurchaseTableColumn header = PurchaseTableModel::headerColumn(index.column());

  QVariant value = index.model()->data(index, Qt::EditRole);
  if (header.field() == "a_count") { // Count
    QSpinBox *m_spinBox = new QSpinBox(parent);
    m_spinBox->setMinimum(config.minCount);
    m_spinBox->setMaximum(config.maxCount);
    m_spinBox->setValue(value.toInt());
    return m_spinBox;
  }

  if (header.field() == "a_price") { // Price
    QDoubleSpinBox *m_price = new QDoubleSpinBox(parent);
    m_price->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_price->setReadOnly(true);
    m_price->setSuffix(config.currency);
    m_price->setRange(config.minPrice, config.maxPrice);
    m_price->setValue(value.toDouble());
    return m_price;
  }

  if (header.field() == "a_sell_price") { // Sell Price
    QDoubleSpinBox *m_sell_price = new QDoubleSpinBox(parent);
    m_sell_price->setSuffix(config.currency);
    m_sell_price->setRange(config.minPrice, config.maxPrice);
    m_sell_price->setValue(value.toDouble());
    return m_sell_price;
  }

  if (header.field() == "a_type") { // Media Type
    QComboBox *m_box = new QComboBox(parent);
    m_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_box->insertItem(0, tr("Unknown"), AntiquaCRM::ArticleType::UNKNOWN);
    m_box->insertItem(1, tr("Book"), AntiquaCRM::ArticleType::BOOK);
    m_box->insertItem(2, tr("Media"), AntiquaCRM::ArticleType::MEDIA);
    m_box->insertItem(3, tr("Print"), AntiquaCRM::ArticleType::PRINTS);
    m_box->insertItem(4, tr("Other"), AntiquaCRM::ArticleType::OTHER);
    int current = m_box->findData(value.toInt(), Qt::UserRole);
    m_box->setCurrentIndex(current);
    return m_box;
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

  PurchaseTableColumn header = PurchaseTableModel::headerColumn(index.column());

  if (header.field() == "a_count") { // Count
    QSpinBox *m_spinBox = qobject_cast<QSpinBox *>(editor);
    m_spinBox->setMinimum(1);
    m_spinBox->setValue(config.minCount);
    m_spinBox->setMaximum(config.maxCount);
    return;
  }

  if (header.field() == "a_price") { // Price
    QDoubleSpinBox *p = qobject_cast<QDoubleSpinBox *>(editor);
    p->setButtonSymbols(QAbstractSpinBox::NoButtons);
    p->setReadOnly(true);
    p->setSuffix(config.currency);
    p->setRange(config.minPrice, config.maxPrice);
    p->setValue(value.toDouble());
    return;
  }

  if (header.field() == "a_sell_price") { // Sell Price
    QDoubleSpinBox *p = qobject_cast<QDoubleSpinBox *>(editor);
    p->setSuffix(config.currency);
    p->setRange(config.minPrice, config.maxPrice);
    p->setValue(value.toDouble());
    return;
  }

  if (header.field() == "a_type") { // Type
    QComboBox *m_box = qobject_cast<QComboBox *>(editor);
    int current = m_box->findData(value.toInt(), Qt::UserRole);
    m_box->setCurrentIndex(current);
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

  PurchaseTableModel *m_ptm = qobject_cast<PurchaseTableModel *>(model);
  PurchaseTableColumn header = m_ptm->headerColumn(index.column());

  if (header.field() == "a_count") { // Count
    QSpinBox *sp = qobject_cast<QSpinBox *>(editor);
    m_ptm->setData(index, sp->value(), Qt::EditRole);
    return;
  }

  if (header.field() == "a_price") { // Price
    QDoubleSpinBox *dsp = qobject_cast<QDoubleSpinBox *>(editor);
    m_ptm->setData(index, dsp->value(), Qt::EditRole);
    return;
  }

  if (header.field() == "a_sell_price") { // Sell Price
    QDoubleSpinBox *dsp = qobject_cast<QDoubleSpinBox *>(editor);
    m_ptm->setData(index, dsp->value(), Qt::EditRole);
    return;
  }

  if (header.field() == "a_type") { // Type
    QComboBox *m_box = qobject_cast<QComboBox *>(editor);
    m_box->setFocusPolicy(Qt::NoFocus);
    QVariant value = m_box->itemData(m_box->currentIndex(), Qt::UserRole);
    m_ptm->setData(index, value.toInt(), Qt::EditRole);
    return;
  }
}
