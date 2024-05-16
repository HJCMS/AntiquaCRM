// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_WHATSTHISBUTTON_H
#define ANTIQUACRM_WIDGETS_WHATSTHISBUTTON_H

#include <AGlobal>
#include <QEvent>
#include <QObject>
#include <QPushButton>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class AWhatsThisButton
 * @brief WhatsThis Helper button class
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY AWhatsThisButton final : public QPushButton {
  Q_OBJECT

private:
  const QString p_text;

private Q_SLOTS:
  void showWhatsThis();

protected:
  bool event(QEvent *e) override;

public:
  explicit AWhatsThisButton(const QString &help, QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_WHATSTHISBUTTON_H
