// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configgeneral.h"
#include "paymentsettingsgroup.h"

#include <QLocale>
#include <QtWidgets>

ConfigGeneral::ConfigGeneral(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"General", QString(), parent} {
  // Central Widget
  QWidget *m_central = new QWidget(this);
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
    QVariant _val = config->value(m_inp->objectName());
    if (_val.isNull())
      continue;

    m_inp->setValue(_val);
  }
}

void ConfigGeneral::saveSectionConfig() {
  QListIterator<AntiquaCRM::AbstractInput *> it(getInputList(widget()));
  while (it.hasNext()) {
    AntiquaCRM::AbstractInput *m_inp = it.next();
    config->setValue(m_inp->objectName(), m_inp->getValue());
  }
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigGeneral::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QIcon ConfigGeneral::getIcon() const {
  return QIcon("://icons/configure.png");
}

const QString ConfigGeneral::getTitle() const { return tr("General"); }
