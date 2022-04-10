// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

class Imaging;
class FileDialog;

/**
   @brief The ImageDialog class
*/
class ImageDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  /**
     @brief imageID
     @note Muss gleich mit ArticleID sein!
  */
  qulonglong imageID;

  /**
    @brief sourcePath
  */
  QDir sourceDir;

  /**
     @brief m_toolBar
     Bildberabeitungsleister
   */
  QToolBar *m_toolBar;

  /**
     @brief m_scrollArea
     Scrollbereich
  */
  QScrollArea *m_scrollArea;

  /**
     @brief m_imageWidget
     Bild Darstellung
  */
  Imaging *m_imageLabel;

  /**
     @brief maximumSize
     Wegene Webshops und Datenbank sind
     Bilder auf ein Maximale Größe von
     450x450 Pixeln begrenzt!
  */
  int maximumSize = 450;

  /**
     @brief m_fileDialog
     Bilder öffnen ...
  */
  FileDialog *m_fileDialog;

  /**
     @brief addActions
     ToolBar Operationen
   */
  void addActions();

  /**
     @brief loadFile
     Lade Bildaten und erstelle Imaging
  */
  void loadFile(const QFileInfo &);

private Q_SLOTS:
  /**
     @brief openFileDialog
     Bilder öffnen aufrufen
  */
  void openFileDialog();

Q_SIGNALS:
  /**
   @brief s_imageScaled
   Wird ausgelöst wenn eine Bild Scalierung
   oder Änderung gut/schlecht verlaufen ist.
  */
  void s_imageScaled(bool);

public:
  explicit ImageDialog(qulonglong id, QWidget *parent = nullptr);

  /**
     @brief getImage
     Wenn Bild geladen und sich die Größe
     geändert hat. Kann das Bild hier
     abgefragt werden.
     @warning Das Bild ist Leer wenn es
        nicht gefunden wurde oder keine
        Transformierung statt gefunden
        hat. Signal @ref s_imageScaled
        gibt mehr aufschluss.

     @return QImage()
  */
  const QImage getImage();

  /**
     @brief setSourceTarget
     Wuche im Verzeichnis nach der Bild ID
     @param path
     @return bool
   */
  bool setSourceTarget(const QString &path);

  /**
     @brief sourceTarget
     Gibt den Verzeichnis Pfad des Bildes zurück
     @return QString
   */
  const QString sourceTarget();
};

#endif // IMAGEDIALOG_H
