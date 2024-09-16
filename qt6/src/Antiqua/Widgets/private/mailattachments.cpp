// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailattachments_p.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QLayout>

namespace AntiquaCRM {

AttachmentInfo::AttachmentInfo(QWidget *parent) : QLineEdit{parent} {
  p_tip = tr("Attachment path");
  setToolTip(p_tip);
  setPlaceholderText(tr("No Attachment changed!"));
  setStyleSheet(styleInvalid());
}

const QString AttachmentInfo::styleInvalid() const {
  QString style("QLineEdit {background:transparent;");
  style.append("border:1px solid red;border-radius:2px;}");
  return style;
}

const QString AttachmentInfo::styleValid() const {
  QString style("QLineEdit {background:transparent;");
  style.append("border:1px solid green;border-radius:2px;}");
  return style;
}

void AttachmentInfo::restore() {
  setToolTip(p_tip);
  p_path.clear();
  clear();
}

void AttachmentInfo::setExistsFrame(bool b) {
  setStyleSheet(b ? styleValid() : styleInvalid());
}

void AttachmentInfo::setPath(const QString &path) {
  QFileInfo _info(path);
  if (_info.isReadable()) {
    p_path = _info.filePath();
    setText("... " + p_path.right(30));
    setToolTip(p_tip + ": " + p_path);
  }
}

const QString AttachmentInfo::path() { return p_path; }

MailAttachments::MailAttachments(QWidget *parent) : QFrame{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);

  m_cfg = new AntiquaCRM::ASettings(this);

  m_info = new AttachmentInfo(this);
  layout->addWidget(m_info);

  m_btn = new QPushButton(this);
  m_btn->setText(tr("Attach file"));
  m_btn->setIcon(antiquaIcon("view-search"));
  m_btn->setToolTip(tr("Search file to attach"));
  m_btn->setWhatsThis(
      tr("Open the file dialog and change the attachment path."));
  layout->addWidget(m_btn);

  layout->setStretch(0, 1);
  setLayout(layout);

  connect(m_btn, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

const QDir MailAttachments::invoiceArchive() {
  QDir _d = QDir::home();
  _d.setPath(m_cfg->value("dirs/archive_invoices").toString());
  return _d;
}

void MailAttachments::openFileDialog() {
  QString _file = QFileDialog::getOpenFileName(
      this, tr("Attachment"), invoiceArchive().path(),
      tr("Mail Attachments (*.pdf *.PDF *.txt *.TXT)"), nullptr,
      QFileDialog::HideNameFilterDetails);
  if (!_file.isEmpty()) {
    m_info->setPath(_file);
    m_info->setExistsFrame(true);
  } else {
    m_info->clear();
    m_info->setExistsFrame(false);
  }
}

void MailAttachments::setActive(bool b) { m_btn->setEnabled(b); }

bool MailAttachments::setAttachment(const QString &name) {
  QStringList filter({"*.pdf", "*.PDF"});
  QDirIterator it(invoiceArchive().path(), filter, QDir::NoFilter,
                  QDirIterator::Subdirectories);

  while (it.hasNext()) {
    QFileInfo f(it.next());
    if (f.baseName() == name) {
      qInfo("Mail Attachment (%s)", qPrintable(f.filePath()));
      m_info->setPath(f.filePath());
      break;
    }
  }
  return exists();
}

bool MailAttachments::exists() {
  QFileInfo info(m_info->path());
  bool b = info.exists();
  m_info->setExistsFrame(b);
  return b;
}

const QFileInfo MailAttachments::attachment() {
  QFileInfo info(m_info->path());
  if (info.isReadable())
    return info;

  return QFileInfo();
}

} // namespace AntiquaCRM
