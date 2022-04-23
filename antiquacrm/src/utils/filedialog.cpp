// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "filedialog.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QMimeData>
#include <QtCore/QMimeDatabase>
#include <QtCore/QStandardPaths>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtGui/QImageReader>
#include <QtWidgets/QDialogButtonBox>

FileDialog::FileDialog(QWidget *parent)
    : QFileDialog{parent, Qt::Dialog}, p_dir(QDir::home()) {
  setObjectName("FileDialog");
  setWindowTitle(tr("Open Files"));
  setDirectory(p_dir);
  setOptions(QFileDialog::ReadOnly);
  setViewMode(QFileDialog::List);
  setAcceptMode(QFileDialog::AcceptOpen);
  setFileMode(QFileDialog::ExistingFile);
  setMinimumSize(QSize(650, 450));
  setShortcutEnabled(false);

  setLabelText(QFileDialog::LookIn, tr("Target"));
  setLabelText(QFileDialog::FileName, tr("File"));
  setLabelText(QFileDialog::FileType, tr("Types"));
  setLabelText(QFileDialog::Accept, tr("&Open"));
  setLabelText(QFileDialog::Reject, tr("Close"));

  findSidebarUrls();
}

void FileDialog::findSidebarUrls() {
  QList<QUrl> urls;
  QStringList list;
#ifdef Q_OS_WIN
  list << QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
#endif
  list << QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
  list << QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
  list << QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
  list << p_dir.home().path();
#ifdef Q_OS_UNIX
  list << QStandardPaths::standardLocations(QStandardPaths::TempLocation);
  list << QStandardPaths::standardLocations(
      QStandardPaths::GenericCacheLocation);
#endif
  list.removeDuplicates();

  foreach (QString l, list) {
    if (QDir(l).exists())
      urls.append(QUrl::fromLocalFile(l));
  }
  setSidebarUrls(urls);
}

bool FileDialog::checkFile(const QString &file) {
  QMimeType type = QMimeDatabase().mimeTypeForFile(
      QFileInfo(file), QMimeDatabase::MatchExtension);
  if (type.name() == selectedMimeTypeFilter()) {
    setResult(QDialog::Accepted);
    return true;
  }
  qDebug() << Q_FUNC_INFO << "Rejected:" << type.name();
  setResult(QDialog::Rejected);
  return false;
}

void FileDialog::accept() {
  QStringList list(selectedFiles());
  if (!checkFile(list.first())) {
    return;
  }
  QFileDialog::accept();
}

void FileDialog::setImageFilter() {
  QStringList filters("inode/directory");
  filters << "image/jpeg";
  // filters << "image/png";
#ifdef Q_OS_WIN
  filters << "application/x-director";
#endif
  setMimeTypeFilters(filters);
  selectMimeTypeFilter("image/jpeg");
}

void FileDialog::setCertsFilter() {
  QStringList filters("inode/directory");
  filters << "application/pkix-cert";
  filters << "application/x-x509-ca-cert";
#ifdef Q_OS_WIN
  filters << "application/x-director";
#endif
  setMimeTypeFilters(filters);
  selectMimeTypeFilter("application/x-x509-ca-cert");
}

void FileDialog::setPdfFilter() {
  QStringList filters("inode/directory");
  filters << "application/pdf";
#ifdef Q_OS_WIN
  filters << "application/x-director";
#endif
  setMimeTypeFilters(filters);
  selectMimeTypeFilter("application/pdf");
}
