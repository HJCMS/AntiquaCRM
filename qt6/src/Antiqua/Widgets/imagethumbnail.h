// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_IMAGETHUMBNAIL_H
#define ANTIQUACRM_WIDGETS_IMAGETHUMBNAIL_H

#include <AntiquaInput>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QObject>
#include <QPixmapCache>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ImageThumbnail
 * @brief Thumbnail Viewer
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ImageThumbnail final : public QGraphicsView {
  Q_OBJECT

private:
  QGraphicsPixmapItem *m_thumbnail;
  QPixmapCache p_pixCache;
  const QSize maxScaleSize() const;
  bool setPixmapItem(const QPixmap &pixmap = QPixmap(0, 0));
  void wheelEvent(QWheelEvent *) override;
  void resizeEvent(QResizeEvent *) override;

private Q_SLOTS:
  void zoom(qreal);
  void zoomIn();
  void zoomOut();

public Q_SLOTS:
  /**
   * @brief fit to frame size
   */
  void adjust();

  /**
   * @brief clear the Scene and reset cache
   */
  void clear();

  /**
   * @brief add QPixmap
   */
  void setPixmap(const QPixmap &);

public:
  /**
   * @param parent - parent widget
   */
  explicit ImageThumbnail(QWidget *parent = nullptr);
  ~ImageThumbnail();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_IMAGETHUMBNAIL_H
