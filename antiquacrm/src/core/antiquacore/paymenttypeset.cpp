// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymenttypeset.h"

#include <QDebug>
#include <QMapIterator>
#include <QObject>

PaymentTypeSet::PaymentTypeSet() : QMap<int, QString>{} {
  insert(0, QObject::tr("Without disclosures"));
  /** @brief  1: Banküberweisung (Vorkasse) */
  insert(1, QObject::tr("Bank transfer (prepayment)"));
  /** @brief  2: Offene Rechnung */
  insert(2, QObject::tr("open account"));
  /** @brief  3: Lastschrift (Vorkasse) */
  insert(3, QObject::tr("direct debit (prepayment)"));
  /** @brief  4: Kreditkarte (Vorkasse) */
  insert(4, QObject::tr("Credit card (prepayment)"));
  /** @brief  5: Nachnahme */
  insert(5, QObject::tr("cash on delivery"));
  /** @brief  6: PayPal (Vorkasse) */
  insert(6, QObject::tr("PayPal (prepayment)"));
  /** @brief  7: OFFEN - Nicht gesetzt */
  insert(7, QObject::tr("Unknown"));
  /** @brief  8: Skrill (Vorkasse) */
  insert(8, QObject::tr("Skrill (prepayment)"));
  /** @brief  9: Selbstabholung - Barzahlung */
  insert(9, QObject::tr("Pickup - cash payment"));
  /** @brief  10: Sofortüberweisung */
  insert(10, QObject::tr("Instant bank transfer"));
  /** @brief  11: Offene Rechnung (Vorkasse vorbehalten) */
  insert(11, QObject::tr("Invoice (prepayment reserved)"));
  /** @brief  12: Scheck */
  insert(12, QObject::tr("Check"));
}

int PaymentTypeSet::findKey(const QString &name) {
  QMap<int, QString>::const_iterator i = constBegin();
  while (i != constEnd()) {
    qDebug() << i.key() << ": " << i.value() << Qt::endl;
    if (i.value() == name)
      return i.key();

    ++i;
  }
  return -1;
}
