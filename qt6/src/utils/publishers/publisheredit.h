// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_PUBLISHEREDIT_H
#define ANTIQUACRM_UTILS_PUBLISHEREDIT_H

#include <AntiquaWidgets>
#include <QWidget>

class PublisherEdit final : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::TextLine *m_publisher;
  AntiquaCRM::TextLine *m_location;

Q_SIGNALS:
  void sendDataChanged();

public Q_SLOTS:
  void setData(const QString &publisher, const QString &location);

public:
  explicit PublisherEdit(QWidget *parent = nullptr);
  const QString getPublisher();
  const QString getLocation();
};

#endif // ANTIQUACRM_UTILS_PUBLISHEREDIT_H
