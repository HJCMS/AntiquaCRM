// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstabconfigwidget.h"
#include "booksconfig.h"

#include <QDebug>

BooksTabConfigWidget::BooksTabConfigWidget(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"tabs", BOOKS_INTERFACE_NAME, parent} {
}

void BooksTabConfigWidget::loadSectionConfig() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void BooksTabConfigWidget::saveSectionConfig() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

AntiquaCRM::TabsConfigWidget::ConfigType BooksTabConfigWidget::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_DATABASE;
}

const QIcon BooksTabConfigWidget::getIcon() const {
  return AntiquaCRM::AntiquaApplIcon("view-log");
}

const QString BooksTabConfigWidget::getTitle() const { return tr("Books"); }
