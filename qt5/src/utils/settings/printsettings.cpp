// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsettings.h"
#include "printerfonts.h"
#include "printersetup.h"
#include "printinglayout.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QStyle>
#include <QtWidgets>

static const QString documentLocation() {
  QStringList list;
  list << QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
  return list.first();
}

PrintSettings::PrintSettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("printing_settings");

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setObjectName("printing_config_layout");

  // PrinterFonts
  m_printerFonts = new PrinterFonts(this);
  layout->addWidget(m_printerFonts, row++, 0, 1, 2);

  // Attachments
  QGroupBox *m_attachGroup = new QGroupBox(this);
  m_attachGroup->setTitle(tr("Specification for Attachments."));

  QGridLayout *attachLayout = new QGridLayout(m_attachGroup);
  m_attachments = new LineEdit(m_attachGroup);
  m_attachments->setObjectName("attachments");
  m_attachments->setInfo(tr("Attachments"));
  m_attachments->setValue(documentLocation());
  attachLayout->addWidget(m_attachments, 0, 0, 1, 1);

  QPushButton *btn_attach =
      new QPushButton(tr("Open Directory"), m_attachGroup);
  btn_attach->setObjectName("btn_attachments");
  btn_attach->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
  attachLayout->addWidget(btn_attach, 0, 1, 1, 1);

  m_watermark = new LineEdit(m_attachGroup);
  m_watermark->setObjectName("watermark");
  m_watermark->setInfo(tr("Watermark"));
  m_watermark->setValue(documentLocation());
  attachLayout->addWidget(m_watermark, 1, 0, 1, 1);

  QPushButton *btn_watermark = new QPushButton(tr("Open File"), m_attachGroup);
  btn_watermark->setObjectName("btn_watermark");
  btn_watermark->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
  attachLayout->addWidget(btn_watermark, 1, 1, 1, 1);

  m_opacity = new OpacitySlider(m_attachGroup);
  m_opacity->setObjectName("watermark_opacity");
  m_opacity->setInfo(tr("Watermark opacity"));
  attachLayout->addWidget(m_opacity, 2, 0, 1, 2);

  layout->addWidget(m_attachGroup, row++, 0, 1, 2);

  // PrinterSetup
  m_printerSetup = new PrinterSetup(this);
  layout->addWidget(m_printerSetup, row++, 0, 1, 2);

  // PrintMargins
  m_printLayout = new PrintingLayout(this);
  layout->addWidget(m_printLayout, row++, 0, 1, 2);

  qrcodeGroup = new QGroupBox(this);
  qrcodeGroup->setTitle(tr("QR Code Settings"));
  QGridLayout *qrLayout = new QGridLayout(qrcodeGroup);
  int qrRow = 0;
  QString qrInfo = tr("The settings for the connectivity to the company server "
                      "are set in this area. These settings will be embedded "
                      "in the QR image. Note: Please keep in mind that QR "
                      "images must be recreated if you change something here.");
  QLabel *qrInfoLabel = new QLabel(qrInfo, qrcodeGroup);
  qrInfoLabel->setWordWrap(true);
  qrLayout->addWidget(qrInfoLabel, qrRow++, 0, 1, 2);
  qrLayout->addWidget(new QLabel(tr("Url Schema"), qrcodeGroup), qrRow, 0, 1, 1,
                      Qt::AlignRight);
  qr_server_schema = new LineEdit(qrcodeGroup);
  qr_server_schema->setObjectName("qrcode_schema");
  qrLayout->addWidget(qr_server_schema, qrRow++, 1, 1, 1);
  qrLayout->addWidget(new QLabel(tr("Server Address"), qrcodeGroup), qrRow, 0,
                      1, 1, Qt::AlignRight);
  qr_server_address = new LineEdit(qrcodeGroup);
  qr_server_address->setObjectName("qrcode_server");
  qrLayout->addWidget(qr_server_address, qrRow++, 1, 1, 1);
  qrLayout->addWidget(new QLabel(tr("Url Path"), qrcodeGroup), qrRow, 0, 1, 1,
                      Qt::AlignRight);
  qr_server_path = new LineEdit(qrcodeGroup);
  qr_server_path->setObjectName("qrcode_path");
  qrLayout->addWidget(qr_server_path, qrRow++, 1, 1, 1);
  qrLayout->addWidget(new QLabel(tr("Query Parameter"), qrcodeGroup), qrRow, 0,
                      1, 1, Qt::AlignRight);
  qr_server_query = new LineEdit(qrcodeGroup);
  qr_server_query->setObjectName("qrcode_parameter");
  qrLayout->addWidget(qr_server_query, qrRow++, 1, 1, 1);
  qrcodeGroup->setLayout(qrLayout);
  layout->addWidget(qrcodeGroup, row++, 0, 1, 2);

  layout->setColumnStretch(0, 75);
  layout->setColumnStretch(1, 25);
  layout->setRowStretch(row, 1);
  setLayout(layout);

  connect(btn_attach, SIGNAL(clicked()), this, SLOT(openDirectoryDialog()));
  connect(btn_watermark, SIGNAL(clicked()), this, SLOT(openWatermarkDialog()));
}

