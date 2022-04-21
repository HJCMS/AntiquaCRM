// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSIMAGEVIEW_H
#define BOOKSIMAGEVIEW_H

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

namespace HJCMS {
class SqlCore;
};

/**
   @brief The ImageWidget class
   Liest mit der Article ID aus Tabelle
   "inventory_images" die Binärdaten
   von Bildern aus.
   @note Es wird nur "jpg" Format Unterstützt!
*/
class ImageWidget : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;

  /**
    @brief m_scrollArea
    Zeigt das Bild Label an ...
   */
  QScrollArea *m_scrollArea;

  /**
    @brief m_imageLabel
    Hier wird das Bild reingeschrieben
   */
  QLabel *m_imageLabel;

  /**
    @brief insertImage
    Esrtellt aus dem Datenbankdaten bytea
    Daten ein Image/Pixmap und fügt es bei
    erfolg in @ref m_imageLabel ein und macht
    es im @ref m_scrollArea sichtbar.
    @param data
   */
  void insertImage(const QByteArray &data);

Q_SIGNALS:
  /**
     @brief Bild gefunden Rückmeldung
  */
  void s_imageFound(bool);

  /**
    @brief Nachricht Ausgeben
  */
  void s_sendMessage(const QString &);

public Q_SLOTS:
  /**
     @brief Anzeige leeren
   */
  void clear();

  /**
    @brief searchImageById
    Sucht mit der ArticleID nach den Bilddaten.
    Werden Daten gefunden:
      @li lese die Daten in ein ByteArary
      @li schliesse die Datenbankverbindung
      @li übergebe die Daten an @ref insertImage
    @param id - Article ID
   */
  void searchImageById(int id);

public:
  explicit ImageWidget(QWidget *parent = nullptr);

  /**
     @brief addImageData
     Schreibe Imagedaten in Tabelle "inventory_images"
     @param img QImage
   */
  void addNewImage(int id, const QImage &img);
};

#endif // BOOKSIMAGEVIEW_H
