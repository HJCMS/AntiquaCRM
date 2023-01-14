// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SPLITTER_H
#define ANTIQUACRM_SPLITTER_H

#include <QMouseEvent>
#include <QObject>
#include <QSplitter>
#include <QSplitterHandle>
#include <QWidget>

class AntiquaSplitterHandle final : public QSplitterHandle {
  Q_OBJECT

protected:
  void mouseReleaseEvent(QMouseEvent *e) override;

Q_SIGNALS:
  void sendSaveState();

public:
  explicit AntiquaSplitterHandle(QSplitter *parent = nullptr);
};

class AntiquaSplitter final : public QSplitter {
  Q_OBJECT

private:
  QString name = QString();
  const QString configPath();

private Q_SLOTS:
  void setSaveState();

protected:
  AntiquaSplitterHandle *createHandle() override;

public:
  explicit AntiquaSplitter(QWidget *parent = nullptr);
  void addLeft(QWidget *widget);
  void addRight(QWidget *widget);
};

#endif // ANTIQUACRM_SPLITTER_H
