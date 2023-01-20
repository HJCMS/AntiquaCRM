// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_POSTALCODE_EDIT_H
#define ANTIQUACRM_POSTALCODE_EDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QAbstractListModel>
#include <QCompleter>
#include <QLineEdit>

/**
 * @brief Postalcode Completer Model
 * @section widgets
 */
class PostalCodeModel final : public QAbstractListModel {
  Q_OBJECT

private:
  QList<AntiquaCRM::PostalCode> p_codes;

public:
  explicit PostalCodeModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  void initModel(const QString &country);
};

/**
 * @brief Postleitzahlen Eingabe-Editor
 * @section widgets
 */
class PostalCodeEdit final : public InputEdit {
  Q_OBJECT

private:
  /**
   * @brief Auswahl der verfügbaren PLZ Länder.
   */
  AntiquaComboBox *m_countries;

  /**
   * @brief Postleitzahlen Eingabefeld
   */
  QLineEdit *m_postalcode;

  /**
   * @brief Autovervollständigung für das Land
   */
  QCompleter *m_completer;

  /**
   * @brief Eingabe Postleitzahl mit der Autovervollständigung vergleichen!
   * Die PLZ Datenbank enhält bei den Zahlen keine führende "0"!
   * @li Manche Einkäufer geben diese jedoch an.
   * @li Deshalb muss hier ein Vergleich gemacht werden.
   * @warning Wenn die 0 in der Eingabezeile, nach dem Provider Import,
   * nachträglich entfernt wird. Kann dies dazu führen das beim Provider Import
   * die SQL Abfrage missglückt und ein doppelter Eintrag erzeugt wird!
   */
  bool comparePostalcode(const QString &source, const QString &input) const;

private Q_SLOTS:
  /**
   * @brief Wenn Länderauswahl ausgelöst wurde.
   * Suche und setze die Autovervollständigung für das Land!
   */
  void dataChanged(int);

  /**
   * @brief Wenn die Postleitzahleneingabe verlassen wird!
   * Suche in der Autovervollständigung nach Übereinstimmungen und erstelle das
   * Signal @ref sendOnLeavePostalEdit für die Daten übermittlung.
   * Diese funktion ermittelt nur mit der Aktuelle Landauswahl!
   * Sie durchläuft NICHT alle Postleitzahlenländer!
   * @note Wenn kein Land ausgewählt ist, wird sie nicht ausgeführt!
   */
  void postalReadyToLeave();

Q_SIGNALS:
  /**
   * @brief Sende die Aktuell ermittelten Postleitzahlen treffer!
   * @note Wird nur ausgelöst wenn @ref postalReadyToLeave() positiv ausfällt!
   */
  void sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &val);
  void setFocus();
  void findCountry(const QString &name);

public:
  explicit PostalCodeEdit(QWidget *parent = nullptr);
  void loadDataset();
  const QStringList locations(const QString &fromCode);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString country();
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_POSTALCODE_EDIT_H
