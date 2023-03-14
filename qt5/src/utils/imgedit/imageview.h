// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_IMAGEVIEW_H
#define ANTIQUA_IMAGEVIEW_H

#include <AntiquaCRM>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QMouseEvent>
#include <QObject>
#include <QPixmap>
#include <QPixmapCache>
#include <QPointF>
#include <QResizeEvent>
#include <QSize>
#include <QWheelEvent>
#include <QWidget>

#include "sourceinfo.h"

class RubberBand;

/**
 * @ingroup _imgedit
 * @brief Image previewer
 */
class ImageView final : public QGraphicsView {
  Q_OBJECT
  Q_PROPERTY(
      bool modified READ isModified WRITE setModified NOTIFY modifiedChanged)

private:
  bool modified;
  AntiquaCRM::ASqlCore *m_sql;
  const QSize p_max;
  const QByteArray p_format;
  QPixmapCache p_pixmapCache;
  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixmapItem;
  SourceInfo p_currentPreview;
  QPoint p_startPoint;
  RubberBand *m_rubberband;

  /**
   * @brief Pixmap in Scene einfügen!
   * @warning resizeEvent();
   * Es gibt keine Möglichkeit einen QGraphicsPixmapItem auf Inhalt zu prüfen
   * ohne das es crashen würde. Deshalb gehe ich hin und füge ein leeres
   * QPixmap(0,0) im Konstuktor und bei clear() ein. Im resizeEvent() kann ich
   * damit auf m_pixmap->pixmap().isNull() prüfen.
   */
  void setPixmapItem(const QPixmap &pixmap = QPixmap(0, 0));

  /**
   * @brief Maximale Bild Quellengröße
   */
  const QSize maxSourceSize() const;

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

  /**
   * @brief Dimensionen des aktuellen Bildinhalts!
   */
  const QRect sourceRect() const;

protected:
  /**
   * @brief Mit Mausrad Zoomen
   */
  void wheelEvent(QWheelEvent *) override;

  /**
   * @brief Bild auf Rahmen Skalieren
   * @warning Es muss immer ein Pixmap vorhanden sein sonst crasht es hier!
   */
  void resizeEvent(QResizeEvent *) override;

  /**
   * @brief Gummiband aktionen
   */
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

Q_SIGNALS:
  /**
   * @brief Änderungen aufzeichnen
   */
  void hasModified(bool);

  /** Wegen QML eingefügt */
  void modifiedChanged();

  /**
   * @brief Wird von den Bildlade Methoden ausgelöst.
   * Folgenden Methoden können bei Erfolg/Fehlschlag auslösen:
   *  @li setImage
   *  @li setImageFile
   *  @li setRawImage
   */
  void sendImageLoadSuccess(bool);

public Q_SLOTS:
  /**
   * @brief Bild in die Ansicht einfügen!
   */
  void setPixmap(const QPixmap &);
  void setImage(const QImage &);

  /**
   * @brief Bild in die Ansicht einfügen!
   * @code
   *  setImage(const QImage &);
   * @endcode
   */
  void setImageFile(const SourceInfo &file);

  /**
   * @brief Rohdaten einfügen
   * @code
   *  setImage(const QImage &);
   * @endcode
   */
  void setRawImage(const QByteArray &data);

  void zoomIn();
  void zoomOut();
  void adjust();

  /**
   * @brief Im Uhrzeigersinn um 90° drehen.
   */
  void rotate();

  /**
   * @brief Bild aus Rubberband zuscheinden!
   */
  void cutting();

  /**
   * @brief Original neu laden!
   */
  void reset();

  /**
   * @brief Setz oder entfernt Modifizierung
   */
  Q_INVOKABLE void setModified(bool b);

  /**
   * @brief Scene reseten.
   */
  void clear();

public:
  /**
   * @brief ImageView
   * @param maxsize
   * @param parent
   */
  explicit ImageView(QSize maxsize, QWidget *parent = nullptr);

  /**
   * @brief Standard Bild Qualität beim Speichern.
   */
  static inline int quality() { return 90; }

  /**
   * @brief Das gedrehte Bild in das Archiv speichern
   * Es wird gleichzeitig mit screenSize() die Maximal Bildgröße ermittelt.
   * Dies setzt den Größenfaktor für die Archivbilder.
   */
  bool saveImageTo(const SourceInfo &info);

  /**
   * @brief Suche/Lade Bild aus der Datenbank
   * @param articleId
   */
  bool readFromDatabase(qint64 articleId);

  /**
   * @brief In Datenbank Speichern
   * Schreibe Bilddatei in Tabelle "inventory_images"
   */
  bool storeInDatabase(qint64 articleId);

  /**
   * @brief Bild aus Datenbank entfernen!
   */
  bool removeFromDatabase(qint64 articleId);

  /**
   * @brief Ansicht zurückgeben!
   */
  const QImage getImage();

  /**
   * @brief Aktuelle Quelle zurück geben
   */
  const SourceInfo getSource();

  /**
   * @brief Datensatz wurde geändert?
   */
  Q_INVOKABLE bool isModified();

  ~ImageView();
};

#endif // ANTIQUA_IMAGEVIEW_H
