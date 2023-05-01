// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configpaths.h"
#include "pathsettingsgroup.h"

#include <QtWidgets>

ConfigPaths::ConfigPaths(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"General", QString(), parent} {
  setWindowTitle(getTitle());
  // Central Widget
  QWidget *m_central = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(m_central);
  layout->setContentsMargins(5, 5, 5, 5);

  m_pathsGroup = new PathSettingsGroup(m_central);
  layout->addWidget(m_pathsGroup);

  m_central->setLayout(layout);
  layout->addStretch(1);
  setWidget(m_central);
}

void ConfigPaths::loadSectionConfig() {
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

void ConfigPaths::saveSectionConfig() {
  QListIterator<AntiquaCRM::AbstractInput *> it(getInputList(widget()));
  while (it.hasNext()) {
    AntiquaCRM::AbstractInput *m_inp = it.next();
    if (m_inp->objectName().isEmpty())
      continue;

    config->setValue(m_inp->objectName(), m_inp->getValue());
  }
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigPaths::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QIcon ConfigPaths::getIcon() const {
  return AntiquaCRM::AntiquaApplIcon("configure");
}

const QString ConfigPaths::getTitle() const { return tr("Paths"); }
