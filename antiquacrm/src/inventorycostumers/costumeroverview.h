// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMEROVERVIEW_H
#define COSTUMEROVERVIEW_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

class Document : public QTextDocument {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit Document(QTextEdit *parent = nullptr);
};

class CostumerOverview : public QTextBrowser {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QTextCursor cursor;
  Document *document;
  static bool check(const QString &p, const QString &k, const QVariant &v);
  void setLinkNode(const QString &key, const QVariant &value);
  void setTextNode(const QString &key, const QVariant &value);
  void setAddressNode(const QString &key, const QVariant &value);

public Q_SLOTS:
  void setTextBlock(const QString &key, const QVariant &data);

public:
  explicit CostumerOverview(QWidget *parent = nullptr);
};

#endif // COSTUMEROVERVIEW_H
