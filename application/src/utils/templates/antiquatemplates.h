// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_ANTIQUATEMPLATES_H
#define TEMPLATES_ANTIQUATEMPLATES_H

#include <AntiquaCRM>
#include <QMap>
#include <QObject>
#include <QRegularExpression>
#include <QVariant>

/**
 * @brief PDF Template locader class
 * @ingroup AntiquaTemplates
 */
class AntiquaTemplates final : public QObject {
  Q_OBJECT

private:
  const QRegularExpression pattern = QRegularExpression("^crm_([a-z_]+)$");
  AntiquaCRM::ASqlCore *m_sql;
  QString tb_message;
  QMap<QString, QString> p_vars;
  bool queryCompanyData();
  bool queryTemplateBody(const QString &sql);

public:
  explicit AntiquaTemplates(QObject *parent = nullptr);

  /**
   * @brief Query Template
   * @note See data/pgsql/select_statement_template_body
   */
  const QString getTemplate(const QString &caller, const QString &whereClause);
};

#endif // TEMPLATES_ANTIQUATEMPLATES_H
