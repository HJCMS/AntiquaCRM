// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SQLINFOPOPUP_H
#define ANTIQUACRM_WIDGETS_SQLINFOPOPUP_H

#include <AGlobal>
#include <QDialog>
#include <QGridLayout>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM
{

/**
 * @class SqlInfoPopUp
 * @brief Database Information Popup dialog
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SqlInfoPopUp final : public QDialog {
  Q_OBJECT

private:
  qint8 row_count = 0;
  QGridLayout* layout;
  QMap<QString, QString> p_trMap;
  const QString translate(const QString& str) const;
  void addRow(const QString& title, const QString& value);

public:
  explicit SqlInfoPopUp(QWidget* parent = nullptr);
  int exec() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SQLINFOPOPUP_H
