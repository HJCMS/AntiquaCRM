// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configpageswidget.h"

ConfigPagesWidget::ConfigPagesWidget(QWidget *parent)
    : QStackedWidget{parent} {}

bool ConfigPagesWidget::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ModifiedChange) {
    AntiquaCRM::PluginConfigWidget *w = page(obj);
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
  AntiquaCRM::PluginConfigWidget *_page = page(index);
  if (_page == nullptr)
    return;

  setCurrentIndex(index);

  QString _t = _page->getMenuEntry().value("title").toString();
  if (!_t.isEmpty())
    emit sendPageTitle(_t);
}

int ConfigPagesWidget::insert(int index, AntiquaCRM::PluginConfigWidget *widget) {
  int _index = insertWidget(index, widget);
  if (_index != -1)
    widget->installEventFilter(this);

  return _index;
}

const QList<AntiquaCRM::PluginConfigWidget *> ConfigPagesWidget::pages() {
  return findChildren<AntiquaCRM::PluginConfigWidget *>(
      QString(), Qt::FindChildrenRecursively);
}

AntiquaCRM::PluginConfigWidget *ConfigPagesWidget::page(int index) {
  AntiquaCRM::PluginConfigWidget *_w = nullptr;
  _w = qobject_cast<AntiquaCRM::PluginConfigWidget *>(widget(index));
  return _w;
}

AntiquaCRM::PluginConfigWidget *ConfigPagesWidget::page(QObject *object) {
  return qobject_cast<AntiquaCRM::PluginConfigWidget *>(object);
}
