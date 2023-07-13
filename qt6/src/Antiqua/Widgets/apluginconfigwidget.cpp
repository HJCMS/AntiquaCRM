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

AntiquaCRM::AInputWidget *PluginConfigWidget::inputWidget(QWidget *parent,
                                                          const QString &name) {
  Q_CHECK_PTR(parent);
  Q_ASSERT_X(name.isEmpty() != true, "name", "objectName is empty");
  return parent->findChild<AntiquaCRM::AInputWidget *>(
      name, Qt::FindDirectChildrenOnly);
}

QList<AntiquaCRM::AInputWidget *>
PluginConfigWidget::getInputList(QObject *parent) {
  Q_CHECK_PTR(parent);
  return parent->findChildren<AntiquaCRM::AInputWidget *>(
      QString(), Qt::FindChildrenRecursively);
}

void PluginConfigWidget::registerInputChangeSignals(QObject *base) {
  Q_CHECK_PTR(base);
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
  if (getType() == ConfigType::CONFIG_DATABASE)
    qWarning("Invalid usage with ConfigType::CONFIG_DATABASE.");

  return p_gid;
}

const QStringList PluginConfigWidget::getCurrentKeys() {
  if (getType() == ConfigType::CONFIG_DATABASE)
    qWarning("Invalid usage with ConfigType::CONFIG_DATABASE.");

  QStringList _keys;
  config->beginGroup(getGroup());
  _keys = config->childKeys();
  config->endGroup();
  return _keys;
}

} // namespace AntiquaCRM
