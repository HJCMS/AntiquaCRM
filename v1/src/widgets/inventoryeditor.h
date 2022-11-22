// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORY_EDITOR_H
#define ANTIQUACRM_INVENTORY_EDITOR_H

#include <AntiquaCRM>
#include <AntiquaInputEdit>
#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QRegularExpression>
#include <QSqlField>
#include <QStringList>
#include <QVariant>
#include <QWidget>

/**
 * @brief Hauptklasse für das Editieren
 * @abstract InventoryEditor
 * @ingroup widgets
 */
class InventoryEditor : public QWidget {
  Q_OBJECT

private:
  int timeoutPopUp = 2;

protected:
  /**
   * @brief PostgreSQL Datenbank Verbindung.
   */
  AntiquaCRM::ASqlCore *m_sql;

  /**
   * @brief Konfigurationen/Einstellungen
   */
  AntiquaCRM::ASettings *m_cfg;

  /**
   * @brief Regulärer Ausdruck für "findChild|findChildren"
   * Die Objektnamen der Eingabedatenfelder sollten Identisch mit den
   * Tabellenfeldnamen in @ref inputFields sein!
   * @code
   *  Tabelle "inventory_books" beginnen die Feldenamen immer mit "ib_*"
   *  Daraus ergibt sich folgender Regulärer Ausdruck für die Objektsuche:
   *    ^ib_[a-z_]+\\b$
   * @endcode
   * @note Bei PostgreSQL sind Feldnamen immer in Kleinschrift!
   */
  const QRegularExpression fieldPattern;

  /**
   * @brief Eingabe Felder
   */
  QStringList inputFields;

  /**
   * @brief Alle Eingabefelder in @ref inputFields einfügen.
   * Hier wird aus den Tabellenfeldern die Eingabeliste für @ref inputList
   * erstellt. Dient als Helfer für das erstellen der SQL Statements und der
   * Datenprüfung verwendet wird.
   */
  virtual void setInputFields() = 0;

  /**
   * @brief Sucht nach Tabellen Feldname in @ref inputFields
   */
  bool isInputField(const QString &fieldName);

  /**
   * @brief Suche @ref SerialID Werte mit Objektname.
   */
  qint64 getSerialID(const QString &name);

  /**
   * @brief Nehme Daten von AntiquaWidgets::InputEdit
   */
  const QVariant getDataValue(const QString &objectName);

  /**
   * @brief Get InputEdit class
   * @param objectName
   */
  InputEdit *getInputEdit(const QString &objectName);

  /**
   * @brief Liste der Datenfelder welche in @ref inputFields enthalten sind.
   * Jedoch \b nicht in SQL::{UPDATE|INSERT}s aufgenommen werden sollen!
   */
  QStringList ignoreFields;

  /**
   * @brief Sucht nach Tabellen Feldname in @ref ignoreFields
   */
  bool isIgnoredField(const QString &fieldName);

  /**
   * @brief Diese Container Klasse enthält alle Artikeldaten.
   * Sie werden beim öffnen mit @ref openEditEntry eingelesen.
   * Die Initialisierung erfolgt Typischerweise in @ref setInputFields()
   */
  AntiquaCRM::ASqlDataQuery *m_tableData;

  /**
   * @brief Mit Zeiger auf Objekt-/Feldnamen Datenfeld befüllen.
   * @param field - Ist der SQL Datenfeldbezeichner der Identisch mit dem
   * Eingabe::Objektnamen ist.
   * @param value - Tabellenspalten Wert
   */
  virtual bool setDataField(const QSqlField &field, const QVariant &value) = 0;

  /**
   * @brief Durchläuft @ref AntiquaCRM::ASqlDataQuery
   * Buchdaten auslesen und @ref setDataField aufrufen.
   */
  virtual void importSqlResult() = 0;

  /**
   * @brief Erstelltes SQL Statements an die Datenbank senden.
   */
  virtual bool sendSqlQuery(const QString &query) = 0;

