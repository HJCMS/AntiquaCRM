// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_IMAGETOOLBAR_H
#define ANTIQUA_IMAGETOOLBAR_H

#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @ingroup _imgedit
 * @brief Imageedit ToolBar
 */
class ImageToolBar final : public QWidget {
  Q_OBJECT
  Q_PROPERTY(qint64 articleId READ getArticleId WRITE setArticleId NOTIFY sendArticleIdChanged)

private:
  qint64 articleId = 0;
  QPushButton *m_openbtn;
  QPushButton *m_delbtn;
#ifdef ANTIQUA_WEBCAM_ENABLED
  QPushButton *m_webcambtn;
#endif

private Q_SLOTS:
  void checkOpen();
#ifdef ANTIQUA_WEBCAM_ENABLED
  void checkWebcam();
#endif
  void checkRemove();

Q_SIGNALS:
  void sendArticleIdChanged();
  void sendOpenWebcam();
  void sendOpenImage();
  void sendDeleteImage(qint64 articleId);

public Q_SLOTS:
  void setArticleId(qint64 id);
  void setActive(bool b = false);
  void enableActions(bool b = false);
  void restoreState();

public:
  explicit ImageToolBar(QWidget *parent = nullptr);
  inline qint64 getArticleId() { return articleId; };
};

#endif // ANTIQUA_IMAGETOOLBAR_H
