// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerorders.h"

ProviderOrder::ProviderOrder() {
  p_provider = QString();
  p_providerId = QString();
  p_customerId = -1;
  p_articleIds = QStringList();
}

void ProviderOrder::setProvider(const QString &name) { p_provider = name; }

const QString ProviderOrder::provider() { return p_provider; }

void ProviderOrder::setProviderId(const QString &id) { p_providerId = id; }

const QString ProviderOrder::providerId() { return p_providerId; }

void ProviderOrder::setCustomerId(int &id) {
  if (id > 0)
    p_customerId = id;
}

int ProviderOrder::customerId() { return p_customerId; }

void ProviderOrder::setArticleIds(const QStringList &ids) {
  if (ids.size() > 0)
    p_articleIds = ids;
}

const QStringList ProviderOrder::articleIds() { return p_articleIds; }
