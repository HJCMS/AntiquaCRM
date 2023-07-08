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
  signalMapper = new QSignalMapper(this);
  connect(signalMapper, SIGNAL(mappedObject(QObject *)),
          SLOT(setInputEditChanged(QObject *)));
}

TabsConfigWidget::~TabsConfigWidget() {
  if (config != nullptr)
    config->deleteLater();

  if (pgsql != nullptr)
    pgsql->deleteLater();

  if (signalMapper != nullptr)
    signalMapper->deleteLater();
}

void TabsConfigWidget::setInputEditChanged(QObject *object) {
  if (signalMapper->mapping(object) != nullptr)
    setWindowModified(true);
}

QList<AntiquaCRM::AInputWidget *>
TabsConfigWidget::getInputList(QObject *parent) {
  Q_CHECK_PTR(parent);
  return parent->findChildren<AntiquaCRM::AInputWidget *>(
      QString(), Qt::FindChildrenRecursively);
}

void TabsConfigWidget::registerInputChangeSignals() {
  QListIterator<AntiquaCRM::AInputWidget *> it(
      findChildren<AntiquaCRM::AInputWidget *>(QString()));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget *inp = it.next();
    connect(inp, SIGNAL(sendInputChanged()), signalMapper, SLOT(map()));
    signalMapper->setMapping(inp, this);
  }
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
