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
#include <QResizeEvent>
#include <QSize>
#include <QWheelEvent>
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
  const QSize p_max;
  QPixmap p_pixmap;
  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixmap;

  const QSize screenSize() const;

  void zoomWith(qreal f);

  const QImage toDatabaseSize(const QImage &img);

protected:
  void wheelEvent(QWheelEvent *event);
  void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
  void s_loadSuccess(bool);

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

  void zoomIn();
  void zoomOut();
  void zoomReset();
  void rotate();
  void clear();

public:
  ImageView(QSize maxsize, QWidget *parent = nullptr);

  /**
   * @brief Suche/Lade Bild aus der Datenbank
   * @param articleId
   */
  bool readFromDatabase(int articleId);

  /**
   * @brief In Datenbank Speichern
   * Schreibe Bilddatei in Tabelle "inventory_images"
   */
  bool storeInDatabase(int articleId);

  /**
   * @brief Bild aus Datenbank entfernen!
   */
  bool removeFromDatabase(int articleId);

  /**
   * @brief Ansicht zurückgeben!
   */
  const QImage getImage();

  ~ImageView();
};

#endif // IMAGEVIEW_IMAGING_H
