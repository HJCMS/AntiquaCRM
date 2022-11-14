// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLE_H
#define ANTIQUACRM_PURCHASETABLE_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QContextMenuEvent>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>

struct EditorProperties {
  qreal minPrice = 3.00;
  qreal maxPrice = 999999.00;
  int minCount = 1;
  int maxCount = 10;
  int maxInteger = 9999999;
  QByteArray currency = QString("€").toLocal8Bit();
};

class PurchaseTableHeader;

class PurchaseTable final : public QTableWidget {
  Q_OBJECT

private:
  const QString tableName;
  EditorProperties config;

  AntiquaCRM::ASqlCore *m_sql;
  QSqlRecord p_tableRecord;

  PurchaseTableHeader *m_header;
  void setHeaders();

  QTableWidgetItem *stringItem(const QSqlField &field) const;
  const QVariant getStringItem(int row, int column) const;

  QSpinBox *integerItem(const QSqlField &field) const;
  qint64 getIntegerItem(int row, int column) const;

  QDoubleSpinBox *priceItem(const QSqlField &field) const;
  double getPriceItem(int row, int column) const;

  SelectArticleType *typeItem(const QSqlField &field) const;
  int getTypeItem(int row, int column) const;

  const QString createSqlUpdate(int row, qint64 id);

  const QString createSqlInsert(int row);

  bool removeTableRow(int row);

protected:
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void removeCurrentRow();

public Q_SLOTS:
  void setQueryId(qint64 id, const QString &field = QString("a_order_id"));

public:
  explicit PurchaseTable(QWidget *parent = nullptr, bool readOnly = false);
  bool save();
  bool setRequiredIds(qint64 oId, qint64 cId);
  void hideColumns(const QList<int> columns);
  bool addRow(const AntiquaCRM::OrderArticleItems &items);
  const AntiquaCRM::OrderArticleItems getRow(int row);
};

#endif // ANTIQUACRM_PURCHASETABLE_H
