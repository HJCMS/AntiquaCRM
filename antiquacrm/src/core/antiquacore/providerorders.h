// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PROVIDERORDERS_ANTIQUACORE_H
#define PROVIDERORDERS_ANTIQUACORE_H

#include "antiqua_global.h"

#include <QList>
#include <QMetaType>
#include <QString>
#include <QStringList>

/**
 * @brief Dienstleister Datensätze
 * @class ProviderOrder
 * Die Klasse dient zum Verschieben von Dienstleister
 * Datensätzen zwischen den einzelnen Hauptfenstern.
 */
class ANTIQUACORE_EXPORT ProviderOrder {
private:
  QString p_provider;
  QString p_providerId;
  int p_customerId;
  QStringList p_articleIds;

public:
  explicit ProviderOrder();

  void setProvider(const QString &name);
  const QString provider();

  void setProviderId(const QString &id);
  const QString providerId();

  void setCustomerId(int &id);
  int customerId();

  void setArticleIds(const QStringList &ids);
  const QStringList articleIds();
};
Q_DECLARE_TYPEINFO(ProviderOrder, Q_PRIMITIVE_TYPE);

typedef QList<ProviderOrder> ProviderOrders;
Q_DECLARE_METATYPE(ProviderOrders)

#endif // PROVIDERORDERS_ANTIQUACORE_H
