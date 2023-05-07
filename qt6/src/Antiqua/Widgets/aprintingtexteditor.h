// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_APRINTINGTEXTEDITOR_H
#define ANTIQUACRM_WIDGETS_APRINTINGTEXTEDITOR_H

#include <AGlobal>
#include <QTextEdit>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY APrintingTextEditor final : public QTextEdit {
  Q_OBJECT

public:
  explicit APrintingTextEditor(QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_APRINTINGTEXTEDITOR_H
