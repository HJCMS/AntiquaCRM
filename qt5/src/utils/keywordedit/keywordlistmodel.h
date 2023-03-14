// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_LISTMODEL_H
#define KEYWORD_LISTMODEL_H

#include <QObject>
#include <QStringListModel>

/**
 * @ingroup _keywordedit
 * @brief User Keywords list model
 */
class KeywordListModel final : public QStringListModel {
  Q_OBJECT

public:
  explicit KeywordListModel(QObject *parent = nullptr);
};

#endif // KEYWORD_LISTMODEL_H
