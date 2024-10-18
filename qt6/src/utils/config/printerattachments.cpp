// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerattachments.h"

#include <QDir>
#include <QFileInfo>
#include <QLayout>

PrinterAttachments::PrinterAttachments(QWidget* parent) : QGroupBox{parent} {
  setTitle(tr("Document heading attachment."));
  QVBoxLayout* layout = new QVBoxLayout(this);

  m_path = new AntiquaCRM::SelectTargets(this);
  m_path->setObjectName("print_attachments_path");
  m_path->setBuddyLabel(tr("Directory"));
  layout->addWidget(m_path);

  m_watermark = new AntiquaCRM::SelectFile(this);
  m_watermark->setObjectName("print_watermark_file");
  m_watermark->setBuddyLabel(tr("Watermark"));
  layout->addWidget(m_watermark);

  QHBoxLayout* opacityLayout = new QHBoxLayout();

  AntiquaCRM::ALabel* m_opacityInfo = new AntiquaCRM::ALabel(this);
  m_opacityInfo->setText(tr("Opacity") + ":");
  opacityLayout->addWidget(m_opacityInfo);

  m_opacity = new AntiquaCRM::ASlider(this);
  m_opacity->setObjectName("print_watermark_opacity");
  m_opacity->setRange(1, 10);
  opacityLayout->addWidget(m_opacity);

  m_opacityLabel = new AntiquaCRM::ALabel(this);
  opacityLayout->addWidget(m_opacityLabel);

  layout->addLayout(opacityLayout);

  setLayout(layout);

  connect(m_path, SIGNAL(sendInputChanged()), SLOT(subWidgetChanged()));
  connect(m_watermark, SIGNAL(sendInputChanged()), SLOT(subWidgetChanged()));
  connect(m_opacity, SIGNAL(valueChanged(int)), SLOT(opacityChanged(int)));
}

void PrinterAttachments::subWidgetChanged() {
  setWindowModified(true);
}

void PrinterAttachments::opacityChanged(int i) {
  if (i == m_opacity->minimum()) {
    m_opacityLabel->setText("0.1");
  } else if (i == m_opacity->maximum()) {
    m_opacityLabel->setText("1.0");
  } else {
    QString _val = QString::asprintf("0.%d", i);
    m_opacityLabel->setText(_val);
  }
  subWidgetChanged();
}

void PrinterAttachments::loadSection(AntiquaCRM::ASettings* config) {
  QDir _dir(config->value("print_attachments_path").toString());
  if (_dir.exists()) {
    m_path->setValue(_dir.path());
    QFileInfo _file(_dir, config->value("print_watermark_file").toString());
    if (_file.isReadable()) {
      m_watermark->setValue(_file.filePath());
    } else {
      qWarning("Missing print_watermark_file:%s", qPrintable(_file.fileName()));
    }
  } else {
    qWarning("Missing print_attachments_path:%s", qPrintable(_dir.path()));
  }

  QString _pwo(config->value("print_watermark_opacity").toString());
  for (int i = m_opacity->minimum(); i < m_opacity->maximum(); ++i) {
    if (QString::asprintf("0.%d", i) == _pwo) {
      m_opacity->setValue(i);
      break;
    }
  }
}

void PrinterAttachments::saveSection(AntiquaCRM::ASettings* config) {
  QDir _dir(m_path->getValue().toString());
  if (_dir.exists()) {
    config->setValue("print_attachments_path", _dir.path());
    QFileInfo _file(_dir, m_watermark->getValue().toString());
    if (_file.isReadable()) {
      config->setValue("print_watermark_file", _file.fileName());
    } else {
      qWarning("Missing print_watermark_file:%s", qPrintable(_file.fileName()));
    }
  } else {
    qWarning("Missing print_attachments_path:%s", qPrintable(_dir.path()));
  }

  QString _opacity(m_opacityLabel->text());
  config->setValue("print_watermark_opacity", _opacity.toDouble());
}
