// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagetreepathview.h"
#include "antiquaicon.h"
#include "alineedit.h"
#include "antiquaicon.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QLayout>

namespace AntiquaCRM {

ImageTreePathView::ImageTreePathView(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_edit = new ALineEdit(this);
  m_edit->setClearButtonEnabled(false);
  layout->addWidget(m_edit);

  m_ac = new QPushButton(this);
  m_ac->setIcon(antiquaIcon("folder-pictures"));
  layout->addWidget(m_ac);

  setLayout(layout);

  connect(m_ac, SIGNAL(clicked()), SLOT(openDirectoryDialog()));
}

void ImageTreePathView::openDirectoryDialog() {
  QFileDialog *d = new QFileDialog(this, tr("Change Directory"));
  d->setFilter(QDir::AllDirs | QDir::Drives);
  d->setAcceptMode(QFileDialog::AcceptOpen);
  d->setViewMode(QFileDialog::Detail);
  d->setFileMode(QFileDialog::ExistingFile);
  d->setOptions(QFileDialog::ReadOnly);
  d->setNameFilter(tr("JPEG-Image (*.jpg *.jpeg)"));
  d->setDirectory(m_edit->text());
#ifdef Q_OS_WIN
  QList<QUrl> list;
  foreach (QFileInfo i, QDir::drives()) {
    QUrl url;
    url.setPath(i.filePath());
    if (url.isValid())
      list.append(url);
  }
  d->setSidebarUrls(list);
#endif

  if (d->exec() == QDialog::Accepted) {
    QStringList l = d->selectedFiles();
    if (l.size() < 1)
      return;

    QFileInfo info(d->directory(), l.first());
    if (info.isFile() && info.isReadable()) {
      setDirectory(d->directory());
      emit sendSelected(info);
    }
  }
}

void ImageTreePathView::setDirectory(const QDir &dir) {
  m_edit->setText(dir.path());
}

} // namespace AntiquaCRM
