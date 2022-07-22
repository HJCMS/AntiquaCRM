// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_LINEDIT_H
#define KEYWORD_LINEDIT_H

#include <QCompleter>
#include <QLineEdit>
#include <QObject>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStringList>
#include <QWidget>

#include <UtilsMain>

/**
 * @brief Texteingabefeld für Kategorie- Schlüsselwörter
 */
class KeywordLineEdit : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Aktueller Schlüsselwort Inhalt
   */
  QString p_keywords;

  /**
   * @brief Trennzeichen
   */
  const QChar p_delimiter = QChar(',');

  /**
   * @brief Autovervolständigung erst ab Mindestlänge starten
   */
  const int minLength = 3;

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
  const QRegExp validatePattern = QRegExp("([\\w\\d\\,\\s]+)");

  /**
   * @brief Regulärer Ausdruck zum bereinigen
   */
  const QRegExp stripPattern = QRegExp("([\\s]+)");

private Q_SLOTS:
  /**
   * @brief Text vom Completer Signal aufarbeiten
   * Weil Completer setText verwendet, an dieser Stelle mit dem Puffer von @ref
   * p_keywords das ganze wieder zusammenfügen.
   */
  void finalize();

public Q_SLOTS:
  /**
   * @brief Weiterleitung von QLineEdit::textChanged
   * Wird verwendet um die aktuelle Eingabe mit Trennzeichen @ref p_delimiter in
   * den Puffer @ref p_keywords zu schreiben. Dieser wird Später bei @ref
   * completerText benötigt. Er soll verhindern, das bestehende Schlüsselwörter
   * Überschrieben werden.
   * Die Methode trennt die Zeichkette von Start bis letztes Trennzeichen.
   * Alle was danach kommt wird erneut mit dem QCompleter Signal highlighted
   * hervorgehoben.
   */
  void keywordChanged(const QString &);

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
