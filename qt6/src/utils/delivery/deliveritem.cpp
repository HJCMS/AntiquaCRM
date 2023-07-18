// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliveritem.h"

DeliverItem::DeliverItem(int index, const QString &name)
    : d_srv{index}, d_class{name} {
  d_cid = -1;
  d_definition = QString();
  d_description = QString();
  d_international = false;
  d_default = false;
  d_price = 0.01;
  d_changed = QDateTime::currentDateTime();
}

int DeliverItem::index() const { return d_srv; }

const QString DeliverItem::name() const { return d_class; };

void DeliverItem::setPrimaryKey(int id) { d_cid = id; }

int DeliverItem::primaryKey() { return d_cid; }

void DeliverItem::setDefinition(const QString &d) { d_definition = d; }

const QString DeliverItem::definition() { return d_definition; }

void DeliverItem::setInternational(bool t) { d_international = t; }

bool DeliverItem::international() { return d_international; }

void DeliverItem::setDefault(bool b) { d_default = b; }

bool DeliverItem::isDefault() { return d_default; }

void DeliverItem::setDescription(const QString &d) { d_description = d; }

const QString DeliverItem::description() { return d_description; }

void DeliverItem::setPrice(qreal p) { d_price = p; }

qreal DeliverItem::price() { return d_price; }

void DeliverItem::setChanged(const QDateTime &dt) { d_changed = dt; }

const QDateTime DeliverItem::lastChanged() { return d_changed; }
