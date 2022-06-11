// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsettings.h"
#include "filedialog.h"
#include "myicontheme.h"

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

  m_signalMapper = new QSignalMapper(this);
  m_signalMapper->setObjectName("printing_signals");

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setObjectName("printing_config_layout");

  QString minfo = tr("Printer settings for delivery note and invoicing.");
  QLabel *mainInfo = new QLabel(minfo, this);
  layout->addWidget(mainInfo, row++, 0, 1, 2, Qt::AlignLeft);

  header_font = new QLabel(tr("Document Header"), this);
  header_font->setObjectName("header_font");
  layout->addWidget(header_font, row, 0, 1, 1, grid_label_align);

  QPushButton *btn_header = setFontButton("header_font");
  layout->addWidget(btn_header, row++, 1, 1, 1);

  body_font = new QLabel(tr("Document Body"), this);
  body_font->setObjectName("body_font");
  layout->addWidget(body_font, row, 0, 1, 1, grid_label_align);

  QPushButton *btn_body = setFontButton("body_font");
  layout->addWidget(btn_body, row++, 1, 1, 1);

  footer_font = new QLabel(tr("Document Footer"), this);
  footer_font->setObjectName("footer_font");
  layout->addWidget(footer_font, row, 0, 1, 1, grid_label_align);

  QPushButton *btn_footer = setFontButton("footer_font");
  layout->addWidget(btn_footer, row++, 1, 1, 1);

  normal_font = new QLabel(tr("Normal Text"), this);
  normal_font->setObjectName("normal_font");
  layout->addWidget(normal_font, row, 0, 1, 1, grid_label_align);

  QPushButton *btn_normal = setFontButton("normal_font");
  layout->addWidget(btn_normal, row++, 1, 1, 1);

  small_font = new QLabel(tr("Small Text"), this);
  small_font->setObjectName("small_font");
  layout->addWidget(small_font, row, 0, 1, 1, grid_label_align);

  QPushButton *btn_small = setFontButton("small_font");
  layout->addWidget(btn_small, row++, 1, 1, 1);

  QString iatxt = tr("Directory specification for the file attachments");
  QLabel *infoAttachments = new QLabel(iatxt, this);
  layout->addWidget(infoAttachments, row++, 0, 1, 2, Qt::AlignLeft);

  m_attachments = new LineEdit(this);
  m_attachments->setObjectName("attachments");
  m_attachments->setInfo(tr("Attachments"));
  m_attachments->setValue(documentLocation());
  layout->addWidget(m_attachments, row, 0, 1, 1);

  QPushButton *btn_attach = new QPushButton(tr("Open Directory"), this);
  btn_attach->setObjectName("btn_attachments");
  btn_attach->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
  layout->addWidget(btn_attach, row++, 1, 1, 1);

  m_watermark = new LineEdit(this);
  m_watermark->setObjectName("watermark");
  m_watermark->setInfo(tr("Watermark"));
  m_watermark->setValue(documentLocation());
  layout->addWidget(m_watermark, row, 0, 1, 1);

  QPushButton *btn_watermark = new QPushButton(tr("Open File"), this);
  btn_watermark->setObjectName("btn_watermark");
  btn_watermark->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
  layout->addWidget(btn_watermark, row++, 1, 1, 1);

  m_printerGroup = new QGroupBox(this);
  m_printerGroup->setTitle(tr("Settings Default Printers"));
  QGridLayout *prLayout = new QGridLayout(m_printerGroup);

  prLayout->addWidget(new QLabel("DIN A4:", m_printerGroup), 0, 0, 1, 1);
  m_dinA4Printer = new QComboBox(m_printerGroup);
  prLayout->addWidget(m_dinA4Printer, 0, 1, 1, 1);

  prLayout->addWidget(new QLabel("DIN A6:", m_printerGroup), 1, 0, 1, 1);
  m_dinA6Printer = new QComboBox(m_printerGroup);
  prLayout->addWidget(m_dinA6Printer, 1, 1, 1, 1);

  prLayout->setColumnStretch(1, 1);
  m_printerGroup->setLayout(prLayout);
  layout->addWidget(m_printerGroup, row++, 0, 1, 2);

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
  connect(m_signalMapper, SIGNAL(mappedString(const QString &)), this,
          SLOT(openFontDialog(const QString &)));
}

