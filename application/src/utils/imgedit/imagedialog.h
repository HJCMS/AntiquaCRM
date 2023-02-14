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
   * @brief Spezielle Medien wie CD's Vinyl und Diverse
   */
  QString p_category = QString();

  /**
   * @brief Bilder Archiv
   */
  QDir p_archiv;

  /**
   * @brief Speicherpfad
   */
  QDir p_savePath = QDir::temp();

  /**
   * @brief Aktionen
   * Im Statusbar ist ein ToolBar eingebettet!
   */
  QStatusBar *m_statusBar;
  QToolBar *m_toolBar;
  QAction *ac_cut;
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
   * @brief Bild in Datenbank speichern
   */
  void save();

  /**
   * @brief Ein bild wurde ausgewählt
   */
  void fileChanged(const SourceInfo &image);

  /**
   * @brief Einzel Bild Importieren
   * @param image (Pfad)
   */
  void setImageImport(const QString &image);

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
   * @brief Standard Bildarchivpfad zurück geben!
   * @note Liest den Pfad aus den Programmeinstellungen!
   */
  const QDir getDefaultImagePath();

  /**
   * @brief Temporäres Unterverzeichnis setzen
   * @param category (Relative ausgehend vom Standard Archiv Pfad!)
   * @note Wird nicht bei Büchern benötigt!
   * Nur Medien wie CD's oder Vinyl Schallplatten benötigen ein Temporäres
   * Unterverzeichnis. Der Grund hierfür ist, das die meisten Dienstleister nur
   * Bücher anbieten. Deshalb werden für die Server Verarbeitung andere Medien
   * gesondert behandelt! Die Server CronJobs können an der Dateinummer nicht
   * erkennen um welchen Artikel-Typ es sich handelt!
   */
  void setSubCategory(const QString &category);

  /**
   * @brief Speicherpfad neu einlesen
   * Wenn der Speicherpfad nicht innerhalb des Archivpfades liegt!
   * Dann den Speicherpfad durch den Standard Archivbilderpfad ersetzen!
   * @note Ein Speichern außerhalb des Bildarchives wird nicht unterstützt!
   */
  const QDir getSavePath();

  /**
   * @brief Dialog aufrufen
   */
  int exec() override;
};

#endif // ANTIQUA_IMAGEDIALOG_H
