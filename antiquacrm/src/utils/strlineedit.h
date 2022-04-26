// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STRLINEEDIT_UTILS_H
#define STRLINEEDIT_UTILS_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>

#include <SqlCore>
#include <UtilsMain>

namespace HJCMS {
class SqlCore;
};

/**
 @class StrLineEdit
 @brief Standard Zeileneingabe für SQL Tabellenspalten
 @note Die Eingabeprüfung erfolgt mit geringen Aufwand,
 muss vor dem absenden der SQL Anweisung unbedingt geprüft werden.
*/
class StrLineEdit : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Datenbank abfragen
   */
  HJCMS::SqlCore *m_sql;

  /**
   * @brief Tabellenname
   * Die Vervollständigung list aus dieser Tabelle.
   */
  QString p_table;

  /**
   * @brief Eingabefeld
   */
  QLineEdit *m_lineEdit;

  /**
   * @brief Verollständigung
   * Siehe @ref setLineEditCompliter
   */
  QCompleter *m_completer;

  /**
   * @brief Validierer
   * Wird im Header initialisiert und verwendet.
   */
  QRegExpValidator *m_validator;

  /**
   * @brief Initialisiere Vervollständigung
   * Setzt an Hand der Liste die Autovervollständigung.
   * @note Wird @ref setKeyword ausgelöst!
   */
  void setLineEditCompliter(const QStringList &);

  /**
   * @brief Wird mehrfach verwendet
   * Das aktuell zulässige Eingabeschema für Datenbank Textfelder.
   * @warning Zeichen wie "'<> dürfen hier nicht vorkommen!
   * @note Wir Escapen wegen der Datenbanksuche nicht!
   */
  const QString regPattern = QString("([\\w\\d\\s\\-\\+,\\.\\?\\!\\/`´:;&#]+)");

private Q_SLOTS:
  /**
   * @brief inputChanged
   * Kontrolliert die erlaubte Zeichenlänge
   * und gibt bei Überlauf den Text in "Rot" an!
   * @param str - Aktuelle Zeicheneingabe
   */
  void inputChanged(const QString &str);

  /**
   * @brief skipReturnPressed
   * @warning Enter soll das Formular nicht absenden!
   */
  void skipReturnPressed();

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
     @brief Datensatz leeren
  */
  Q_INVOKABLE void reset();

public:
  explicit StrLineEdit(QWidget *parent = nullptr);

  /**
   * @brief Aktueller Rückgabewert
   * Gibt den aktuellen Wert als QVariant zurück
   * damit er besser Verarbeitet werden kann.
   * (SQL|LineEdit) etc.
   */
  const QVariant value();

  /**
   * @brief Tabelle für die Vervollständigung
   * Referenztabelle aus der gelesen wird.
   */
  void setTableName(const QString &table);
  const QString tableName();

  /**
   * @brief Zusatz für Schlüsselwörter
   * @note Wird nur von ib_keyword benötigt!
   */
  void loadStorageKeywords();

  /**
   * @brief loadDataset
   * Sucht Schlüsselwörter mit dem Parameter
   * @i key aus Tabelle @ref tableName().
   * Bei Erfolg wird @ref setLineEditCompliter aufgerufen.
   * @param key - column_name Qt::CaseSensitive
   */
  void loadDataset(const QString &key = QString("condition"));

  /**
   * @brief setMaxAllowedLength
   * Setzt die Maximal erlaubte Zeichsatzlänge und
   * erstellt @ref QLineEdit::setPlaceholderText
   */
  void setMaxAllowedLength(int);

  /**
   * @brief Prüfung ob die Bedingungen erfüllt sind!
   */
  bool isValid();

  /**
   * @brief Nachricht bei Fehlern für das PoUp-Fenster
   */
  const QString notes();
};

#endif // STRLINEEDIT_UTILS_H
