// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsconfigwidget.h"

#include <QBoxLayout>
#include <QDate>

namespace AntiquaCRM {

TabsConfigWidget::TabsConfigWidget(const QString &group, const QString &id,
                                   QWidget *parent)
    : QScrollArea{parent}, p_gid{group + "/" + id} {
  setWidgetResizable(true);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);
  config = new AntiquaCRM::ASettings(this);
}

TabsConfigWidget::~TabsConfigWidget() {
  if (config != nullptr)
    config->deleteLater();

  if (pgsql != nullptr)
    pgsql->deleteLater();
}

const QString TabsConfigWidget::getGroup() const {
  if (!config->contains(p_gid)) {
    config->beginGroup(p_gid);
    config->setValue("registered", QDate::currentDate().toString(Qt::ISODate));
    config->endGroup();
  }
  return p_gid;
}

const QStringList TabsConfigWidget::getCurrentKeys() {
  QStringList _keys;
  config->beginGroup(getGroup());
  _keys = config->childKeys();
  config->endGroup();
  return _keys;
}

const QMap<QString, QMetaType::Type> TabsConfigWidget::getProviderApiKeys() {
  QMap<QString, QMetaType::Type> _m;
  _m.insert("host", QMetaType::QString);
  _m.insert("key", QMetaType::QString);
  _m.insert("port", QMetaType::Int);
  _m.insert("user", QMetaType::QString);
  return _m;
}

} // namespace AntiquaCRM
