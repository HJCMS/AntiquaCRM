// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configpageswidget.h"

ConfigPagesWidget::ConfigPagesWidget(QWidget *parent)
    : QStackedWidget{parent} {}

bool ConfigPagesWidget::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ModifiedChange) {
    AntiquaCRM::TabsConfigWidget *w = page(obj);
    if (w != nullptr) {
      bool status = w->isWindowModified();
      setWindowModified(status);
      emit sendModified(status);
      return true;
    }
  }
  return QStackedWidget::eventFilter(obj, event);
}

void ConfigPagesWidget::changeEvent(QEvent *event) {
  if (event->type() == QEvent::EnabledChange)
    emit sendEnabledStatus(isEnabled());

  QStackedWidget::changeEvent(event);
}

void ConfigPagesWidget::setPage(int index) {
  AntiquaCRM::TabsConfigWidget *_page = page(index);
  if (_page == nullptr)
    return;

  setCurrentIndex(index);

  QString _t = _page->getMenuEntry().value("title").toString();
  if (!_t.isEmpty())
    emit sendPageTitle(_t);
}

int ConfigPagesWidget::insert(int index, AntiquaCRM::TabsConfigWidget *widget) {
  int _index = insertWidget(index, widget);
  if (_index != -1)
    widget->installEventFilter(this);

  return _index;
}

const QList<AntiquaCRM::TabsConfigWidget *> ConfigPagesWidget::pages() {
  return findChildren<AntiquaCRM::TabsConfigWidget *>(
      QString(), Qt::FindChildrenRecursively);
}

AntiquaCRM::TabsConfigWidget *ConfigPagesWidget::page(int index) {
  AntiquaCRM::TabsConfigWidget *_w = nullptr;
  _w = qobject_cast<AntiquaCRM::TabsConfigWidget *>(widget(index));
  return _w;
}

AntiquaCRM::TabsConfigWidget *ConfigPagesWidget::page(QObject *object) {
  return qobject_cast<AntiquaCRM::TabsConfigWidget *>(object);
}
