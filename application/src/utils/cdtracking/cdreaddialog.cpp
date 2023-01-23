// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdreaddialog.h"

#include <QDebug>
#include <QLayout>
#include <QUrl>

CDReadDialog::CDReadDialog(QWidget *parent) : QDialog{parent} {
  setWindowTitle(tr("CD title reader"));
  setMinimumSize(500, 450);
  setContentsMargins(2, 2, 2, 2);
  setSizeGripEnabled(true);

  m_cfg = new AntiquaCRM::ASettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new QToolBar(this);
  layout->addWidget(m_toolBar);

  m_hwInfo = new QTextEdit(this);
  m_hwInfo->setReadOnly(true);
  m_hwInfo->setStyleSheet("QTextEdit {border:none; background:transparent;}");
  layout->addWidget(m_hwInfo);

  m_centralWidget = new QWidget(this);
  layout->addWidget(m_centralWidget);

  layout->addStretch(1);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_btnBox, SIGNAL(accepted()), SLOT(accept()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
}

void CDReadDialog::getCDInfo() {
  QString fallback;
#ifdef Q_OS_LINUX
  fallback = QString("/dev/cdrom");
#endif
  QUrl cd_path = m_cfg->value("cdrom_device", fallback).toUrl();
  CDInfo *m_info = new CDInfo(cd_path, this);
  connect(m_info, SIGNAL(finished()), m_info, SLOT(deleteLater()));
  connect(m_info, SIGNAL(sendStatusMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
  connect(m_info, SIGNAL(sendMediaInfo(const HardwareInfo &)),
          SLOT(getMediaInfo(const HardwareInfo &)));

  m_hwInfo->clear();
  m_info->start();
}

void CDReadDialog::getMediaInfo(const HardwareInfo &info) {
  QStringList content(tr("Hardware") + ":");
  content << tr("Vendor: %1").arg(info.vendor.data());
  content << tr("Model: %1").arg(info.model.data());
  content << tr("Revision: %1").arg(info.revision.data());
  m_hwInfo->setPlainText(content.join("\n").trimmed());
}

int CDReadDialog::exec() {
  getCDInfo();
  return QDialog::exec();
}

CDReadDialog::~CDReadDialog() {}
