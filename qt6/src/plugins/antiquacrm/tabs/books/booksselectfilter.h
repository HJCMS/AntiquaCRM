// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSSELECTFILTER_H
#define ANTIQUACRM_PLUGIN_BOOKSSELECTFILTER_H

#include <AntiquaTabs>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY BooksSelectFilter final
    : public AntiquaCRM::TabsSearchBarFilter {
  Q_OBJECT

private:
  virtual void createItemData() override;

public:
  explicit BooksSelectFilter(QWidget *parent = nullptr);

  /**
   * @brief Nehme aktuellen Suchfilter
   * @sa getValue(int)
   * @code
   *  QJsonObject(
   *    "search" => QJSonValue().toString()
   *    "fields" => QJSonValue().toString(),
   *  );
   * @endcode
   */
  const QJsonObject getFilter(int index = -1);
};

#endif // ANTIQUACRM_PLUGIN_BOOKSSELECTFILTER_H
