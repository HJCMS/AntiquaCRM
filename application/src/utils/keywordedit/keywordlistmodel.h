// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

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
