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
#include <QCompleter>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PostalCodeEdit
 * @brief Postalcode selecter and editor
 * - The main Country selector and set Zip Code Editor helper class.
 * - This class only returns the zip code.
 * - It supports Binding for Country and Location classes.
 *
 * @code
 *
 * using namespace AntiquaCRM;
 *
 * QGridLayout *layout = new QGridLayout(parent);
 * PostalCodeEdit *m_plz = new PostalCodeEdit(parent);
 * m_plz->setObjectName("c_postalcode");
 * layout->addWidget(m_plz, 0, 0, 1, 2);
 *
 * PostalCodeState *m_state = new PostalCodeState(parent);
 * m_state->setObjectName("c_country");
 * layout->addWidget(m_state, 1, 0, 1, 1);
 *
 * PostalCodeLocation *m_location = new PostalCodeLocation(parent);
 * m_location->setObjectName("c_location");
 * layout->addWidget(m_location, 1, 1, 1, 1);
 *
 * connect(m_plz, SIGNAL(sendOnLeavePostalEdit(const PostalCode &)),
 *         m_state, SLOT(setCountry(const PostalCode &)));
 *
 * connect(m_plz, SIGNAL(sendOnLeavePostalEdit(const PostalCode &)),
 *         m_location, SLOT(setCompletion(const PostalCode &)));
 *
 * QStringList inputs({"c_postalcode","c_location","c_country"});
 * foreach(QString n, inputs) {
 *  AInputWidget *obj = findChild<AInputWidget *>(n);
 *  if (obj != nullptr)
 *   qDebug() << obj->objectName() << obj->getValue();
 * }
 * @endcode
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PostalCodeEdit final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AComboBox *m_countries;  /**< @brief Country Selecter */
  ALineEdit *m_postalcode; /**< @brief Current Postalcode */
  QCompleter *m_completer; /**< @brief zip code Completer */

  const AntiquaCRM::PostalCode dummyCode() const;

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
   * submission.
   * This function only determines with the current country selection!
   * It does NOT go through all postcode countries!
   * @note Make sure a valid country is selected first, otherwise it doesn't
   * executed!
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
   * @brief set Country for Country-Selector
   * Change Model data to given country from ComboBox.
   * - Find translated Country::String in ComboBox.
   * - A internal process select and update the Zip-Code-Model started.
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
   * @brief Clear Input line and set ComboBox to Index:0.
   */
  void reset() override;

public:
  explicit PostalCodeEdit(QWidget *parent = nullptr);

  void initData() override;

  /**
   * @brief get current selected country
   */
  const QString getCountry();

  /**
   * @brief Get AntiquaCRM::PostalCode by country and postalcode
   * @param plz - postalcode
   */
  const AntiquaCRM::PostalCode getPostalCode(const QString &plz);

  /**
   * @brief get locations list from current zip code
   * It will return QCompleter for location list.
   */
  QCompleter *getCompleter(QLineEdit *editor);

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

#endif // ANTIQUACRM_WIDGETS_POSTALCODEEDIT_H
