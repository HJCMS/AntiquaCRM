// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodemodel.h"

#include <AntiquaCRM>
#include <QApplication>

namespace AntiquaCRM {

PostalCodeModel::PostalCodeModel(QObject *parent) : QAbstractListModel{parent} {
  p_palette = qApp->palette();
}

int PostalCodeModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return p_codes.size();
}

int PostalCodeModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant PostalCodeModel::data(const QModelIndex &index, int role) const {
  const QVariant _empty;
  if (!index.isValid())
    return _empty;

  int _row = index.row();
  const AntiquaCRM::PostalCode _code = p_codes[_row];
  if (_code.plz.isEmpty())
    return _empty;

  switch (role) {
  case (Qt::UserRole):
  case (Qt::EditRole): {
    if (index.column() == 1)
      return _code.location;
    else if (index.column() == 2)
      return _code.state;
    else
      return _code.plz;
  } break;

  case (Qt::DisplayRole): {
    QString _v(_code.plz);
    _v.append(" ");
    _v.append(_code.location);
    return _v;
  } break;

  case (Qt::ToolTipRole): {
    QString _v(_code.plz);
    _v.append(" (");
    _v.append(_code.location);
    QString st = _code.state;
    if (!st.isEmpty()) {
      _v.append("," + st);
    }
    _v.append(")");
    return _v;
  } break;

  case (Qt::BackgroundRole):
    return ((_row % 2) & 1) ? p_palette.alternateBase() : p_palette.base();

  default:
    return _empty;
  };

  return _empty;
}

void PostalCodeModel::initModel(const QString &country) {
  if (country.isEmpty())
    return;

  beginResetModel();
  p_codes.clear();

  AntiquaCRM::ASharedDataFiles dataFile;
  if (dataFile.fileExists(QString("postalcodes"))) {
    QJsonDocument jdoc = dataFile.getJson("postalcodes");
    QJsonObject _obj = jdoc.object();
    QJsonArray _array = _obj.value(country).toArray();
    QString _country = tr("Unknown");
    if (_obj.contains("tables")) {
      QJsonObject _tables = _obj.value("tables").toObject();
      _country = _tables.value(country).toString();
    }
    if (_array.size() > 0) {
      for (int i = 0; i < _array.size(); i++) {
        QJsonObject _cobj = _array[i].toObject();
        AntiquaCRM::PostalCode _pcode;
        _pcode.plz = QString::number(_cobj.value("plz").toInt());
        _pcode.location = _cobj.value("location").toString();
        _pcode.state = _cobj.value("state").toString();
        _pcode.country = _country;
        p_codes.append(_pcode);
      }
    }
  } else {
    qWarning("PostalCodeEdit:No postalcodes.json - fallback to SQL query!");
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QString _sql("SELECT p_plz,p_location,p_state,p_country");
    _sql.append(" FROM " + country);
    _sql.append(" LEFT JOIN ui_postalcodes ON p_table='" + country + "'");
    _sql.append(" ORDER BY p_plz;");
    QSqlQuery q = m_sql->query(_sql);
    if (q.size() > 0) {
      p_codes.clear();
      while (q.next()) {
        AntiquaCRM::PostalCode _pcode;
        _pcode.plz = QString::number(q.value("p_plz").toInt());
        _pcode.location = q.value("p_location").toString();
        _pcode.state = q.value("p_state").toString();
        _pcode.country = q.value("p_country").toString();
        p_codes.append(_pcode);
      }
    }
  }
  endResetModel();
}

} // namespace AntiquaCRM
