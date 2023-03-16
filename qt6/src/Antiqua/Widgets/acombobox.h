// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_COMBOBOX_H
#define ANTIQUA_WIDGETS_COMBOBOX_H

#include <AGlobal>
#include <AInputEdit>
#include <QComboBox>
#include <QObject>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @brief AntiquaCRM Combobox widget
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY AComboBox final : public QComboBox, public AInputEdit {
  Q_OBJECT

private:
  bool mouse_wheel_support = false;

protected:
  void wheelEvent(QWheelEvent *e) override;

public Q_SLOTS:
  /**
   * @brief set Qt::UserRole value
   */
  void setValue(const QVariant &) override;

public:
  explicit AComboBox(QWidget *parent = nullptr);

  /**
   * @brief Standard text für „keine Auswahl“ getroffen!
   */
  const QString withoutDisclosures();

  /**
   * @brief Erstellt einen Eintrage mit Text von withoutDisclosures()
   */
  void setWithoutDisclosures(const QVariant &data = QVariant());

  /**
   * @brief get Qt::UserRole value
   */
  const QVariant getValue() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_COMBOBOX_H
