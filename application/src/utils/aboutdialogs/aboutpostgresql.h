// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ABOUT_POSTGRESQL_H
#define ANTIQUACRM_ABOUT_POSTGRESQL_H

#include "infowidget.h"
#include <QWidget>

/**
 * @ingroup _aboutdialogs
 * @brief About PostgreSQL InfoWidget
 */
class AboutPostgreSQL : public InfoWidget {
  Q_OBJECT

public:
  explicit AboutPostgreSQL(QWidget *parent = nullptr);
  void setInfoWidget() override;
};

#endif // ANTIQUACRM_ABOUT_POSTGRESQL_H
