// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGEVIEW_IMAGING_H
#define IMAGEVIEW_IMAGING_H

#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QWidget>

namespace HJCMS {
class SqlCore;
};

class ImageView : public QGraphicsView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  QPixmap p_pixmap;
  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixmap;

public Q_SLOTS:
  /**
   * @brief Bild in die Ansicht einfügen!
   */
  void setImage(const QImage &img);

  /**
   * @brief Bild in die Ansicht einfügen!
   * @ref setImage(const QImage &)
   */
  void setImageFile(const QFileInfo &file);

  /**
   * @brief Rohdaten einfügen
   * @ref setImage(const QImage &)
   */
  void setRawImage(const QByteArray &data);

  /**
   * @brief drehen
   */
  void rotate();

public:
  ImageView(QWidget *parent = nullptr);

  /**
   * @brief Suche Bild in Datenbank
   * @param articleId
   */
  bool loadFromDatabase(int articleId);

  /**
   * @brief In Datenbank Speichern
   * Schreibe Imagedaten in Tabelle "inventory_images"
   */
  bool storeImage(int articleId);

  /**
   * @brief Ansicht zurückgeben!
   */
  const QImage getImage();
};

#endif // IMAGEVIEW_IMAGING_H
