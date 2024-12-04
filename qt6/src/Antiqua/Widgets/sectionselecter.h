// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SECTIONSELECTER_H
#define ANTIQUACRM_WIDGETS_SECTIONSELECTER_H

#include <AntiquaInput>
#include <QAbstractListModel>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM
{

/**
 * @class SectionSelecterModel
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SectionSelecterModel final : public QAbstractListModel {
  Q_OBJECT

private:
  /**
   * @brief Color::palette from Parent Widget
   */
  const QPalette p_palette;

  QVariant typeName(ArticleType) const;

public:
  /**
   * @param parent - a Widget is required to fetch the right Color palette.
   */
  explicit SectionSelecterModel(QWidget* parent = nullptr);

  /**
   * @brief response current row count of countries
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief get country from model index
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
};

/**
 * @class SectionSelecter
 * @brief Select Tab Sections
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SectionSelecter final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AComboBox* m_edit;
  SectionSelecterModel* m_model;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant&) override;

  void setFocus() override;

  void reset() override;

public:
  explicit SectionSelecter(QWidget* parent = nullptr);

  void setRestrictions(const QSqlField&) override;

  void setInputToolTip(const QString&) override;

  void setBuddyLabel(const QString&) override;

  bool isValid() override;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SECTIONSELECTER_H
