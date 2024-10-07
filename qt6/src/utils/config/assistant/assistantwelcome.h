// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_ASSISTANT_WELCOME_H
#define ANTIQUA_ASSISTANT_WELCOME_H

#include <QLabel>
#include <QObject>
#include <QWidget>

class AssistantWelcome final : public QWidget {
  Q_OBJECT

private:
  QLabel* m_text;

public:
  explicit AssistantWelcome(QWidget* parent = nullptr);
  void setContent(const QString& key);
};

#endif // ANTIQUA_ASSISTANT_WELCOME_H
