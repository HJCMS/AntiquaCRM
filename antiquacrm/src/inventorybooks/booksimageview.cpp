// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksimageview.h"
#include "version.h"

#include <QtCore/QBuffer>
#include <QtCore/QDebug>
#include <QtGui/QImageReader>
#include <QtGui/QPixmap>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtWidgets/QVBoxLayout>

BooksImageView::BooksImageView(QWidget *parent) : QWidget{parent} {
  setObjectName("BooksImageViewer");
  setMinimumSize(QSize(300, 360));
  setMaximumWidth(300);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("image_viewer_layout");

  m_scrollArea = new QScrollArea(this);
  m_scrollArea->setObjectName("image_viewer_scrollarea");
  m_scrollArea->setVisible(true);
  m_scrollArea->setWidgetResizable(false);
  layout->addWidget(m_scrollArea);

  m_imageLabel = new QLabel(m_scrollArea);
  m_imageLabel->setObjectName("image_view_item");
  m_imageLabel->setBackgroundRole(QPalette::Base);
  m_imageLabel->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding);
  m_imageLabel->setScaledContents(false);
  setLayout(layout);
}

void BooksImageView::insertImage(const QByteArray &data) {
  QImage image = QImage::fromData(data, "jpeg");
  if (image.isNull()) {
    // qDebug() << Q_FUNC_INFO << "No valid image data";
    return;
  } else {
    m_imageLabel->setPixmap(QPixmap::fromImage(image));
    m_imageLabel->update();
    m_scrollArea->setWidget(m_imageLabel);
  }
}

void BooksImageView::clear() {
  m_imageLabel->clear();
  m_imageLabel->update();
  update();
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
      // qDebug() << Q_FUNC_INFO << q.record();
      data = q.value(0).toByteArray();
    }
    db.close();

    data = QByteArray::fromBase64(data, QByteArray::Base64Encoding);
    if (data.size() > 1024)
      insertImage(data);
  }
}

void BooksImageView::addNewImage(int id, const QImage &img) {
  if (img.isNull())
    return;

  QByteArray rawimg;
  QBuffer buffer(&rawimg);
  buffer.open(QIODevice::WriteOnly);
  img.save(&buffer, "jpeg");
  buffer.close();
  insertImage(rawimg);

  if (id < 1) {
    qWarning("No Article ID - no image save");
    return;
  }

  QByteArray b64 = rawimg.toBase64();

  QString sql("SELECT im_id FROM inventory_images WHERE im_id=");
  sql.append(QString::number(id));
  sql.append(" LIMIT 1;");

  bool update = false;
  QSqlDatabase db = QSqlDatabase::database(sqlConnectionName);
  if (db.isValid()) {
    QSqlQuery q = db.exec(sql);
    update = q.next();
  }

  if (update) {
    sql = QString("UPDATE inventory_images SET ");
    sql.append("im_imgdata='");
    sql.append(b64);
    sql.append("' WHERE im_id=");
    sql.append(QString::number(id));
    sql.append(";");
  } else {
    sql = QString("INSERT INTO inventory_images (");
    sql.append("im_id,im_imgdata) VALUES (");
    sql.append(QString::number(id));
    sql.append(",'");
    sql.append(b64);
    sql.append("');");
  }

  // qDebug() << Q_FUNC_INFO << sql;
  if (db.isValid()) {
    QSqlQuery q = db.exec(sql);
    if (q.lastError().isValid())
      qDebug() << Q_FUNC_INFO << q.lastError();

    db.close();
  }
}
