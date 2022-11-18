// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUATEMPLATES_H
#define ANTIQUATEMPLATES_H

#include <AntiquaCRM>
#include <QMap>
#include <QObject>
#include <QRegularExpression>
#include <QVariant>

class AntiquaTemplates final : public QObject, public AntiquaCRM::ASqlFiles {
  Q_OBJECT

private:
  const QRegularExpression pattern = QRegularExpression("^crm_([a-z_]+)$");

  QString tb_message;

  bool tb_attachment = false;

  QMap<QString, QString> p_vars;

  void loadDefaults();

  bool queryContent();

public:
  explicit AntiquaTemplates(const QString &name, QObject *parent = nullptr);
  const QString getTemplate();
};

#endif // ANTIQUATEMPLATES_H
