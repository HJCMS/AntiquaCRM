// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTEUCOUNTRY_H
#define ANTIQUACRM_WIDGETS_SELECTEUCOUNTRY_H

#include <AntiquaInput>
#include <QAbstractListModel>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class EUCountryItem
 * @brief Country Model rowItem
 */
class ANTIQUACRM_LIBRARY EUCountry final {
public:
  int index;        /**< @brief Sort order index */
  QString iso;      /**< @brief ISO 3166-1 Alpha-2 Code */
  QString name;     /**< @brief Translated Country name */

  /**
   * @param ro - Row index
   * @param is - ISO 3166-1 Alpha-2 Code
   * @param na - Country name
   */
  explicit EUCountry(int ro, QString is, QString na);
};

/**
 * @class SelectEUCountryModel
 * @brief European Country Model Class
 * EU Countries initialed with ISO 3166-1 Alpha-2 Code.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SelectEUCountryModel final
    : public QAbstractListModel {
  Q_OBJECT

private:
  /**
   * @brief Color::palette from Parent Widget
   */
  const QPalette p_palette;

  /**
   * @brief European flag icon
   */
  const QIcon euIcon;

  /**
   * @brief Default warn icon
   */
  const QIcon dwIcon;

  /**
   * @brief Country Model list
   */
  QList<EUCountry> p_list;

public:
  /**
   * @param parent - a Widget is required to fetch the right Color palette.
   */
  explicit SelectEUCountryModel(QWidget *parent = nullptr);

  /**
   * @brief response current row count of countries
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief get country from model index
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  /**
   * @brief Initial Countries model
   * @note This must for performance manually done.
   * @code
   *  QComboBox *box = new QComboBox(parent);
   *  SelectEUCountryModel *model = new SelectEUCountryModel(box);
   *  if(model->initModel())
   *    box->setModel(model);
   * @endcode
   */
  bool initModel();

  /**
   * @brief current model size
   */
  int size();
};

/**
 * @class SelectEUCountry
 * @brief ComboBox Selecter for European Countries
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SelectEUCountry final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AComboBox *m_edit;
  SelectEUCountryModel *m_model;
  int noMemberIndex();

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit SelectEUCountry(QWidget *parent = nullptr);
  ~SelectEUCountry();

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SELECTEUCOUNTRY_H
