// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_VIEWS_TABLE_H
#define INVENTORY_VIEWS_TABLE_H

#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

#include <SqlCore>

class ViewsTableModel : public QSqlTableModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QByteArray currency;
  const QString translations(const QString &key) const;
  const QString getFieldName(const QModelIndex &index) const;

public:
  ViewsTableModel(const QSqlDatabase &db, QWidget *parent = nullptr);
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;

  virtual QVariant data(const QModelIndex &index,
                        int role = Qt::DisplayRole) const;
};

/**
 * @brief Hier werden nur PostgreSQL Sichten ausgegeben.
 * @class ViewsTable
 * @ingroup Inventory
 */
class ViewsTable : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  ViewsTableModel *m_model;
  QHeaderView *m_headerView;

private Q_SLOTS:
  void articleClicked(const QModelIndex &);

Q_SIGNALS:
  void s_queryFinished();
  void s_articleSelected(int articleId, const QString &editor);

public Q_SLOTS:
  /**
   * @brief Sichten Tabellenname hier entgegen nehmen.
   */
  void sqlQuery(const QString &viewtable);

public:
  ViewsTable(QWidget *parent = nullptr);
  ~ViewsTable();
};

#endif // INVENTORY_VIEWS_TABLE_H
