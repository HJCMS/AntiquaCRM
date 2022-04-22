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
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  /**
    @brief Volatile Variable für Required
  */
  bool required = false;

  /**
    @bug Wenn Completer aktiviert ist stimmt von QLineEdit
      das isModified() nicht. Deshalb setze ich noch zusätzlich
      bei setModified() meine eigene Variable und verwende
      in @ref hasModified() eine Abfrage.
    @code
      qDebug() << objectName() << (isModified() || modified);
    @endcode
  */
  bool modified = false;

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
    @brief skipReturnPressed
    @warning Enter soll das Formular nicht absenden!
  */
  void skipReturnPressed();

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
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
     @brief Auf Modifiziert setzen/entfernen.
  */
  Q_INVOKABLE void setModified(bool b);

  /**
     @brief Datensatz leeren
  */
  Q_INVOKABLE void reset();

public:
  explicit StrLineEdit(QWidget *parent = nullptr);

  /**
     @brief Setze Datensatz auf erforderlich
  */
  void setRequired(bool b);

  /**
    @brief Abfrage für Datensatz erforderlich
  */
  bool isRequired();

  /**
     @brief Wurde der Datensatz geändert?
  */
  Q_INVOKABLE bool hasModified();

  /**
    @brief Aktueller Rückgabewert
    Gibt den aktuellen Wert als QVariant
    zurück damit er besser Verarbeitet
    werden kann. (SQL|LineEdit) etc.
  */
  const QVariant value();

  /**
    @brief tableName
    Referenztabelle aus der gelesen wird.
    @return
   */
  static const QString tableName() { return QString("ui_autofill_keywords"); };

  /**
    @brief loadDataset
    Sucht Schlüsselwörter mit dem Parameter
    @i key aus Tabelle @ref tableName().
    Bei Erfolg wird @ref setLineEditCompliter
    aufgerufen.
    @param key - column_name Qt::CaseSensitive
  */
  void loadDataset(const QString &key = QString("condition"));

  /**
    @brief setMaxAllowedLength
    Setzt die Maximal erlaubte Zeichsatzlänge
    und erstellt @ref QLineEdit::setPlaceholderText
   */
  void setMaxAllowedLength(int);

  /**
     @brief Prüfung ob die Bedingungen erfüllt sind!
   */
  bool isValid();

  /**
     @brief Nachricht bei Fehlern für das PoUp-Fenster
  */
  const QString notes();
};

#endif // STRLINEEDIT_H
