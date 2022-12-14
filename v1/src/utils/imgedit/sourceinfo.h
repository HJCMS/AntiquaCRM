// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_SOURCEINFO_H
#define ANTIQUA_SOURCEINFO_H

#include <QDir>
#include <QFileInfo>
#include <QString>

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
  qint64 fileId;

  /**
   * @brief Wo das Bild gespeichert werden soll!
   */
  QString p_target;

public:
  SourceInfo();
  SourceInfo(const QFileInfo &other);
  SourceInfo(const SourceInfo &other);
  SourceInfo &operator=(const SourceInfo &other);

  /**
   * @brief Stimme alle Parameter
   */
  bool isValidSource() const;

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
  const QString getFileTarget();
};

#endif // ANTIQUA_SOURCEINFO_H
