
#include "sqltitlesearch.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
// #include <QtWidgets/QGridLayout>
// #include <QtWidgets/QGroupBox>
// #include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QLabel>
// #include <QtWidgets/QSpacerItem>

#include <QtSql/QSqlQuery>
#include <QtWidgets/QHeaderView>

SQLTitleSearch::SQLTitleSearch(QWidget *parent) : QTableView{parent}
{
  setObjectName("SQLTitleSearch");
  setEditTriggers(QAbstractItemView::DoubleClicked);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior( QAbstractItemView::SelectRows );
  setSelectionMode( QAbstractItemView::SingleSelection );

  m_queryModel = new SQLDataModel(this);
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL ( doubleClicked(const QModelIndex&)),
     this, SLOT ( clickedGetArticleID(const QModelIndex&)));
}

/**
 * @brief SQLTitleSearch::clickedGetArticleID
 * @short Suche nach Artikel ID und Titeltext
 * @param index
 */
void SQLTitleSearch::clickedGetArticleID(const QModelIndex &index)
{
    // ib_id
    int id = m_queryModel->data(index.sibling(index.row(),0),Qt::EditRole).toInt();
    if(id>=1)
    {
        // ib_title
        QString str = m_queryModel->data(index.sibling(index.row(),2),Qt::EditRole).toString();
        QHash<int,QString> data;
        data.insert(id,str);
        emit articleSelected(data);
    }
}

void SQLTitleSearch::searchByTitle(const QString &str)
{
  QRegExp reg("^(\\s+)");
  if(reg.exactMatch(str))
  {
    qDebug() << "Rejected:" << str;
    return;
  }
  // QSqlQuery query = m_queryModel->query();
  // while (query.next()) { qDebug() << query.value(0) << query.value(1); }

  QString q("SELECT b.ib_id,b.ib_count,b.ib_title,b.ib_author,b.ib_publisher,b.ib_year,b.ib_price,s.sl_storage");
  q.append(" FROM inventory_books AS b ");
  q.append(" LEFT JOIN ref_storage_location AS s ON s.sl_id=b.ib_storage");
  q.append(" WHERE ( b.ib_title ILIKE '%");
  q.append(str);
  q.append("%') OR (b.ib_title_extended ILIKE '");
  q.append(str);
  q.append("%') ORDER BY b.ib_count DESC LIMIT 1000;");
  p_db = QSqlDatabase::database(sqlConnectionName);
  if (p_db.open()) {
    qDebug() << "{SQL Title Search} " << q;
    m_queryModel->setQuery(q, p_db);
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}
