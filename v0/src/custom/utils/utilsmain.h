// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef UTILSMAIN_UTILS_H
#define UTILSMAIN_UTILS_H

#include <QObject>
#include <QVariant>
#include <QFocusEvent>
#include <QFrame>
#include <QWidget>
#include <QMetaType>

/**
 * @class UtilsMain
 * @abstract
 * @ingroup Utils
 * @brief Hauptklasse für Eingabe Klassen
 */
class UtilsMain : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged)
  Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY modifiedChanged)

private:
  bool required = false;
  bool modified = false;

protected:
  QVariant p_value;
  Qt::Alignment labelAlignment();

protected Q_SLOTS:
  virtual void skipReturnPressed();
  virtual void focusOutEvent(QFocusEvent *);

Q_SIGNALS:
  void hasModified(bool); /**< Änderungen aufzeichnen */
  void requireChanged();  /**< Wegen QML eingefügt */
  void modifiedChanged(); /**< Wegen QML eingefügt */
  void editingFinished(); /**< Wird von focusOutEvent abgefangen */

public Q_SLOTS:
  /**
    Wert aus Datenbank einfügen setzen
  */
  virtual void setValue(const QVariant &) = 0;

  /**
   Eingabefeld zurücksetzen
  */
  Q_INVOKABLE virtual void reset() = 0;

  /**
    Setz oder entfernt Modifiziert
  */
  Q_INVOKABLE void setModified(bool b);

  /**
   * @brief Wegen Verschachtelung notwendig!
   */
  virtual void setFocus() = 0;

public:
  explicit UtilsMain(QWidget *parent = nullptr);

  /**
   Setzt das Flag zwingend Norwendig
  */
  void setRequired(bool b);

  /**
     Abfrage ob zwingend Notwendig
  */
  bool isRequired();

  /**
   Datensatz wurde geändert?
  */
  Q_INVOKABLE bool isModified();

  /**
   Aktuellen Wert ausgeben
  */
  virtual const QVariant value() = 0;

  /**
   Datensatz prüfung
  */
  virtual bool isValid() = 0;

  /**
   * Info Label
   */
  virtual void setInfo(const QString &) = 0;

  /**
   * @brief toolTip
   */
  virtual const QString info() = 0;

  /**
   Standard Fehlernachricht
  */
  virtual const QString notes() = 0;
};

typedef QList<UtilsMain *> UtilsMainList;
Q_DECLARE_METATYPE (UtilsMainList)

#endif // UTILSMAIN_UTILS_H
