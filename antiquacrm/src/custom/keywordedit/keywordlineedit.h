// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_LINEDIT_H
#define KEYWORD_LINEDIT_H

#include <QCompleter>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStringList>
#include <QWidget>

#include <UtilsMain>

class KeywordLabelList;

/**
 * @brief Texteingabefeld für Kategorie- Schlüsselwörter
 */
class KeywordLineEdit : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

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

  /**
   * @brief Initialisiere Datenbank und setze QCompleter
   */
  void loadKeywords();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // KEYWORDEDIT_LINEDIT_H
