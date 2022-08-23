// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGEKEYWORDLIST_UTILS_H
#define MESSAGEKEYWORDLIST_UTILS_H

#include <QJsonObject>
#include <QList>
#include <QMouseEvent>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class MessageKeywordList final : public QTreeWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

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

#endif // MESSAGEKEYWORDLIST_UTILS_H
