// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_IMAGEVIEWER_H
#define ANTIQUACRM_WIDGETS_IMAGEVIEWER_H

#include <AntiquaInput>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QPaintEvent>
#include <QPen>
#include <QPixmapCache>
#include <QRubberBand>
#include <QStyleOptionRubberBand>
#include <QWidget>

namespace AntiquaCRM {

class RubberBand;

/**
 * @class ImageViewer
 * @brief Source Image Edit Viewer
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ImageViewer final : public QGraphicsView {
  Q_OBJECT

private:
  /**
   * @brief Maximum Areasize
   */
  QSize p_maxSize;

  /**
   * @brief Start point for create Rubberband region
   */
  QPoint p_startPoint;
  QRect p_rubberRect;

  /**
   * @brief Original Pixmap
   * This will only done when use setPixmap or setImage
   */
  QPixmapCache p_origin;

  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixItem;
  RubberBand *m_rubberband;

  bool setPixmapItem(const QPixmap &pixmap = QPixmap(0, 0));

  void wheelEvent(QWheelEvent *) override;

  void resizeEvent(QResizeEvent *) override;

  void mousePressEvent(QMouseEvent *) override;

  void mouseMoveEvent(QMouseEvent *) override;

Q_SIGNALS:
  /**
   * @brief a add scene successfully or not
   */
  void sendSetSceneView(bool);

public Q_SLOTS:
  /**
   * @brief zoom with factor
   */
  void zoom(qreal);

  /**
   * @brief zoom in with factor 2.0
   */
  void zoomIn();

  /**
   * @brief zoom out with factor 0.5
   */
  void zoomOut();

  /**
   * @brief fit to frame size
   */
  void adjust();

  /**
   * @brief rotate 90° clockwise
   */
  void rotate();

  /**
   * @brief reset and load GraphicsPixmapItem
   */
  void reset();

  /**
   * @brief cut image to current rubberband region
   */
  void cutting();

  /**
   * @brief clear the Scene and reset GraphicsPixmapItem
   */
  void clear();

  /**
   * @brief Add Pixmap and History cache
   */
  void setPixmap(const QPixmap &);

  /**
   * @brief add QImage
   */
  void setImage(const QImage &);

public:
  /**
   * @param parent - parent widget
   */
  explicit ImageViewer(QWidget *parent = nullptr);
  ~ImageViewer();

  /**
   * @brief get max visible Sreen size from current display
   * Used to change the size limit of the preview window.
   * If this cannot be read, fall back to PAL 800x600 Size!
   * @code
   * AntiquaCRM::ASettings cfg(this);
   * QSize thumpSize = cfg.value("image/max_size", QSize(128, 128)).toSize();
   *
   * AntiquaCRM::ImageViewer *viewer = new AntiquaCRM::ImageViewer(this);
   * QSize displaySize = viewer->getMaxScaleSize();
   * int max_width = qMin(thumpSize.width(), displaySize.width());
   * int max_height = qMin(thumpSize.height(), displaySize.height());
   * viewer->setMaximumSize(QSize(max_width, max_height));
   * addWidget(viewer);
   * @endcode
   */
  const QSize getMaxScaleSize() const;

  /**
   * @brief get Current Modified source Pixmap
   */
  const QPixmap getPixmap();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_IMAGEVIEWER_H
