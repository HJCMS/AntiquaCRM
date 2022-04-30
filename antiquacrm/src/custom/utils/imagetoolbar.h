// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGETOOLBAR_UTILS_H
#define IMAGETOOLBAR_UTILS_H

#include <QtCore/QObject>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class ImageToolBar : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(int aId READ getArticleId WRITE setArticleId NOTIFY s_articleId)

private:
  int aId = 0;
  QPushButton *m_openbtn;
  QPushButton *m_delbtn;

private Q_SLOTS:
  void checkOpen();
  void checkRemove();

Q_SIGNALS:
  void s_articleId();
  void s_openImage();
  void s_deleteImage(int);

public Q_SLOTS:
  void setArticleId(int);
  void setActive(bool b = false);

public:
  explicit ImageToolBar(QWidget *parent = nullptr);
  inline int getArticleId() { return aId; };
};

#endif // IMAGETOOLBAR_UTILS_H