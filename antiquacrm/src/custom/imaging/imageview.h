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

/**
 * @brief Bilder Vorschau
 * @class ImageView
 * @ingroup Imaging
 */
class ImageView final : public QGraphicsView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  const QSize p_max;
  QPixmap p_pixmap;
  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixmap;

  /**
   * @brief Sucht die aktuelle Bildschirmgröße
   * Hiermit wird die Performance erhöht, in dem das Bild beim laden aus den
   * Quellen auf die Maximale Bildschirmauflösung reduziert wird.
   */
  const QSize screenSize() const;

  /**
   * @brief Skalierungs Methode
   */
  void zoomWith(qreal f);

  /**
   * @brief Bild auf Datenbankgröße skalieren.
   * Wird vor dem Speichern aufgerufen und kommt nur zum Einsatz wenn das Bild
   * Größer als @b p_max ist.
   */
  const QImage toDatabaseSize(const QImage &img);

protected:
  /**
   * @brief Mit Mausrad Zoomen
   */
  void wheelEvent(QWheelEvent *event);

  /**
   * @brief Bild auf Rahmen Skalieren
   * @warning Es muss immer ein Pixmap vorhanden sein sonst crasht es hier!
   */
  void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
  /**
   * @brief Wird von den Bildlade Methoden ausgelöst.
   * Folgenden Methoden können bei Erfolg/Fehlschlag auslösen:
   *  @li @ref setImage
   *  @li @ref setImageFile
   *  @li @ref setRawImage
   */
  void s_imageLoadSuccess(bool);

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

  /**
   * @brief Im Uhrzeigersinn um 90° drehen.
   */
  void rotate();

  /**
   * @brief Szene leeren und ein Leeres Pixmap einfügen!
   * @warning resizeEvent();
   * @note Es gibt keine Möglichkeit einen QGraphicsPixmapItem auf Inhalt zu
   * prüfen ohne das es crashen würde. Deshalb gehe ich hin und füge ein leeres
   * QPixmap(0,0) beim start und nach jedem clear() ein.
   * Im resizeEvent() kann ich damit auf m_pixmap->pixmap().isNull() prüfen.
   */
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
