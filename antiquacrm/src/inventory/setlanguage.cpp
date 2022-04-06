// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "setlanguage.h"
#include "version.h"

#include <QtCore/QDebug>

SetLanguage::SetLanguage(QWidget *parent) : QComboBox{parent} {
  setObjectName("SetLanguage");
  addItem(tr("German"), QString("de_DE"));
  addItem(tr("Czech"), QString("cs_CS"));
  addItem(tr("Danish"), QString("da_DA"));
  addItem(tr("English"), QString("en_EN"));
  addItem(tr("Spanish"), QString("es_ES"));
  addItem(tr("Finnish"), QString("fi_FI"));
  addItem(tr("French"), QString("fr_FR"));
  addItem(tr("Italian"), QString("it_IT"));
  addItem(tr("Dutch"), QString("nl_NL"));
  addItem(tr("Norwegian"), QString("nn_NN"));
  addItem(tr("Polish"), QString("pl_PL"));
  addItem(tr("Portuguese"), QString("pt_PT"));
  addItem(tr("Slovenian"), QString("sl_SL"));
  addItem(tr("Swedish"), QString("sv_SV"));
}

void SetLanguage::setValue(const QVariant &v) {
  int index = findData(v, Qt::UserRole, Qt::MatchExactly);
  setCurrentIndex(index);
}

const QVariant SetLanguage::value() {
  return itemData(currentIndex(), Qt::UserRole);
}
