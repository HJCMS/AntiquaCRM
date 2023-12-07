// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "apluginconfigwidget.h"

#include <QDate>
#include <QJsonDocument>
#include <QJsonParseError>

namespace AntiquaCRM {

PluginConfigWidget::PluginConfigWidget(const QString &id, QWidget *parent)
    : QScrollArea{parent}, p_identifier{id} {
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

const QJsonObject PluginConfigWidget::getDatabaseConfig(const QString &key) {
  QJsonObject _errno;
  _errno.insert("status", false);

  if (!key.startsWith("CONFIG_"))
    return _errno;

  if (pgsql == nullptr)
    pgsql = new AntiquaCRM::ASqlCore(this);

  QString _sql("SELECT cfg_jsconfig::json FROM antiquacrm_configs ");
  _sql.append(" WHERE cfg_group='" + key + "';");
  QSqlQuery _q = pgsql->query(_sql);
  if (_q.size() == 1) {
    _q.next();
    QString _buffer = _q.value("cfg_jsconfig").toString();
    if (_buffer.isEmpty()) {
      qWarning("No data for configuration %s", qPrintable(key));
      return _errno;
    }
    QJsonDocument _doc;
    QJsonParseError _parser;
    _doc = QJsonDocument::fromJson(_buffer.toLocal8Bit(), &_parser);
    if (_parser.error != QJsonParseError::NoError) {
      qWarning("Json Document Error: '%s'.", qPrintable(_parser.errorString()));
      return _errno;
    }
    return _doc.object();
  }
  return _errno;
}

bool PluginConfigWidget::saveDatabaseConfig(const QString &key,
                                            const QJsonObject &obj) {
  if (!key.startsWith("CONFIG_") || obj.isEmpty())
    return false;

  if (pgsql == nullptr)
    pgsql = new AntiquaCRM::ASqlCore(this);

  QJsonDocument _doc(obj);
  QString _sql("UPDATE antiquacrm_configs SET cfg_jsconfig='");
  _sql.append(_doc.toJson(QJsonDocument::Compact));
  _sql.append("' WHERE cfg_group='" + key + "';");
  pgsql->query(_sql);
  return pgsql->lastError().isEmpty();
}

const QString PluginConfigWidget::getIdentifier() {
  if (getType() == ConfigType::CONFIG_DATABASE)
    qWarning("Invalid usage with ConfigType::CONFIG_DATABASE.");

  return p_identifier;
}

const QStringList PluginConfigWidget::getCurrentKeys(const QString &path) {
  if (getType() == ConfigType::CONFIG_DATABASE)
    qWarning("Invalid usage with ConfigType::CONFIG_DATABASE.");

  QStringList _keys;
  config->beginGroup(path);
  _keys = config->childKeys();
  config->endGroup();
  return _keys;
}

} // namespace AntiquaCRM
