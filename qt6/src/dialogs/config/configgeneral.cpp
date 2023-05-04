// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configgeneral.h"
#include "paymentsettingsgroup.h"

#include <QLocale>
#include <QMetaType>
#include <QtWidgets>

ConfigGeneral::ConfigGeneral(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"General", QString(), parent} {
  setWindowTitle(getTitle());
  // Central Widget
  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QVBoxLayout *layout = new QVBoxLayout(m_central);
  layout->setContentsMargins(5, 5, 5, 5);

  m_paymentGroup = new PaymentSettingsGroup(m_central);
  layout->addWidget(m_paymentGroup);

  m_central->setLayout(layout);
  layout->addStretch(1);
  setWidget(m_central);
}

void ConfigGeneral::loadSectionConfig() {
  QListIterator<AntiquaCRM::AbstractInput *> it(getInputList(widget()));
  while (it.hasNext()) {
    AntiquaCRM::AbstractInput *m_inp = it.next();
    if (m_inp->objectName().isEmpty())
      continue;

    const QMetaType _type = m_inp->getType();
    QVariant _val = config->getValue(m_inp->objectName(), _type);
    if (_val.isNull())
      continue;

    m_inp->setValue(_val);
  }
}

void ConfigGeneral::saveSectionConfig() {
  QListIterator<AntiquaCRM::AbstractInput *> it(getInputList(widget()));
  while (it.hasNext()) {
    AntiquaCRM::AbstractInput *m_inp = it.next();
    if (m_inp->objectName().isEmpty())
      continue;

    config->setValue(m_inp->objectName(), m_inp->getValue());
  }
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigGeneral::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QIcon ConfigGeneral::getIcon() const {
  return AntiquaCRM::AntiquaApplIcon("configure");
}

const QString ConfigGeneral::getTitle() const { return tr("General"); }