void PrintSettings::setLabelFont(QLabel *lb) {
  bool b = true;
  QFont font = QFontDialog::getFont(&b, lb->font(), this);
  if (b) {
    lb->setFont(font);
    chieldModified(true);
  }
}

void PrintSettings::initPrinterInfos() {
  QListIterator<QPrinterInfo> it(QPrinterInfo::availablePrinters());
  while (it.hasNext()) {
    QPrinterInfo info = it.next();
    if (!info.isNull()) {
      m_dinA4Printer->addItem(info.printerName());
      m_dinA6Printer->addItem(info.printerName());
    }
  }
  int i = 0;
  QString a4p = config->value("printer/DIN_A4_Printer").toString();
  i = m_dinA4Printer->findText(a4p);
  if (i > 0)
    m_dinA4Printer->setCurrentIndex(i);

  QString a6p = config->value("printer/DIN_A6_Printer").toString();
  i = m_dinA6Printer->findText(a6p);
  if (i > 0)
    m_dinA6Printer->setCurrentIndex(i);
}

QPushButton *PrintSettings::setFontButton(const QString &objName) {
  QIcon icon = style()->standardIcon(QStyle::SP_DirIcon);
  QPushButton *btn = new QPushButton(tr("set Font"), this);
  btn->setObjectName("btn_" + objName);
  btn->setIcon(icon);
  connect(btn, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
  m_signalMapper->setMapping(btn, objName);
  return btn;
}

void PrintSettings::openFontDialog(const QString &objName) {
  QLabel *l = findChild<QLabel *>(objName, Qt::FindDirectChildrenOnly);
  if (l != nullptr) {
    setLabelFont(l);
  }
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
  initPrinterInfos();
  config->beginGroup("printer");
  QList<QLabel *> labels =
      findChildren<QLabel *>(p_fontPattern, Qt::FindDirectChildrenOnly);
  if (labels.count() > 1) {
    for (int i = 0; i < labels.count(); i++) {
      QLabel *w = labels.at(i);
      if (w != nullptr) {
        QString name(w->objectName());
        if (name.isEmpty() || !name.contains("font"))
          continue;

        QFont font;
        font.fromString(config->value(name, qApp->font()).toString());
        w->setFont(font);
      }
    }
  }
  m_attachments->setValue(config->value("attachments", documentLocation()));
  m_watermark->setValue(config->value("watermark"));
  config->endGroup();
  config->beginGroup("qrcode");
  foreach (QString key, config->allKeys()) {
    LineEdit *e = qrcodeGroup->findChild<LineEdit *>("qrcode_" + key);
    if (e != nullptr) {
      e->setValue(config->value(key));
    }
  }
  config->endGroup();
}

void PrintSettings::saveSectionConfig() {
  config->beginGroup("printer");
  QList<QLabel *> labels =
      findChildren<QLabel *>(p_fontPattern, Qt::FindDirectChildrenOnly);
  if (labels.count() > 1) {
    for (int i = 0; i < labels.count(); i++) {
      QLabel *w = labels.at(i);
      if (w != nullptr) {
        if (w->objectName().isEmpty())
          continue;

        config->setValue(w->objectName(), w->font().toString());
      }
    }
  }
  config->setValue("watermark", m_watermark->value());
  config->setValue("attachments", m_attachments->value());
  config->setValue("DIN_A4_Printer", m_dinA4Printer->currentText());
  config->setValue("DIN_A6_Printer", m_dinA6Printer->currentText());
  config->endGroup();

  config->beginGroup("qrcode");
  QList<LineEdit *> ledits = qrcodeGroup->findChildren<LineEdit *>(QString());
  if (ledits.count() > 1) {
    for (int i = 0; i < ledits.count(); i++) {
      LineEdit *e = ledits.at(i);
      if (e != nullptr) {
        QStringList section(e->objectName().split("_"));
        config->setValue(section.last(), e->value());
      }
    }
  }
  config->endGroup();
}
