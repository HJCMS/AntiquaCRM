// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGEDIALOG_IMAGING_H
#define IMAGEDIALOG_IMAGING_H

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

class ApplSettings;
class ImageView;

/**
 * @brief Erweiterte Datei Info erstellen
 * @class SourceInfo
 * @ingroup Imaging
 */
class SourceInfo final : public QFileInfo {
private:
  /**
   * @brief Artikel Nummer angeben.
   * Wird Später für das Speichern benötigt!
   */
  int fileId = -1;

public:
  explicit SourceInfo(const QFileInfo &other) : QFileInfo{other} {}

  /**
   * @brief Artikel Nummer mit Setzen.
   */
  void setFileId(int id) { fileId = id; };

  /**
   * @brief Artikel Nummer abfragen.
   */
  int getFileId() { return fileId; };

  /**
   * @brief Neuer Bildname
   * @param id Artikel Nummer
   */
  static const QString imageBaseName(int id) {
    return QString::number(id).rightJustified(8, '0');
  }

  /**
   * @brief Neues Bild Ziel definieren!
   */
  const QString getCopyTarget(const QDir &dest);
};

/**
 * @brief Ableitung von QFileDialog
 * @class FileBrowser
 * @ingroup Imaging
 */
class FileBrowser final : public QFileDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  /**
   * @brief Automatisches schließen unterdrücken!
   * Der Dialog soll sich nicht schließen wenn kein explizites Beenden
   * aufgerufen wird!
   */
  void accept() override;
  void reject() override;

Q_SIGNALS:
  void s_close();
  void s_save();

public:
  explicit FileBrowser(QWidget *parent);
};

/**
 * @brief Primärer Bilddialog
 * @class ImageDialog
 * @defgroup Imaging
 */
class ImageDialog final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Bild Nummer
   */
  const int p_articleId;

  /**
   * @brief Konfigurationen
   */
  ApplSettings *config;

  /**
   * @brief Trenner für Vorschau und Navigation
   */
  QSplitter *m_splitter;

  /**
   * @brief Vorschaubereich
   */
  ImageView *m_view;

  /**
   * @brief Filedialog
   */
  FileBrowser *browser;

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
  inline bool isImageFromArchive(const SourceInfo &info) {
    return info.path().startsWith(imagesArchiv.path());
  }

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
  void fileChanged(const QString &);

protected:
  void closeEvent(QCloseEvent *e) override;

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

#endif // IMAGEDIALOG_IMAGING_H
