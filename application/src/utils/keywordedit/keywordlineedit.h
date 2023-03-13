// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_LINEDIT_H
#define KEYWORD_LINEDIT_H

#include <AntiquaInputEdit>
#include <QCompleter>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStringList>
#include <QWidget>

class KeywordLabelList;

/**
 * @ingroup _keywordedit
 * @brief Texteingabefeld für Kategorie- Schlüsselwörter
 */
class KeywordLineEdit final : public InputEdit {
  Q_OBJECT

private:
  /**
   * @brief Trennzeichen
   */
  const QChar p_delimiter = QChar(',');

  /**
   * @brief Autovervolständigung erst ab Mindestlänge starten
   */
  const int minLength = 3;

  /**
   * @brief Schlüüselwortliste
   */
  KeywordLabelList *m_keywordList;

  /**
   * @brief Eingabefeld
   */
  QLineEdit *m_lineEdit;

  /**
   * @brief Zeicheingabe einschränken
   */
  QRegExpValidator *m_validator;

  /**
   * @brief Autovervollständiger
   * @note Wird in @ref loadKeywords Initialisiert!
   */
  QCompleter *m_completer;

  /**
   * @brief Regulärer Ausdruck für die Standardeingabe
   */
  const QRegExp validatePattern = QRegExp("([\\w\\d]+)");

  /**
   * @brief Regulärer Ausdruck zum bereinigen
   */
  const QRegExp stripPattern = QRegExp("([^\\w\\d]+)");

private Q_SLOTS:
  /**
   * @brief Liste leeren
   * Beinhaltet ein Warn PopUp
   */
  void clearKeywords();

  /**
   * @brief Text vom Completer Signal aufarbeiten
   * An dieser Stelle mit dem Puffer von @ref p_keywords das ganze wieder
   * zusammenfügen.
   */
  void finalize();

public Q_SLOTS:
  /**
   * @brief Datenbankwerte in Puffer und Eingabefeld setzen.
   */
  void setValue(const QVariant &str);

  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit KeywordLineEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  void loadDataset();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // KEYWORDEDIT_LINEDIT_H
