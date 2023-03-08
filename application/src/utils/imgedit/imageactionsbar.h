// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_IMAGEACTIONSBAR_H
#define ANTIQUA_IMAGEACTIONSBAR_H

#include <QAction>
#include <QObject>
#include <QToolBar>
#include <QWidget>

/**
 * @ingroup AntiquaImgEdit
 * @brief Image Actions ToolBar
 */
class ImageActionsBar final : public QToolBar {
  Q_OBJECT

private:
  QAction *ac_reset;
  QAction *ac_cut;
  QAction *ac_rotate;
  QAction *ac_scale;
  QAction *ac_save;
  QAction *ac_close;

Q_SIGNALS:
  void sendReset();
  void sendCutting();
  void sendRotate();
  void sendAdjust();
  void sendSave();
  void sendAccept();

public Q_SLOTS:
  void enableCustomActions(bool);

public:
  explicit ImageActionsBar(QWidget *parent = nullptr);
};

#endif // ANTIQUA_IMAGEACTIONSBAR_H
