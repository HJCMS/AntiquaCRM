// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "trustlist.h"
#include "antiqua_global.h"

#include <QObject>

Trust::Trust() {
  p_index = -1;
  p_title = QString();
}

Trust::Trust(int index, const QString &title) {
  p_index = index;
  p_title = title;
}

int Trust::index() { return p_index; }

const QString Trust::title() { return p_title; }

TrustList::TrustList() : p_list() {
  p_list.clear();
  p_list.append(Trust(0, QObject::tr("Trust Settings")));
  p_list.append(Trust(1, QObject::tr("Pays on time")));
  p_list.append(Trust(2, QObject::tr("Pays reliable")));
  p_list.append(Trust(3, QObject::tr("Pays with delay")));
  p_list.append(Trust(4, QObject::tr("Prepayment")));
  p_list.append(Trust(5, QObject::tr("No delivery or order")));
}

int TrustList::size() { return p_list.size(); };

const Trust TrustList::trust(int index) {
  Trust ret(p_list.at(0));
  for (int i = 0; i < p_list.size(); i++) {
    Trust cur = p_list.at(i);
    if (cur.index() == index)
      return cur;
  }
  return ret;
}

const int TrustList::index(int index) {
  Trust ret = trust(index);
  return ret.index();
}

const QString TrustList::title(int index) {
  Trust ret = trust(index);
  return ret.title();
}
