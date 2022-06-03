// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageview.h"
#include "sqlcore.h"

#include <QBuffer>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QImageReader>
#include <QPixmap>

ImageView::ImageView(QWidget *parent) : QGraphicsView(parent) {
  setObjectName("ImagePreview");
  setBackgroundRole(QPalette::Base);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setCacheMode(QGraphicsView::CacheBackground);
  m_sql = new HJCMS::SqlCore(this);
  m_scene = new QGraphicsScene(this);
}

void ImageView::setImage(const QImage &img) {
  p_pixmap = QPixmap::fromImage(img);
  if (!p_pixmap.isNull()) {
    m_scene->clear();
    m_pixmap = m_scene->addPixmap(p_pixmap);
    setScene(m_scene);
  }
}

void ImageView::setImageFile(const QFileInfo &file) {
  QImageReader reader(file.filePath());
  reader.setAutoTransform(true);
  QRect r = rect();
  QImage img = reader.read().scaled(QSize(r.width() - 2, r.height() - 2),
                                    Qt::KeepAspectRatio, /* Ratio */
                                    Qt::FastTransformation);
  setImage(img);
}

void ImageView::setRawImage(const QByteArray &data) {
  QImage image = QImage::fromData(data, "jpeg");
  if (image.isNull())
    return;

  setImage(image);
}

void ImageView::rotate() {
  QImage img(getImage());
  if (img.isNull())
    return;

  QTransform transform;
  transform.rotate(90.0);
  QImage out = img.transformed(transform, Qt::SmoothTransformation);
  if (!out.isNull()) {
    setImage(out);
    update();
  }
}

bool ImageView::loadFromDatabase(int articleId) {
  if (articleId < 1)
    return false;

  QString strArticleId = QString::number(articleId);
  if (strArticleId.isEmpty())
    return false;

  QString select("SELECT im_imgdata FROM ");
  select.append("inventory_images WHERE im_id=");
  select.append(strArticleId);
  select.append(" ORDER BY im_id LIMIT 1;");

  QByteArray data;
  QSqlQuery q = m_sql->query(select);
  if (q.next()) {
    data = QByteArray::fromBase64(q.value(0).toByteArray(),
                                  QByteArray::Base64Encoding);
    if (data.size() > 60)
      setRawImage(data);

    return true;
  }
  return false;
}

bool ImageView::storeImage(int articleId) {
  if (articleId < 1)
    return false;

  QString strArticleId = QString::number(articleId);
  if (strArticleId.isEmpty())
    return false;

  QImage img(getImage());
  QByteArray rawimg;
  QBuffer buffer(&rawimg);
  buffer.open(QIODevice::WriteOnly);
  img.save(&buffer, "jpeg");
  buffer.close();

  QByteArray base64 = rawimg.toBase64();
  QString sql("SELECT im_id FROM inventory_images WHERE im_id=");
  sql.append(strArticleId);
  sql.append(" LIMIT 1;");

  QSqlQuery q = m_sql->query(sql);
  if (q.next()) {
    sql = QString("UPDATE inventory_images SET ");
    sql.append("im_imgdata='");
    sql.append(base64);
    sql.append("' WHERE im_id=");
    sql.append(strArticleId);
    sql.append(";");
  } else {
    sql = QString("INSERT INTO inventory_images (");
    sql.append("im_id,im_imgdata) VALUES (");
    sql.append(strArticleId);
    sql.append(",'");
    sql.append(base64);
    sql.append("');");
  }
  q.clear();
  m_sql->query(sql);
  if (!m_sql->lastError().isEmpty()) {
    qDebug() << m_sql->lastError();
    return false;
  }
  return true;
}

const QImage ImageView::getImage() {
  if (m_pixmap == nullptr)
    return QImage();

  if (m_pixmap->pixmap().isNull())
    return QImage();

  return m_pixmap->pixmap().toImage();
}
