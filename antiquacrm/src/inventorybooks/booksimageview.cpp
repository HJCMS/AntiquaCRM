// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksimageview.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtGui/QPixmap>
#include <QtWidgets/QVBoxLayout>

/* QtSql */
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

BooksImageView::BooksImageView(QWidget *parent) : QWidget{parent} {
  setObjectName("BooksImageViewer");
  setMinimumSize(QSize(300, 450));

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("image_viewer_layout");

  m_scrollArea = new QScrollArea(this);
  m_scrollArea->setObjectName("image_viewer_scrollarea");
  layout->addWidget(m_scrollArea);

  m_imageLabel = new QLabel(m_scrollArea);
  m_imageLabel->setObjectName("image_view_item");
  m_imageLabel->setBackgroundRole(QPalette::Base);
  m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_imageLabel->setScaledContents(true);

  setLayout(layout);
}

void BooksImageView::insertImage(const QByteArray &data) {
  QImage image = QImage::fromData(data, "jpg");
  if (!image.isNull()) {
    m_imageLabel->setPixmap(QPixmap::fromImage(image));
    m_scrollArea->setWidget(m_imageLabel);
  }
}

void BooksImageView::searchImageById(int id) {
  QString select("SELECT im_imgdata FROM ");
  select.append("inventory_images WHERE im_id=");
  select.append(QString::number(id));
  select.append(" ORDER BY im_id LIMIT 1;");

  QByteArray data;
  QSqlDatabase db = QSqlDatabase::database(sqlConnectionName);
  if (db.isValid()) {
    QSqlQuery q = db.exec(select);
    if (q.next()) {
      data = q.value(0).toByteArray();
    }
    db.close();
    if (data.size() > 1024)
      insertImage(data);
  }
}
