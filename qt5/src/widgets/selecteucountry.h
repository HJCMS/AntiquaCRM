// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SELECTEUCOUNTRY_H
#define ANTIQUACRM_SELECTEUCOUNTRY_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QIcon>

/**
 * @brief The SelectEuCountry class
 * @ingroup widgets
 */
class SelectEuCountry final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  const QIcon entryIcon(const QString &) const;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;

  /**
   * @note The default input is in BCP 47 uppercase.
   * Complete countrynames or translated name from Class
   * AntiquaCRM::AEuropeanCountries also supported!
   * https://www.rfc-editor.org/info/bcp47
   */
  void setValue(const QVariant &) override;

  void setFocus() override;

public:
  explicit SelectEuCountry(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_SELECTEUCOUNTRY_H
