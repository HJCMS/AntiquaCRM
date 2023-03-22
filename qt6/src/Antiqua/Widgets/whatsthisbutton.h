// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_WHATSTHISBUTTON_H
#define ANTIQUACRM_WIDGETS_WHATSTHISBUTTON_H

#include <AGlobal>
#include <QEvent>
#include <QObject>
#include <QToolButton>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY WhatsThisButton final : public QToolButton {
  Q_OBJECT

private:
  const QString p_text;

private Q_SLOTS:
  void showWhatsThis();

protected:
  virtual bool event(QEvent *e) override;

public:
  explicit WhatsThisButton(const QString &help, QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_WHATSTHISBUTTON_H