// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerqrcode.h"

#include <QGridLayout>

PrinterQRCode::PrinterQRCode(QWidget *parent) : QGroupBox{parent} {
  setTitle(tr("QR Code Settings for Smartphone apps."));

  QGridLayout *layout = new QGridLayout(this);
  int row = 0;
  QString qrInfo = tr("The settings for the connectivity to the company server "
                      "are set in this area. These settings will be embedded "
                      "in the QR image.\nNote: Please keep in mind that QR "
                      "images must be recreated if you change something here.");
  QLabel *info = new QLabel(qrInfo, this);
  info->setIndent(2);
  info->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  info->setWordWrap(true);
  layout->addWidget(info, row++, 0, 1, 2);

  layout->addWidget(label(tr("Url")), row, 0, 1, 1);
  m_url = new AntiquaCRM::ALineEdit(this);
  m_url->setPlaceholderText(tr("https://company-server.com/bookcard/"));
  layout->addWidget(m_url, row++, 1, 1, 1);

  layout->addWidget(label(tr("Query")), row, 0, 1, 1);
  m_query = new AntiquaCRM::ALineEdit(this);
  m_query->setPlaceholderText(tr("article_number"));
  layout->addWidget(m_query, row++, 1, 1, 1);

  setLayout(layout);
}

QLabel *PrinterQRCode::label(const QString &title) {
  QLabel *m_lb = new QLabel(this);
  m_lb->setIndent(2);
  m_lb->setText(title + ":");
  m_lb->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  return m_lb;
}

void PrinterQRCode::loadSection(AntiquaCRM::ASettings *config) {
  m_url->setText(config->value("qrcode_url").toString());
  m_query->setText(config->value("qrcode_query").toString());
}

void PrinterQRCode::saveSection(AntiquaCRM::ASettings *config) {
  config->setValue("qrcode_url", m_url->text());
  config->setValue("qrcode_query", m_query->text());
}
