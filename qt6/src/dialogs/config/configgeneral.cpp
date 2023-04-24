// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configgeneral.h"

#include <QLocale>
#include <QtWidgets>

ConfigGeneral::ConfigGeneral(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"General", QString(), parent} {
  setContentsMargins(0, 0, 0, 0);

  QGroupBox *moneyGroup = new QGroupBox(this);
  moneyGroup->setTitle(tr(""));

  qint8 row = 0;
  QGridLayout *layout = new QGridLayout(moneyGroup);
  layout->setContentsMargins(5, 5, 5, 5);

  m_currency = new AntiquaCRM::CurrencySelector(this);
  m_currency->setBuddyLabel(tr("Currency"));
  m_currency->appendStretch(1);
  layout->addWidget(m_currency, row++, 0, 1, 2);

  m_vatNormal = new AntiquaCRM::NumEdit(this);
  m_vatNormal->setBuddyLabel(tr("Tax Normal"));
  m_vatNormal->setSuffix("%");
  m_vatNormal->setValue(19);
  layout->addWidget(m_vatNormal, row, 0, 1, 1);

  m_vatReduced = new AntiquaCRM::NumEdit(this);
  m_vatReduced->setBuddyLabel(tr("Tax Reduced"));
  m_vatReduced->setSuffix("%");
  m_vatReduced->appendStretch(1);
  m_vatReduced->setValue(7);
  layout->addWidget(m_vatReduced, row++, 1, 1, 1);

  layout->setRowStretch(row++, 1);
  moneyGroup->setLayout(layout);

  setWidget(moneyGroup);
}

void ConfigGeneral::initUi() {
}

void ConfigGeneral::loadSectionConfig() { initUi(); }

void ConfigGeneral::saveSectionConfig() {}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigGeneral::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QIcon ConfigGeneral::getIcon() const {
  return QIcon("://icons/configure.png");
}

const QString ConfigGeneral::getTitle() const { return tr("General"); }
