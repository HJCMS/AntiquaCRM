// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_IMAGE_VIEWER_H
#define ANTIQUACRM_IMAGE_VIEWER_H

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

class ImageRubberBand;

/**
 * @class ImageViewer
 * @brief Source Image Edit Viewer
 * @ingroup ImageWidgets
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
  QPixmapCache p_sourceCache;

  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixItem;
  ImageRubberBand *m_rubberband;

  /**
   * @brief Initial and set PixmapCache limit.
   */
  void initSourceCache(const QSize &, int depth);

  bool setPixmapItem(const QPixmap &pixmap = QPixmap(0, 0));

  void wheelEvent(QWheelEvent *) override;

  void resizeEvent(QResizeEvent *) override;

  void mousePressEvent(QMouseEvent *) override;

  void mouseMoveEvent(QMouseEvent *) override;

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
   * To prevent Buffer Overflows, setPixmap() will always resize with
   * getMaxScaleSize().
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
   * @brief Check for Scenes
   */
  bool isEmpty();

  /**
   * @brief get max visible Sreen size from current display
   * Used to change the size limit of the preview window.
   * If this cannot be read, fall back to PAL 800x600 Size!
   */
  const QSize getMaxScaleSize() const;

  /**
   * @brief get Current Modified source Pixmap
   */
  const QPixmap getPixmap();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_IMAGE_VIEWER_H
