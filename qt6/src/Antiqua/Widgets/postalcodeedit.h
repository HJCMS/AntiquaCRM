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
   * This model class prevents a network performance issue.
   * The Database file get updated at Application start and has a large size.
   * @param parent
   */
  explicit PostalCodeModel(QObject *parent = nullptr);

  /**
   * @brief Rows from current country zip codes.
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const;

  /**
   * @brief return a fixed column count 3
   */
  int columnCount(const QModelIndex &parent) const;

  /**
   * @brief returning sections from AntiquaCRM::PostalCode
   */
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  /**
   * @brief The header data returns always Null in this model!
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;

  /**
   * @brief Init Model data
   * It using „postalcodes.json“ File from User Interface shared data location.
   * @note If Json file not exists a SQL fallback will done.
   * @param country - state or country from database
   */
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
  AComboBox *m_countries;  /**< @brief Country Selecter */
  ALineEdit *m_postalcode; /**< @brief Current Postalcode */
  QCompleter *m_completer; /**< @brief zip code Completer */

  /**
   * @brief Compare postal code with the auto-completion!
   * The zip code database entries are integer values and not contain leading
   * zeros! However, if some buyers indicate zip code with leading zero.
   * We must comparison therefore be made here.
   * See also UNIQUE CONSTRAINT’s in Table „customers“.
   */
  bool comparePostalcode(const QString &source, const QString &input) const;

private Q_SLOTS:
  /**
   * @brief If country selection was triggered.
   * Search and set the autocomplete for country zip codes!
   */
  void valueChanged(int);

  /**
   * @brief If the zip code entry is left!
   * Match the autocomplete and create the sendOnLeavePostalEdit signal for data
   * submission. This function only determines with the current country
   * selection! It does NOT go through all postcode countries!
   * @note Make sure valid a country is selected first, otherwise it will not
   * run!
   */
  void setPostalCodeLeave();

Q_SIGNALS:
  /**
   * @brief Send the currently determined postcode hits!
   * @note Only emitted when „comparePostalCode()“ is positive!
   */
  void sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &);

  /**
   * @brief Send reset dependencies when state has changed
   */
  void sendResetDependencies();

public Q_SLOTS:
  /**
   * @brief Select Country
   * Change Model data to given country from ComboBox.
   * - Find translated Country::String in ComboBox.
   * - If found, a internal process select and update the Zip-Code-Model
   * started.
   * @param country
   */
  void setCountry(const QString &country);

  /**
   * @brief Add current Zip Code to the Inputline.
   * @param value - postalcode from database
   */
  void setValue(const QVariant &value) override;

  /**
   * @brief Focus into the Input line.
   */
  void setFocus() override;

  /**
   * @brief Clear Input line and set ComboBox to main Index.
   */
  void reset() override;

public:
  /**
   * @brief Postalcode selecter and editor
   * @param parent - parent Object
   */
  explicit PostalCodeEdit(QWidget *parent = nullptr);

  void initData() override;

  /**
   * @brief get current selected country
   */
  const QString getCountry();

  /**
   * @brief get locations list from current zip code
   * It will return QCompleter for location list.
   */
  QCompleter *getLocations(QWidget *parent);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
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

  void initData() override{};

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

  void setPostalCodes(const AntiquaCRM::PostalCode &);

public:
  /**
   * @brief Display Postalcode country/state
   * @param parent - parent Object
   */
  explicit PostalCodeState(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

/**
 * @ingroup AntiquaWidgets
 * @class PostalCodeLocation
 */
class ANTIQUACRM_LIBRARY PostalCodeLocation final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  ALineEdit *m_edit;       /**< @brief location */
  QCompleter *m_completer; /**< @brief location Completer */
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

  /**
   * @brief Set Postal codes to get Completer locations
   */
  void setPostalCodes(const AntiquaCRM::PostalCode &);

public:
  /**
   * @brief Display Postalcode locations
   * @param parent - parent Object
   */
  explicit PostalCodeLocation(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_POSTALCODEEDIT_H
