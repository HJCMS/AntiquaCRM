// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configprinting.h"
#include "printerattachments.h"
#include "printerfonts.h"

#include <QtWidgets>

ConfigPrinting::ConfigPrinting(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"General", "printer", parent} {
  setWindowTitle(getTitle());
  QWidget *m_central = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(m_central);
  layout->setContentsMargins(5, 5, 5, 5);

  m_fontGroup = new PrinterFonts(this);
  layout->addWidget(m_fontGroup);

  m_attachmentGroup = new PrinterAttachments(this);
  layout->addWidget(m_attachmentGroup);

  m_central->setLayout(layout);
  layout->addStretch(1);
  setWidget(m_central);
}

void ConfigPrinting::loadSectionConfig() {
  // Fonts
  m_fontGroup->loadFonts(config);
  // Attachments
  m_attachmentGroup->loadSection(config);

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

void ConfigPrinting::saveSectionConfig() {
  // Fonts
  m_fontGroup->saveFonts(config);
  // Attachments
  m_attachmentGroup->saveSection(config);

  QListIterator<AntiquaCRM::AbstractInput *> it(getInputList(widget()));
  while (it.hasNext()) {
    AntiquaCRM::AbstractInput *m_inp = it.next();
    if (m_inp->objectName().isEmpty())
      continue;

    config->setValue(m_inp->objectName(), m_inp->getValue());
  }
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigPrinting::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QIcon ConfigPrinting::getIcon() const {
  return AntiquaCRM::AntiquaApplIcon("configure");
}

const QString ConfigPrinting::getTitle() const { return tr("Printer"); }
