// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagelistfound.h"
#include "sourceinfo.h"

#include <AntiquaCRM>
#include <QHeaderView>
#include <QItemSelectionModel>

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
  p_archivUrn = QString("Archive://");

  QStringList _headers({tr("Date"), tr("Article"), tr("File")});
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

void ImageListFound::highlightRow(int row) {
  if (row < 0)
    return;

  QItemSelectionModel *m_sm = selectionModel();
  if (m_sm == nullptr)
    return;

  blockSignals(true);
  for (int c = 0; c < columnCount(); c++) {
    m_sm->setCurrentIndex(indexFromItem(item(row, c)),
                          QItemSelectionModel::Select);
  }
  blockSignals(false);
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

  QTableWidgetItem *_path = new QTableWidgetItem(QTableWidgetItem::UserType);
  QString _urn = src.filePath().replace(p_archivPath, p_archivUrn);
  _path->setData(Qt::UserRole, src.filePath());
  _path->setData(Qt::DisplayRole, _urn);
  setItem(_r, 2, _path);
}

void ImageListFound::setSourceInfos(const QList<SourceInfo> &list) {
  QListIterator<SourceInfo> it(list);
  while (it.hasNext()) {
    addSourceInfo(it.next());
  }
}

void ImageListFound::setSelected(const SourceInfo &src) {
  clearSelection();
  for (int _r = 0; _r < rowCount(); _r++) {
    QFileInfo _source(item(_r, 2)->data(Qt::UserRole).toString());
    if (src.filePath() == _source.filePath()) {
      highlightRow(_r);
      break;
    }
  }
}
