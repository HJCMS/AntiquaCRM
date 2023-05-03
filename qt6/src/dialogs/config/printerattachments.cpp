// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerattachments.h"

#include <QtWidgets>

PrinterAttachments::PrinterAttachments(QWidget *parent) : QGroupBox{parent} {
  setTitle(tr("Document heading attachment."));
  QGridLayout *layout = new QGridLayout(this);

  m_path = new AntiquaCRM::SelectTargets(this);
  m_path->setObjectName("print_attachments_path");
  m_path->setBuddyLabel(tr("Directory"));
  layout->addWidget(m_path, 0, 0, 1, 1);

  m_watermark = new AntiquaCRM::SelectFile(this);
  m_watermark->setObjectName("print_watermark_file");
  m_watermark->setBuddyLabel(tr("Watermark"));
  layout->addWidget(m_watermark, 1, 0, 1, 1);

  m_opacity = new AntiquaCRM::ASlider(this);
  m_opacity->setObjectName("print_watermark_opacity");
  m_opacity->setRange(1, 10);
  m_opacity->setValue(5);
  layout->addWidget(m_opacity, 2, 0, 1, 1);

  m_opacityLabel = new AntiquaCRM::ALabel(this);
  layout->addWidget(m_opacityLabel, 2, 1, 1, 1);

  setLayout(layout);

  connect(m_opacity, SIGNAL(valueChanged(int)), SLOT(opacityChanged(int)));
}

void PrinterAttachments::opacityChanged(int i) {
  if (i == m_opacity->minimum()) {
    m_opacityLabel->setText("0.1");
  } else if (i == m_opacity->maximum()) {
    m_opacityLabel->setText("1.0");
  } else {
    m_opacityLabel->setText(QString::asprintf("0.%d", i));
  }
}

void PrinterAttachments::loadSection(AntiquaCRM::ASettings *config) {
  config->beginGroup("printer");
  QDir _dir(config->value("print_attachments_path").toString());
  if (_dir.exists())
    m_path->setValue(_dir.path());

  QFileInfo _file(_dir, config->value("print_watermark_file").toString());
  if (_file.isReadable())
    m_watermark->setValue(_file.filePath());

  QString _pwo(config->value("print_watermark_opacity").toString());
  for (int i = m_opacity->minimum(); i < m_opacity->maximum(); ++i) {
    if (QString::asprintf("0.%d", i) == _pwo) {
      m_opacity->setValue(i);
      break;
    }
  }
  config->endGroup();
}

void PrinterAttachments::saveSection(AntiquaCRM::ASettings *config) {
  config->beginGroup("printer");
  QDir _dir(m_path->getValue().toString());
  if (_dir.exists())
    config->setValue("print_attachments_path", _dir.path());

  QFileInfo _file(_dir, m_watermark->getValue().toString());
  if (_file.isReadable())
    config->setValue("print_watermark_file", _file.fileName());

  QString _opacity(m_opacityLabel->text());
  config->setValue("print_watermark_opacity", _opacity.toDouble());
  config->endGroup();
}