void PrintSettings::openDirectoryDialog() {
  QString ret = getDirectory(m_attachments->value().toString());
  QFileInfo info(ret);
  if (info.isDir() && info.isReadable()) {
    m_attachments->setValue(ret);
    chieldModified(true);
  }
}

void PrintSettings::openWatermarkDialog() {
  QString dest = m_attachments->value().toString();
  QStringList mimetypes({"image/jpeg", "image/png"});
  QFileDialog *dialog = new QFileDialog(this, tr("Watermark"), dest);
  dialog->setMimeTypeFilters(mimetypes);
  dialog->setNameFilter(tr("Image") + " (*.jpg *.png *.jpeg)");
  if (dialog->exec() == QDialog::Accepted) {
    QStringList list = dialog->selectedFiles();
    if (list.count() > 0) {
      QFileInfo info(list.first());
      if (info.isReadable()) {
        m_watermark->setValue(info.fileName());
        chieldModified(true);
      }
    }
  }
}

void PrintSettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString PrintSettings::getPageTitle() { return pageTitle; }

void PrintSettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon PrintSettings::getPageIcon() { return pageIcon; }

void PrintSettings::loadSectionConfig() {
  // Schriften
  m_printerFonts->loadFonts(config);

  m_printerSetup->init(config);

  QMarginsF ma;
  config->beginGroup("printer/table_margins");
  ma.setLeft(config->value("left", 20.0).toReal());
  ma.setTop(1.0);
  ma.setRight(config->value("right", 20.0).toReal());
  ma.setBottom(1.0);
  config->endGroup();
  m_printLayout->setValue(ma);

  config->beginGroup("printer");

  int sp = config->value("subject_position", 120).toInt();
  m_printLayout->setSubjectPosition(sp);

  int bp = config->value("body_position", 20).toInt();
  m_printLayout->setBodyPosition(bp);

  qreal rp = config->value("recipient_padding_top", 4.9).toReal();
  m_printLayout->setRecipientPosition(rp);

  m_attachments->setValue(config->value("attachments", documentLocation()));
  m_watermark->setValue(config->value("watermark"));
  m_opacity->setValue(config->value("watermark_opacity", 0.6));
  config->endGroup();
  config->beginGroup("qrcode");
  foreach (QString key, config->allKeys()) {
    LineEdit *e = qrcodeGroup->findChild<LineEdit *>("qrcode_" + key);
    if (e != nullptr) {
      e->setValue(config->value(key));
      e->setModified(false);
    }
  }
  config->endGroup();
}

void PrintSettings::saveSectionConfig() {
  // Schriften
  m_printerFonts->saveFonts(config);

  // Tabellenabstände
  QMarginsF ma = m_printLayout->value();
  config->beginGroup("printer/table_margins");
  config->setValue("left", ma.left());
  config->setValue("top", ma.top());
  config->setValue("right", ma.right());
  config->setValue("bottom", ma.bottom());
  config->endGroup();

  config->beginGroup("printer");

  int sp = m_printLayout->getSubjectPosition();
  config->setValue("subject_position", sp);

  int bp = m_printLayout->getBodyPosition();
  config->setValue("body_position", bp);

  qreal rp = m_printLayout->getRecipientPosition();
  config->setValue("recipient_padding_top", rp);

  config->setValue("watermark", m_watermark->value());
  config->setValue("watermark_opacity", m_opacity->value().toReal());
  config->setValue("attachments", m_attachments->value());
  config->setValue("DIN_A4_Printer", m_printerSetup->mainPrinter());
  config->setValue("DIN_A6_Printer", m_printerSetup->slavePrinter());
  config->endGroup();

  config->beginGroup("qrcode");
  QList<LineEdit *> ledits = qrcodeGroup->findChildren<LineEdit *>(QString());
  if (ledits.count() > 1) {
    for (int i = 0; i < ledits.count(); i++) {
      LineEdit *e = ledits.at(i);
      if (e != nullptr) {
        QStringList section(e->objectName().split("_"));
        config->setValue(section.last(), e->value());
        e->setModified(false);
      }
    }
  }
  config->endGroup();
}
