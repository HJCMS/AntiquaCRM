// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_SOURCEINFO_H
#define ANTIQUA_SOURCEINFO_H

#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QString>

/**
 * @brief Erweiterte Datei Info erstellen
 * @ingroup AntiquaImgEdit
 */
class SourceInfo final : public QFileInfo {
private:
  /**
   * @brief Artikel Nummer angeben.
   * Wird Später für das Speichern benötigt!
   */
  qint64 p_fileId;

  /**
   * @brief Wo das Bild gespeichert werden soll!
   */
  QString p_target;

  /**
   * @brief Bildspeicher
   * Wenn das Bild von der Quelle abweicht, wird hier eine Kopie eingefügt.
   */
  QPixmap p_pixmap;

public:
  SourceInfo(const QString &target = QString());
  SourceInfo(const QFileInfo &other);
  SourceInfo(const SourceInfo &other);
  SourceInfo &operator=(const SourceInfo &other);

  /**
   * @brief Stimmen alle Parameter
   */
  bool isValidSource() const;

  /**
   * @brief Ist keine andere Version als das Original verfügbar?
   */
  bool isOriginal();

  /**
   * @brief Artikel Nummer mit Setzen.
   */
  void setFileId(qint64 id);

  /**
   * @brief Artikel Nummer abfragen.
   */
  qint64 getFileId() const;

  /**
   * @brief Neuer Bildname
   * @param id Artikel Nummer
   */
  static const QString imageBaseName(qint64 id);

  /**
   * @brief Neues Bild Ziel definieren!
   */
  void setTarget(const QDir &dest);

  /**
   * @brief Zielinfo
   */
  const QString getTarget() const;

  /**
   * @brief geändertes Bild einfügen
   */
  void setPixmap(const QPixmap &);

  /**
   * @brief Bild nehmen.
   */
  const QPixmap getPixmap();

  /**
   * @brief Bild entfernen!
   */
  void removePixmap();
};

#endif // ANTIQUA_SOURCEINFO_H
