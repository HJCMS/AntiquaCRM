// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookswidget.h"
#include "booksconfig.h"

BooksWidget::BooksWidget(QWidget *parent)
    : AntiquaCRM::TabsIndex{"books_tab", parent} {
  setObjectName(BOOKS_SQL_TABLE_NAME);
  setWindowTitle(tr("Books"));
  setWindowIcon(AntiquaCRM::AntiquaApplIcon("view-log"));
}

void BooksWidget::popupWarningTabInEditMode() {
  // TODO
}

void BooksWidget::setDefaultTableView() {
  // TODO
}

void BooksWidget::openStartPage() {
  // TODO
}

void BooksWidget::createSearchQuery(const QString &query) {
  Q_UNUSED(query);
  // TODO
}

void BooksWidget::createNewEntry() {
  // TODO
}

void BooksWidget::openEntry(qint64 articleId) {
  Q_UNUSED(articleId);
  // TODO
}

void BooksWidget::onEnterChanged() {
  // TODO Enter event
}

AntiquaCRM::TabsIndex::ViewPage BooksWidget::currentView() {
  return AntiquaCRM::TabsIndex::MainView;
}

bool BooksWidget::customAction(const QJsonObject &obj) {
  Q_UNUSED(obj);
  return false;
}
