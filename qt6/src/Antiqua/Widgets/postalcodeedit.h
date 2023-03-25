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
   * @note Make sure valid a country is selected first, otherwise it will not run!
   */
  void setPostalCodeLeave();

Q_SIGNALS:
  /**
   * @brief Send the currently determined postcode hits!
   * @note Is only triggered if postalReadyToLeave() is positive!
   */
  void sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &);

  /**
   * @brief Send reset dependencies when state has changed
   */
  void sendResetDependencies();

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

  void setCountry(const QString &);

public:
  /**
   * @brief Postalcode selecter and editor
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

  virtual void initData() override{};

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

  void setPostalCodes(const AntiquaCRM::PostalCode &);

public:
  /**
   * @brief Display Postalcode country/state
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
 * @class PostalCodeLocation
 */
class ANTIQUACRM_LIBRARY PostalCodeLocation final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  ALineEdit *m_edit;

  virtual void initData() override{};

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

  void setPostalCodes(const AntiquaCRM::PostalCode &);

public:
  /**
   * @brief Display Postalcode locations
   * @param parent - parent Object
   */
  explicit PostalCodeLocation(QWidget *parent = nullptr);

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
