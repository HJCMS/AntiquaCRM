// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_INPUT_COMBOBOX_H
#define ANTIQUA_INPUT_COMBOBOX_H

#include <QAbstractItemView>
#include <QComboBox>
#include <QObject>
#include <QWidget>

/**
 * @brief The AntiquaComboBox class
 * @ingroup widgets
 */
class AntiquaComboBox final : public QComboBox {
  Q_OBJECT
  Q_PROPERTY(bool mouseWheel READ isMouseWheelEnabled WRITE setMouseWheelEnabled
                 NOTIFY sendMouseWheelChanged)

private:
  bool mouseWheel;
  QAbstractItemView *m_view;

protected:
  void wheelEvent(QWheelEvent *e) override;
  void setMouseWheelEnabled(bool);

Q_SIGNALS:
  void sendMouseWheelChanged();

public:
  explicit AntiquaComboBox(QWidget *parent = nullptr);

  /**
   * @brief Standard text für „keine Auswahl“ getroffen!
   */
  const QString withoutDisclosures();

  /**
   * @brief Erstellt einen Eintrage mit Text von withoutDisclosures()
   */
  void setWithoutDisclosures(const QVariant &data = QVariant());

  bool isMouseWheelEnabled();
};

#endif // ANTIQUACRM_COMBOBOX_H
