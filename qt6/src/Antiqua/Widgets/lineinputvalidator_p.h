// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_LINEINPUTVALIDATOR_H
#define ANTIQUACRM_PRIVATE_LINEINPUTVALIDATOR_H

#include <AGlobal>
#include <QStringList>
#include <QValidator>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY LineInputValidator final : public QValidator {
  Q_OBJECT

private:
  const QStringList p_list;

public:
  explicit LineInputValidator(const QStringList &list,
                              QObject *parent = nullptr);

  QValidator::State validate(QString &input, int &pos) const override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_LINEINPUTVALIDATOR_H
