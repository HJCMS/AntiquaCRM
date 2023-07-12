// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configprinting.h"
#include "printerattachments.h"
#include "printerdevices.h"
#include "printerfonts.h"
#include "printerpaperlayout.h"
#include "printerqrcode.h"

#include <QtWidgets>

ConfigPrinting::ConfigPrinting(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"General", "printer", parent} {
  setWindowTitle(getMenuEntry().value("title").toString());

  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QVBoxLayout *layout = new QVBoxLayout(m_central);
  layout->setContentsMargins(5, 5, 5, 5);

  m_printers = new PrinterDevices(this);
  layout->addWidget(m_printers);

  m_fontGroup = new PrinterFonts(this);
  layout->addWidget(m_fontGroup);

  m_attachmentGroup = new PrinterAttachments(this);
  layout->addWidget(m_attachmentGroup);

  m_paperLayout = new PrinterPaperLayout(this);
  layout->addWidget(m_paperLayout);

  m_qrCode = new PrinterQRCode(this);
  layout->addWidget(m_qrCode);

  m_central->setLayout(layout);
  layout->addStretch(1);

  setWidget(m_central);
}

bool ConfigPrinting::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ModifiedChange) {
    QWidget *m_te = qobject_cast<QWidget *>(obj);
    if (m_te != nullptr && m_te->isWindowModified()) {
      setWindowModified(true);
      return true;
    }
  }
  return AntiquaCRM::PluginConfigWidget::eventFilter(obj, event);
}

void ConfigPrinting::loadSectionConfig() {
  config->beginGroup("printer");
  // Printers
  m_printers->loadSection(config);
  // Fonts
  m_fontGroup->loadSection(config);
  // Attachments
  m_attachmentGroup->loadSection(config);
  // Page Layout
  m_paperLayout->loadSection(config);
  // QRCode
  m_qrCode->loadSection(config);
  // end
  config->endGroup();

  QListIterator<QGroupBox *> it(findChildren<QGroupBox *>(QString()));
  while (it.hasNext()) {
    it.next()->installEventFilter(this);
  }
}

void ConfigPrinting::saveSectionConfig() {
  config->beginGroup("printer");
  // Printers
  m_printers->saveSection(config);
  // Fonts
  m_fontGroup->saveSection(config);
  // Attachments
  m_attachmentGroup->saveSection(config);
  // Page Layout
  m_paperLayout->saveSection(config);
  // QRCode
  m_qrCode->saveSection(config);
  // end
  config->endGroup();
  setWindowModified(false);
}

AntiquaCRM::PluginConfigWidget::ConfigType ConfigPrinting::getType() const {
  return AntiquaCRM::PluginConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ConfigPrinting::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "preferences-devices-printer");
  _o.insert("title", tr("Printer"));
  _o.insert("tooltip", tr("Configure Printers and Layout."));
  return _o;
}
