// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_IMAGEDIALOG_H
#define ANTIQUA_IMAGEDIALOG_H

#include <AntiquaCRM>
#include <QAction>
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

class ImageView;
class ImageSelecter;
class SourceInfo;

/**
 * @brief Primärer Bilddialog
 * @class ImageDialog
 * @defgroup Imaging
 */
class ImageDialog final : public QDialog {
  Q_OBJECT

private:
  /**
   * @brief Bild Nummer
   */
  const qint64 p_articleId;

  /**
   * @brief Konfigurationen
   */
  AntiquaCRM::ASettings *config;

  /**
   * @brief Trenner für Vorschau und Navigation
   */
  QSplitter *m_splitter;

  /**
   * @brief Vorschaubereich
   */
  ImageView *m_view;

  /**
   * @brief Ordneransicht
   */
  ImageSelecter *m_imageSelecter;

  /**
   * @brief Bilder Archiv
   */
  QDir imagesArchiv;

  /**
   * @brief Aktionen
   * Im Statusbar ist ein ToolBar eingebettet!
   */
  QStatusBar *m_statusBar;
  QToolBar *m_toolBar;
  QAction *ac_rotate;
  QAction *ac_scale;
  QAction *ac_save;
  QAction *ac_close;

  /**
   * @brief Suche Bildnummer im Archiv
   */
  bool findSourceImage();

  /**
   * @brief Ist es ein Bild vom Archiv?
   * Prüfen ob das ausgewählte Bild aus dem Quellenarchiv stammt oder von einer
   * Externen Quelle eingefügt wurde. Wenn NEIN beim Speichern das Bild in das
   * Quellearchiv kopieren/einfügen.
   * @return bool - „true“ = Ist ein Archivbild.
   */
  inline bool isImageFromArchive(const SourceInfo &info);

  /**
   * @brief Hinweis und Nachfragen ob das Bild Kopiert werden soll.
   */
  bool askToCopyFile();

  /**
   * @brief In die Vorschau einfügen
   */
  bool imagePreview(const SourceInfo &info);

private Q_SLOTS:
  /**
   * @brief Verzeichnisverlauf speichern
   */
  void setHistoryDir(const QDir &);

  /**
   * @brief Bild in Datenbank speichern
   */
  void save();

  /**
   * @brief Ein bild wurde ausgewählt
   */
  void fileChanged(const SourceInfo &image);

protected:
  void closeEvent(QCloseEvent *) override;
  void resizeEvent(QResizeEvent *) override;

public Q_SLOTS:
  /**
   * @brief Statusbar Nachrichten
   */
  void notifyStatus(const QString &);

public:
  /**
   * @param articleId - !!! Muss gesetzt sein. !!!
   */
  explicit ImageDialog(int articleId, QWidget *parent = nullptr);

  /**
   * @brief Dialog aufrufen
   */
  int exec() override;
};

#endif // ANTIQUA_IMAGEDIALOG_H
