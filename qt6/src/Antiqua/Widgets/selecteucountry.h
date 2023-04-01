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
#include <QHash>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class AEuropeanCountries
 * @brief European Countries Container
 * EU Countries initialed with ISO 3166-1 Alpha-2 Code.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY AEUCountries final : public QObject,
                                              public QMap<QString, QString> {
  Q_OBJECT

public:
  explicit AEUCountries(QObject *parent = nullptr);

  /**
   * @brief get bcp47Name with translated Country!
   * @param country
   * @return bcp47Name
   */
  const QString bcp47Name(const QString &country);

  /**
   * @brief get Country with bcp47Name!
   * @param bcp47
   * @return Translated Country name
   */
  const QString country(const QString &bcp47);
};

/**
 * @class SelectEUCountry
 * @brief ComboBox Selecter for European Countries
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SelectEUCountry final
    : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AComboBox *m_edit;

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

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SELECTEUCOUNTRY_H
