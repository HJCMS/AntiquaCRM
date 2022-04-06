// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STRLINEEDIT_H
#define STRLINEEDIT_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
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
  QCompleter *m_completer;
  QRegExpValidator *m_validator;
  void setLineEditCompliter(const QStringList &);

private Q_SLOTS:
  void inputChanged(const QString &);

public:
  explicit StrLineEdit(QWidget *parent = nullptr);

  /**
     @brief Die SQL Daten sind vom Type QVariant
   */
  void setValue(const QVariant &);
  const QVariant value();

  /**
     @brief Referenztabelle  aus der gelesen wird.
     @return
   */
  static const QString tableName() { return QString("ui_autofill_keywords"); };

  /**
     @brief Schlüsselwort in der Tabelle
     @param column_name Qt::CaseSensitive
  */
  void setKeyword(const QString &);

  /**
     @brief setzt setMaxLength und setPlaceholderText
   */
  void setMaxAllowedLength(int);
};

#endif // STRLINEEDIT_H
