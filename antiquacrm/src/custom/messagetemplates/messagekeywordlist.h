// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGEKEYWORDLIST_UTILS_H
#define MESSAGEKEYWORDLIST_UTILS_H

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class MessageKeywordList final : public QTreeWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QTreeWidgetItem *getSection(const QString &name);
  bool insertTplKey(QTreeWidgetItem *parent, const QString &key,
                    const QJsonObject &value);

public Q_SLOTS:
  void addKey(const QString &section, const QString &key,
              const QJsonObject &value);

public:
  explicit MessageKeywordList(QWidget *parent = nullptr);
};

#endif // MESSAGEKEYWORDLIST_UTILS_H
