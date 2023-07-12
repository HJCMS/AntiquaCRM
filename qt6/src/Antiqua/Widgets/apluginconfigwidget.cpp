// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "apluginconfigwidget.h"

#include <QDate>

namespace AntiquaCRM {

PluginConfigWidget::PluginConfigWidget(const QString &group, const QString &id,
                                       QWidget *parent)
    : QScrollArea{parent}, p_gid{group + "/" + id} {
  setWidgetResizable(true);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);
  config = new AntiquaCRM::ASettings(this);
  signalMapper = new QSignalMapper(this);
  connect(signalMapper, SIGNAL(mappedObject(QObject *)),
          SLOT(setInputEditChanged(QObject *)));
}

PluginConfigWidget::~PluginConfigWidget() {
  if (config != nullptr)
    config->deleteLater();

  if (pgsql != nullptr)
    pgsql->deleteLater();

  if (signalMapper != nullptr)
    signalMapper->deleteLater();
}

void PluginConfigWidget::setInputEditChanged(QObject *object) {
  if (signalMapper->mapping(object) != nullptr)
    setWindowModified(true);
}

QList<AntiquaCRM::AInputWidget *>
PluginConfigWidget::getInputList(QObject *parent) {
  Q_CHECK_PTR(parent);
  return parent->findChildren<AntiquaCRM::AInputWidget *>(
      QString(), Qt::FindChildrenRecursively);
}

void PluginConfigWidget::registerInputChangeSignals(QObject *base) {
  QListIterator<AntiquaCRM::AInputWidget *> it(
      base->findChildren<AntiquaCRM::AInputWidget *>(QString()));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget *inp = it.next();
    if (inp == nullptr)
      continue;

    // qDebug() << inp->objectName();
    connect(inp, SIGNAL(sendInputChanged()), signalMapper, SLOT(map()));
    signalMapper->setMapping(inp, this);
  }
}

const QString PluginConfigWidget::getGroup() const {
  if (!config->contains(p_gid)) {
    config->beginGroup(p_gid);
    config->setValue("registered", QDate::currentDate().toString(Qt::ISODate));
    config->endGroup();
  }
  return p_gid;
}

const QStringList PluginConfigWidget::getCurrentKeys() {
  QStringList _keys;
  config->beginGroup(getGroup());
  _keys = config->childKeys();
  config->endGroup();
  return _keys;
}

const QMap<QString, QMetaType::Type> PluginConfigWidget::getProviderApiKeys() {
  QMap<QString, QMetaType::Type> _m;
  _m.insert("host", QMetaType::QString);
  _m.insert("key", QMetaType::QString);
  _m.insert("port", QMetaType::Int);
  _m.insert("user", QMetaType::QString);
  return _m;
}

} // namespace AntiquaCRM
