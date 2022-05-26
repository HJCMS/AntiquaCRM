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
  config->beginGroup("printer");
  QList<QLabel *> labels =
      findChildren<QLabel *>(QString(), Qt::FindDirectChildrenOnly);
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
}

void PrintSettings::saveSectionConfig() {
  config->beginGroup("printer");
  QList<QLabel *> labels =
      findChildren<QLabel *>(QString(), Qt::FindDirectChildrenOnly);
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
  config->endGroup();
}
