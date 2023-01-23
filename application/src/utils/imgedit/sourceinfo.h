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

  /**
   * @brief Optionaler prefix z.B.: media_
   */
  QString p_prefix = QString();

public:
  SourceInfo(const QString &prefix = QString());
  SourceInfo(const QFileInfo &other);
  SourceInfo(const SourceInfo &other);
  SourceInfo &operator=(const SourceInfo &other);

  /**
   * @brief Stimmen alle Parameter
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
   * @brief Artikel Nummern prefix setzen!
   */
  void setPrefix(const QString &prefix);

  /**
   * @brief Artikel Nummern prefix abfragen!
   */
  const QString getPrefix() const;

  /**
   * @brief Neuer Bildname
   * @param id Artikel Nummer
   * @param prefix - Wird bei nicht Buchnummern benötigt!
   * @note Weil die Methode statisch ist muss bei einem Vorhandenen Prefix
   * dieser expliziet gesetzt werden.
   */
  static const QString imageBaseName(qint64 id,
                                     const QString &prefix = QString());

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
