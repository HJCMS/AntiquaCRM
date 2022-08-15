// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailattachments.h"
#include "applsettings.h"
#include "myicontheme.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QLayout>

AttachmentInfo::AttachmentInfo(QWidget *parent) : QLabel{parent} {
  p_tip = tr("Attachment path");
  setToolTip(p_tip);
}

void AttachmentInfo::restore() {
  setToolTip(p_tip);
  p_path.clear();
  clear();
}

void AttachmentInfo::setPath(const QString &path) {
  QFileInfo info(path);
  if (info.isReadable()) {
    p_path = path;
    setText("... " + path.right(30));
    setToolTip(p_tip + ": " + path);
  }
}

const QString AttachmentInfo::path() { return p_path; }

MailAttachments::MailAttachments(QWidget *parent) : QFrame{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);

  m_cfg = new ApplSettings(this);

  m_info = new AttachmentInfo(this);
  layout->addWidget(m_info);

  m_btn = new QPushButton(this);
  m_btn->setText(tr("Attachment"));
  m_btn->setIcon(myIcon("folder_green"));
  m_btn->setToolTip(tr("Open Filedialog and change attachment path."));
  layout->addWidget(m_btn);

  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_btn, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

void MailAttachments::openFileDialog() {
  QString path = m_cfg->value("dirs/invoices").toString();
  QFileInfo info(path);
  QString attachFile =
      QFileDialog::getOpenFileName(this, tr("Attachment"), info.filePath(),
                                   tr("Mail Attachments (*.pdf *.PDF *.gs)"),
                                   nullptr, QFileDialog::HideNameFilterDetails);
  if (!attachFile.isEmpty())
    m_info->setPath(attachFile);
}

void MailAttachments::setActive(bool b) { m_btn->setEnabled(b); }

void MailAttachments::setAttachment(const QString &name) {
  QDir path(m_cfg->value("dirs/invoices").toString());
  QStringList filter({"*.pdf", "*.PDF"});
  QDirIterator it(path.path(), filter, QDir::NoFilter,
                  QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QFileInfo f(it.next());
    if (f.baseName() == name) {
      m_info->setPath(f.filePath());
      break;
    }
  }
}

bool MailAttachments::exists() {
  QFileInfo info(m_info->path());
  return info.exists();
}

const QFileInfo MailAttachments::attachment() {
  QFileInfo info(m_info->path());
  if (info.isReadable())
    return info;

  return QFileInfo();
}
