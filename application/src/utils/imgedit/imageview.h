// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_IMAGEVIEW_H
#define ANTIQUA_IMAGEVIEW_H

#include <AntiquaCRM>
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

#include "sourceinfo.h"

/**
 * @brief Bilder Vorschau
 * @class ImageView
 * @ingroup Imaging
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
  QPixmap p_pixmap;
  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixmap;
  SourceInfo p_currentPreview;

  /**
   * @brief Prefix bei abweichenden Formaten wie z.B.: Media Dateien
   * @note Bücher benötigen keinen Prefix!
   * @example "media_" für CD's oder Vinyl Schallplatten
   */
  const QString p_prefix;

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
  void wheelEvent(QWheelEvent *event) override;

  /**
   * @brief Bild auf Rahmen Skalieren
   * @warning Es muss immer ein Pixmap vorhanden sein sonst crasht es hier!
   */
  void resizeEvent(QResizeEvent *event) override;

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
   *  @li @ref setImage
   *  @li @ref setImageFile
   *  @li @ref setRawImage
   */
  void sendImageLoadSuccess(bool);

public Q_SLOTS:
  /**
   * @brief Bild in die Ansicht einfügen!
   */
  void setImage(const QImage &img);

  /**
   * @brief Bild in die Ansicht einfügen!
   * @ref setImage(const QImage &)
   */
  void setImageFile(const SourceInfo &file);

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
   * @brief Setz oder entfernt Modifizierung
   */
  Q_INVOKABLE void setModified(bool b);

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
  explicit ImageView(QSize maxsize, QWidget *parent = nullptr,
                     const QString &prefix = QString());

  /**
   * @brief Standard Bild Qualität beim Speichern.
   */
  static inline int quality() { return 90; }

  /**
   * @brief Wird für den Bilder Bearbeiten/Importieren Dialog benötigt!
   */
  inline const QString prefix() { return p_prefix; };

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
