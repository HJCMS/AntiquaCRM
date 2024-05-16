// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_POSTALCODEMODEL_H
#define ANTIQUACRM_WIDGETS_POSTALCODEMODEL_H

#include <AGlobal>
#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QPalette>

namespace AntiquaCRM {

/**
 * @class PostalCodeModel
 * @brief Postalcode Completer Model
 * This model class prevents a network performance issue.
 * The Database file get updated at Application start and has a large size.
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PostalCodeModel final : public QAbstractListModel {
  Q_OBJECT

private:
  QList<AntiquaCRM::PostalCode> p_codes;
  QPalette p_palette;

public:
  /**
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
   * @brief Init Model data
   * It using „postalcodes.json“ File from User Interface shared data location.
   * @note If Json file not exists a SQL fallback will done.
   * @param country - state or country from database
   */
  void initModel(const QString &country);

}; // ANTIQUACRM_WIDGETS_POSTALCODEMODEL_H

} // namespace AntiquaCRM

#endif
