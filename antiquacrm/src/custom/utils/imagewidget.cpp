// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagewidget.h"
#include "messagebox.h"
#include "sqlcore.h"
#include <AntiquaCRM>

#include <QBuffer>
#include <QDebug>
#include <QImageReader>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVBoxLayout>

ImageWidget::ImageWidget(QWidget *parent) : QWidget{parent} {
  setObjectName("ImageWidgetViewer");
  setMinimumSize(QSize(300, 360));
  setMaximumWidth(300);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("image_viewer_layout");
  layout->setContentsMargins(0, 0, 0, 0);

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

void ImageWidget::insertImage(const QByteArray &data) {
  QImage image = QImage::fromData(data, "jpeg");
  if (image.isNull()) {
    emit s_imageFound(false);
    return;
  } else {
    m_imageLabel->setPixmap(QPixmap::fromImage(image));
    m_imageLabel->update();
    m_scrollArea->setWidget(m_imageLabel);
    emit s_imageFound(true);
  }
}

void ImageWidget::clear() {
  m_imageLabel->clear();
  m_imageLabel->update();
  update();
}

void ImageWidget::searchImageById(int id) {
  QString select("SELECT im_imgdata FROM ");
  select.append("inventory_images WHERE im_id=");
  select.append(QString::number(id));
  select.append(" ORDER BY im_id LIMIT 1;");

  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);

  QByteArray data;
  QSqlQuery q = m_sql->query(select);
  if (q.next()) {
    data = QByteArray::fromBase64(q.value(0).toByteArray(),
                                  QByteArray::Base64Encoding);
    if (data.size() > 60)
      insertImage(data);
  } else {
    emit s_postMessage(tr("no image in database"));
  }
}
