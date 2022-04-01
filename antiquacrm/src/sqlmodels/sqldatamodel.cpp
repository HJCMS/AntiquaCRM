
#include "version.h"
#include "sqldatamodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtSql */
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>

SQLDataModel::SQLDataModel(QObject *parent)
    : QSqlQueryModel{parent}
{
  setObjectName("SQLBookModel");
}

QVariant SQLDataModel::data(const QModelIndex &index, int role) const
{
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);
  switch (index.column())
  {
      case 0: // ib_id
        return item.toUInt();

      case 1: // ib_count
        return item.toUInt();

      case 2: // ib_title
        return item.toString();

      case 3: // ib_author
        return item.toString();

      case 4: // ib_publisher
        return item.toString();

      case 5: // ib_year
        return item.toUInt();

      case 6: // ib_price
      {
        bool b = false;
        QVariant buffer = item.toDouble(&b);
        if (!b)
        {
          buffer = 0.00;
        }
        if(role == Qt::DisplayRole)
        {
          QString str;
          buffer = str.asprintf("%.02f €",buffer.toFloat());
        }
        return buffer;
      }

      case 7: // ib_storage
        return item.toString();

      default: // nicht registrierter Datentype !!!
        return item;
  }
}

QVariant SQLDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // ib_id
      return tr("Article");

    case 1: // ib_count
      return tr("Count");

    case 2: // ib_title
      return tr("Titel");

    case 3: // ib_author
      return tr("Author");

    case 4: // ib_publisher
      return tr("Publisher");

    case 5: // ib_year
      return tr("Year");

    case 6: // ib_price
      return tr("Price");

    case 7: // ib_storage
      return tr("Storage Location");

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
