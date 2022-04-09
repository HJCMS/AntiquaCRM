// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STRLINEEDIT_H
#define STRLINEEDIT_H

#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>

/**
 @class StrLineEdit
 @brief Standard Zeileneingabe für SQL Tabellenspalten
 @note Die Eingabeprüfung erfolgt mit geringen Aufwand,
 muss vor dem absenden der SQL Anweisung unbedingt geprüft werden.
*/
class StrLineEdit : public QLineEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  /**
    @brief m_completer
    Siehe @ref setLineEditCompliter
  */
  QCompleter *m_completer;

  /**
    @brief m_validator
    Wird im Header initialisiert
    und verwendet @ref pcre
  */
  QRegExpValidator *m_validator;

  /**
    @brief setLineEditCompliter
    Setzt an Hand der Liste die
    Autovervollständigung.
    @note Wird @ref setKeyword ausgelöst!
  */
  void setLineEditCompliter(const QStringList &);

  /**
    @brief Wird mehrfach verwendet
    Das aktuell zulässige Eingabeschema für
    Datenbank Textfelder.
    @warning Zeichen wie "'<> dürfen hier nicht vorkommen!
    @note Wir Escapen wegen der Datenbanksuche nicht!
    @return QRegExp
  */
  static const QRegExp pcre() {
    return QRegExp("([\\w\\d\\s\\-\\+,\\.\\?\\!\\/`´:;&#]+)");
  }

private Q_SLOTS:
  /**
    @brief inputChanged
    Kontrolliert die erlaubte Zeichenlänge
    und gibt bei Überlauf den Text in "Rot" an!
    @param str - Aktuelle Zeicheneingabe
   */
  void inputChanged(const QString &str);

  /**
    @brief checkReturnPressed
    @todo checkInput when Signal::returnPressed() emitted
  */
  void checkReturnPressed();

public:
  explicit StrLineEdit(QWidget *parent = nullptr);

  /**
    @brief setValue
    Die SQL Daten sind vom Type QVariant
    und werden hier für setText vorbereitet!
    Es werden Zeilenumbrüche und überflüssige
    Leerzeichen werden for dem einfügen umgehend
    entfernt!
    @note Der RegExpValidator reagiert nur auf
        aktive Eingaben und nicht auf gesetzte
        Werte die mit setText() erfolgen!
    @param str - SQL Datensatz
   */
  void setValue(const QVariant &str);

  /**
    @brief value
    Gibt den aktuellen Wert als QVariant
    zurück damit er besser Verarbeitet
    werden kann. (SQL|LineEdit) etc.
    @return QVariant::String
  */
  const QVariant value();

  /**
    @brief tableName
    Referenztabelle aus der gelesen wird.
    @return
   */
  static const QString tableName() { return QString("ui_autofill_keywords"); };

  /**
    @brief setKeyword
    Sucht Schlüsselwörter mit dem Parameter
    @i key aus Tabelle @ref tableName().
    Bei Erfolg wird @ref setLineEditCompliter
    aufgerufen.
    @param key - column_name Qt::CaseSensitive
  */
  void setKeyword(const QString &key);

  /**
    @brief setMaxAllowedLength
    Setzt die Maximal erlaubte Zeichsatzlänge
    und erstellt @ref QLineEdit::setPlaceholderText
   */
  void setMaxAllowedLength(int);
};

#endif // STRLINEEDIT_H
