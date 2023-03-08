// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_TABLE_H
#define KEYWORD_TABLE_H

#include <QHeaderView>
#include <QObject>
#include <QSqlDatabase>
#include <QTableView>
#include <QWidget>

class KeywordTableModel;

/**
 * @ingroup KeywordsEdit
 * @brief The KeywordTable class
 */
class KeywordTable : public QTableView {
  Q_OBJECT

private:
  const QSqlDatabase p_db;
  KeywordTableModel *m_model;
  QHeaderView *m_headerView;

private Q_SLOTS:
  void keywordEditClicked(const QModelIndex &index);

Q_SIGNALS:
  void sendEditKeyword(int id);

public Q_SLOTS:
  void refresh();
  void searchKeyword(const QString &);

public:
  explicit KeywordTable(const QSqlDatabase &db, QWidget *parent = nullptr);
};

#endif // KEYWORD_TABLE_H
