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

class AntiquaTemplates final : public QObject {
  Q_OBJECT

private:
  const QRegularExpression pattern = QRegularExpression("^crm_([a-z_]+)$");

  AntiquaCRM::ASqlCore *m_sql;

  QString tb_message;

  bool tb_attachment = false;

  QMap<QString, QString> p_vars;

  bool queryCompanyData();

  bool queryTemplateBody(const QString &sql);

public:
  explicit AntiquaTemplates(QObject *parent = nullptr);
  const QString getTemplate(const QString &caller,
                            const QString &whereClause);
};

#endif // ANTIQUATEMPLATES_H