  /**
   * @brief Prüft und erstellt die Datensatzfelder.
   *
   * In dieser Methode werden alle Datenfelder von @class InputEdit abgefragt
   * und bei Erfolg in den Hash geschrieben. Die Erstellung ist von mehreren
   * Faktoren abhängig und beinhaltet folgende Vorgangsweise, welche sich für
   * jedes Datenfeld wiederholt.
   *
   * @li Prüfe Datenfeld Klasse auf @b isRequired() @b isValid()
   * @li Wenn die vorherige Abfrage fehlschlägt dann:
   *  a) MessageBox aufrufen,
   *  b) setFocus auf das Datenfeld,
   *  c) den Hash wieder leeren und abbrechen.
   *
   * Die Leerung muss zur Fehlervermeidung und für die Abfrage Methoden
   * durchgeführt werden.
   */
  virtual const QHash<QString, QVariant> createSqlDataset() = 0;

  /**
   * @brief SQL UPDATE Statement erstellen!
   */
  virtual void createSqlUpdate() = 0;

  /**
   * @brief SQL INSERT Statement erstellen!
   */
  virtual void createSqlInsert() = 0;

  /**
   * @brief Mit Objectnamensliste alle Änderungen zurück setzen!
   */
  void setResetModified(const QStringList &objectList);

  /**
   * @brief Eigenschaften für das Object setzen!
   */
  void setProperties(const QString &objectName, QSqlField &field);

  /**
   * @brief Sucht mit @ref fieldPattern nacht Datenfeldänderungen!
   */
  bool checkIsModified();

  /**
   * @brief Sucht mit @ref fieldPattern nacht Datenfeld und vergleicht Werte.
   * @param name   - Objectname
   * @param origin - Origin (Restore Value)
   * @return true if modified
   */
  bool isModifiedCompare(const QString &name, const QVariant &origin);

protected Q_SLOTS:
  void openErrnoMessage(const QString &info, const QString &error);
  void openSuccessMessage(const QString &info);
  void openNoticeMessage(const QString &info);

  /**
   * @brief Statusnachricht senden.
   */
  void sendStatusMessage(const QString &message);

  /**
   * @brief Article Status an Dienstleister senden.
   */
  void pushPluginOperation(const QJsonObject &obj);

  /**
   * @brief Ruft die reset() Methode Eingabefelder auf!
   * @note Die "reset" Funktion der Eingabefelder verwendet auch
   * setModified(false)!
   */
  void setResetInputFields();

  /**
   * @brief Vorbereitung für die SQL- /Speichern Operationen.
   */
  virtual void setSaveData() = 0;

  /**
   * @brief Methode zum durchführen verschiedener Prüfungen.
   * @note Sollte immer vor setFinalLeaveEditor aufgerufen werden!
   */
  virtual void setCheckLeaveEditor() = 0;

  /**
   * @brief Beenden/Verlassen
   * Vor dem verlassen Aufräumen!
   * Hier kann Abschließend das Signal::sendLeaveEditor gesendet werden.
   */
  virtual void setFinalLeaveEditor() = 0;

Q_SIGNALS:
  /**
   * @brief Sende Artikel Nummer an den Bilder Editor.
   */
  void sendEditImage(qint64 articleId);

  /**
   * @brief Sende Artikel Nummer für Bilder hochladen.
   */
  void sendUploadImage(qint64 articleId);

  /**
   * @brief Editor Beenden
   */
  void sendLeaveEditor();

public Q_SLOTS:
  /**
   * @brief Methode für das Zurücksetzen von Datenfeldern.
   */
  virtual void setRestore() = 0;

public:
  /**
   * @param pattern - Der Reguläre Ausdruck für @ref fieldPattern
   */
  explicit InventoryEditor(const QString &pattern, QWidget *parent = nullptr);

  /**
   * @brief Einen Artikel zum bearbeiten öffnen.
   * @note Hier wird AntiquaCRM::ASqlDataQuery befüllt!
   */
  virtual bool openEditEntry(qint64 articleId) = 0;

  /**
   * @brief Einen Neuen Artikel erstellen.
   */
  virtual bool createNewEntry() = 0;

  virtual ~InventoryEditor();
};

#endif // ANTIQUACRM_INVENTORY_EDITOR_H
