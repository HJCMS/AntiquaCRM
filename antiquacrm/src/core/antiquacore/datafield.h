// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DATAFIELD_ANTIQUACORE_H
#define DATAFIELD_ANTIQUACORE_H

#include "antiqua_global.h"

#include <QMetaType>
#include <QList>
#include <QString>
#include <QVariant>

/**
 * @brief SQL Datafield
 * @class DataField
 */
class ANTIQUACORE_EXPORT DataField {
private:
  QString p_field;
  int p_type;
  bool p_notnull;
  QVariant p_value;

public:
  explicit DataField();

  /**
   * @brief DataField
   * @param field   - set SQL column Fieldname
   * @param type    - set QVariant::Type
   * @param notnull - set SQL column "not null"
   * @param data    - set SQL column Value
   */
  DataField(const QString &field, int type, bool notnull, const QVariant &data);

  /**
   * @brief set SQL columns Fieldname
   */
  void setField(const QString &field);

  /**
   * @brief get SQL columns Fieldname
   */
  const QString field();

  /**
   * @brief set QVariant::Type
   */
  void setType(int type);

  /**
   * @brief get QVariant::Type
   */
  int type();

  /**
   * @brief set SQL column "not null"
   */
  void setRequired(bool b = false);

  /**
   * @brief is SQL column "not null"
   */
  bool isRequired();

  /**
   * @brief set SQL column Value
   */
  void setValue(const QVariant &value);

  /**
   * @brief get SQL column Value
   */
  const QVariant value();
};
Q_DECLARE_TYPEINFO(DataField, Q_PRIMITIVE_TYPE);

typedef QList<DataField> DataFields;
Q_DECLARE_METATYPE (DataFields)

#endif // DATAFIELD_ANTIQUACORE_H
