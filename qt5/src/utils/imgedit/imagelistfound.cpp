// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagelistfound.h"
#include "sourceinfo.h"

#include <AntiquaCRM>
#include <QHeaderView>

ImageListFound::ImageListFound(QWidget *parent) : QTableWidget{parent} {
  setSortingEnabled(false);
  setAlternatingRowColors(true);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setMaximumHeight(100);

  AntiquaCRM::ASettings cfg(this);
  p_archivPath = cfg.value("dirs/images").toString();
  p_archivUrn = tr("Archiv") + "://";

  QStringList _headers({tr("Date"), tr("Article"), tr("Location")});
  setColumnCount(_headers.size());

  QHeaderView *m_header = horizontalHeader();
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_header->setStretchLastSection(true);
  setHorizontalHeader(m_header);

  for (int c = 0; c < _headers.size(); c++) {
    QString _n = _headers.at(c);
    QTableWidgetItem *_i = new QTableWidgetItem(_n, QTableWidgetItem::Type);
    setHorizontalHeaderItem(c, _i);
  }

  connect(this, SIGNAL(itemSelectionChanged()), SLOT(findSelection()));
}

void ImageListFound::findSelection() {
  int _row = currentRow();
  SourceInfo _info(p_archivPath);
  _info.setFileId(item(_row, 1)->data(Qt::UserRole).toInt());
  _info.setFile(item(_row, 2)->text().replace(p_archivUrn, p_archivPath));
  if (_info.isValidSource())
    emit sendSelection(_info);
}

void ImageListFound::addSourceInfo(const SourceInfo &src) {
  int _r = rowCount();
  insertRow(_r);

  QDateTime _dt = src.lastModified();
  setItem(_r, 0, new QTableWidgetItem(_dt.toString(ANTIQUACRM_DATE_DISPLAY)));

  QString _name = AntiquaCRM::AUtil::zerofill(src.getFileId());
  QTableWidgetItem *_id = new QTableWidgetItem(QTableWidgetItem::UserType);
  _id->setData(Qt::UserRole, src.getFileId());
  _id->setData(Qt::DisplayRole, _name);
  setItem(_r, 1, _id);

  QString _path = src.filePath().replace(p_archivPath, p_archivUrn);
  setItem(_r, 2, new QTableWidgetItem(_path));
}

void ImageListFound::setSourceInfos(const QList<SourceInfo> &list) {
  QListIterator<SourceInfo> it(list);
  while (it.hasNext()) {
    addSourceInfo(it.next());
  }
}
