// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMEROVERVIEW_H
#define COSTUMEROVERVIEW_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class DomDocument : public QDomDocument {
public:
  QDomElement div;
  explicit DomDocument(const QString &name);
  QDomElement createElementNode(const QString &nodeName, const QString &data);
  QDomElement createLinkNode(const QString &set, const QString &data);
  QDomElement createAddressNode(const QString &data);
};

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
  DomDocument *dom;

public:
  explicit CostumerOverview(QWidget *parent = nullptr);
  void createDocument(QHash<QString, QString> &);
};

#endif // COSTUMEROVERVIEW_H
