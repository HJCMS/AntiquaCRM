// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMEROVERVIEW_H
#define COSTUMEROVERVIEW_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QTextDocument>
#include <QTextBrowser>
#include <QWidget>
#include <QDomDocument>
#include <QDomElement>

#include <AntiquaCRM>

class DomDocument : public QDomDocument {
public:
  QDomElement div;
  explicit DomDocument(const QString &name);
  QDomElement createElementNode(const QString &nodeName, const QString &data);
  QDomElement createLinkNode(const QString &set, const QString &data);
  QDomElement createAddressNode(const QString &data);
};

class CostumerOverview : public QTextBrowser {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QTextDocument *doc;
  DomDocument *dom;
  QHash<QString, QString> items;
  bool check(const QString &key);
  void addLineBreak();
  void createCompanySection();
  void createTitleSection();
  void createPhoneSection();
  void createEMailSection();
  void createAddressSection();
  void createAdditionalSection();

public:
  explicit CostumerOverview(QWidget *parent = nullptr);
  void createDocument(const DataFields &);
};

#endif // COSTUMEROVERVIEW_H
