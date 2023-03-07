// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_MESSAGEKEYWORDLIST_H
#define TEMPLATES_MESSAGEKEYWORDLIST_H

#include <QJsonObject>
#include <QList>
#include <QMouseEvent>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

/**
 * @brief The MessageKeywordList class
 * @ingroup AntiquaTemplates
 */
class MessageKeywordList final : public QTreeWidget {
  Q_OBJECT

private:
  QString delimiter = QString("@");
  QTreeWidgetItem *getSection(const QString &section, const QString &type);
  bool insertTplKey(QTreeWidgetItem *parent, const QJsonObject &jsObj);

protected:
  void mousePressEvent(QMouseEvent *event) override;

public Q_SLOTS:
  void addKey(const QString &section, const QJsonObject &jsObj);

public:
  explicit MessageKeywordList(QWidget *parent = nullptr);
};

#endif // TEMPLATES_MESSAGEKEYWORDLIST_H
