// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_POSTALCODEEDIT_H
#define ANTIQUACRM_WIDGETS_POSTALCODEEDIT_H

#include <AntiquaInput>
#include <QAbstractListModel>
#include <QCompleter>
#include <QList>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class PostalCodeModel
 */
class ANTIQUACRM_LIBRARY PostalCodeModel final : public QAbstractListModel {
  Q_OBJECT

private:
  QList<AntiquaCRM::PostalCode> p_codes;

public:
  /**
   * @brief Postalcode Completer Model
   * @param parent
   */
  explicit PostalCodeModel(QObject *parent = nullptr);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  void initModel(const QString &country);
};

/**
 * @ingroup AntiquaWidgets
 * @class PostalCodeEdit
 */
class ANTIQUACRM_LIBRARY PostalCodeEdit final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AComboBox *m_countries;
  ALineEdit *m_postalcode;
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
  void valueChanged(int);

  /**
   * @brief Wenn die Postleitzahleneingabe verlassen wird!
   * Suche in der Autovervollständigung nach Übereinstimmungen und erstelle das
   * Signal @ref sendOnLeavePostalEdit für die Daten übermittlung.
   * Diese funktion ermittelt nur mit der Aktuelle Landauswahl!
   * Sie durchläuft NICHT alle Postleitzahlenländer!
   * @note Wenn kein Land ausgewählt ist, wird sie nicht ausgeführt!
   */
  void setPostalCodeLeave();

Q_SIGNALS:
  /**
   * @brief Sende die Aktuell ermittelten Postleitzahlen treffer!
   * @note Wird nur ausgelöst wenn postalReadyToLeave() positiv ausfällt!
   */
  void sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &);

  void sendResetDependencies();

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

  void setCountry(const QString &);

public:
  /**
   * @brief Postalcode editor
   * @param parent - parent Object
   */
  explicit PostalCodeEdit(QWidget *parent = nullptr);

  virtual void initData() override;

  const QString getCountry();

  virtual void setRestrictions(const QSqlField &) override;

  virtual void setInputToolTip(const QString &) override;

  virtual void setBuddyLabel(const QString &) override;

  virtual bool isValid() override;

  virtual const QVariant getValue() override;

  virtual const QString popUpHints() override;

  virtual const QString statusHints() override;
};

/**
 * @ingroup AntiquaWidgets
 * @class PostalCodeState
 */
class ANTIQUACRM_LIBRARY PostalCodeState final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  ALineEdit *m_edit;

  virtual void initData() override {};

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

  void setPostalCodes(const AntiquaCRM::PostalCode &);

public:
  /**
   * @brief Display Postalcode state
   * @param parent - parent Object
   */
  explicit PostalCodeState(QWidget *parent = nullptr);

  virtual void setRestrictions(const QSqlField &) override;

  virtual void setInputToolTip(const QString &) override;

  virtual void setBuddyLabel(const QString &) override;

  virtual bool isValid() override;

  virtual const QVariant getValue() override;

  virtual const QString popUpHints() override;

  virtual const QString statusHints() override;
};

/**
 * @ingroup AntiquaWidgets
 * @class PostalCodeCountry
 */
class ANTIQUACRM_LIBRARY PostalCodeCountry final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  ALineEdit *m_edit;

  virtual void initData() override {};

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

  void setPostalCodes(const AntiquaCRM::PostalCode &);

public:
  /**
   * @brief Display Postalcode countries
   * @param parent - parent Object
   */
  explicit PostalCodeCountry(QWidget *parent = nullptr);

  virtual void setRestrictions(const QSqlField &) override;

  virtual void setInputToolTip(const QString &) override;

  virtual void setBuddyLabel(const QString &) override;

  virtual bool isValid() override;

  virtual const QVariant getValue() override;

  virtual const QString popUpHints() override;

  virtual const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_POSTALCODEEDIT_H
