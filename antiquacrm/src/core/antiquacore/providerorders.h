// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PROVIDERORDERS_ANTIQUACORE_H
#define PROVIDERORDERS_ANTIQUACORE_H

#include "antiqua_global.h"

#include <QHash>
#include <QList>
#include <QMetaObject>
#include <QString>
#include <QVariant>

/**
 * @brief Dienstleister Datensätze
 * @class ProviderOrder
 * Die Klasse dient zum Verschieben von Dienstleister
 * Datensätzen zwischen den einzelnen Hauptfenstern.
 */
class ANTIQUACORE_EXPORT ProviderOrder {
private:
  QString p_group;
  QString p_param;
  QString p_field;
  QVariant p_value;

public:
  /**
   * @brief ProviderShipping
   * @param param - Parameter von Json Dokument
   * @param field - Der SQL Spaltenname wo es hin soll
   * @param value
   */
  explicit ProviderOrder(const QString &param, const QString &field,
                            const QVariant &value = QVariant(),
                            const QString &group = QString());

  /**
   * @brief Erstellt einen leeren container
   */
  ProviderOrder();

  /**
   * @brief Json{Tree} Gruppe
   */
  void setGroup(const QString &p);

  /**
   * @brief Json Gruppe
   */
  const QString group();

  /**
   * @brief Json{Tree} Parameter
   */
  void setParam(const QString &p);

  /**
   * @brief Json parameter
   */
  const QString param();

  /**
   * @brief SQL Feldname setzen
   */
  void setFieldname(const QString &f);

  /**
   * @brief SQL  Feldname
   */
  const QString fieldname();

  /**
   * @brief Wert setzen
   */
  void setValue(const QVariant &v);

  /**
   * @brief Rückgabewert
   */
  const QVariant value();
};
Q_DECLARE_TYPEINFO(ProviderOrder, Q_PRIMITIVE_TYPE);

typedef QList<ProviderOrder> ProviderOrders;
Q_DECLARE_METATYPE(ProviderOrders)

#endif // PROVIDERORDERS_ANTIQUACORE_H
