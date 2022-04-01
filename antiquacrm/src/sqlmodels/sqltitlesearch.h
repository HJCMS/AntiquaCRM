/** @COPYRIGHT_HOLDER@ */

#ifndef SQLTITLESEARCH_H
#define SQLTITLESEARCH_H

#include "sqldatamodel.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QItemSelectionModel>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QTableView>

class SQLTitleSearch : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
    QSqlDatabase p_db;
    SQLDataModel *m_queryModel;

private Q_SLOTS:
    void clickedGetArticleID(const QModelIndex &);

Q_SIGNALS:
    void articleSelected(const QHash<int,QString>&);

public:
  explicit SQLTitleSearch(QWidget *parent = 0);
    void searchByTitle(const QString &);

};

#endif // SQLTITLESEARCH_H
