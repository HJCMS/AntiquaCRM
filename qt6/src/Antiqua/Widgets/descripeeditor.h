// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_DESCRIPEEDITOR_H
#define ANTIQUACRM_WIDGETS_DESCRIPEEDITOR_H

#include <AntiquaInput>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>

namespace AntiquaCRM {

class Splitter;

/**
 * @class DescripeEditor
 * @brief The „DescripeEditor“ input edit class.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY DescripeEditor final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  Splitter *m_splitter;
  ATextEdit *m_edit;
  QListWidget *m_list;
  void initData() override;

private Q_SLOTS:
  /**
   * @brief Internal changes when data has changed
   */
  void valueChanged();

  void appendText(QListWidgetItem *);

public Q_SLOTS:
  /**
   * @brief set Value slot
   */
  void setValue(const QVariant &) override;

  /**
   * @brief Set focus to Input Editor
   */
  void setFocus() override;

  /**
   * @brief Reset all Input fields and restore Window Modified
   */
  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit DescripeEditor(QWidget *parent = nullptr);

  /**
   * @brief set data words list
   */
  void setWordsList(const QStringList &);

  /**
   * @brief Restrict input wirt QSqlField parameters.
   * With this function the Input Field can restricted.
   * The QSqlField class contains all Parameters to configure a Input field.
   */
  void setRestrictions(const QSqlField &) override;

  /**
   * @brief Append Tooltip Icon with Description text.
   * If Empty nothing will done.
   */
  void setInputToolTip(const QString &) override;

  /**
   * @brief Prepend a Textlabel with Buddy Focus.
   */
  void setBuddyLabel(const QString &) override;

  /**
   * @brief Validate Input with given Parameters from setRestrictions.
   */
  bool isValid() override;

  /**
   * @brief Current MetaType for this input
   */
  const QMetaType getType() const override;

  /**
   * @brief get current Data ...
   */
  const QVariant getValue() override;

  /**
   * @brief Reserved for QMessageBox poup Windows.
   */
  const QString popUpHints() override;

  /**
   * @brief Reserved for QStatusBar messages.
   */
  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_DESCRIPEEDITOR_H
