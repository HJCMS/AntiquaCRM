// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SPLITTER_H
#define ANTIQUACRM_SPLITTER_H

#include <ASettings>
#include <QMouseEvent>
#include <QObject>
#include <QShowEvent>
#include <QSplitter>
#include <QSplitterHandle>
#include <QWidget>

/**
 * @brief AntiquaCRM Splitter handle
 * @ingroup widgets
 */
class AntiquaSplitterHandle final : public QSplitterHandle {
  Q_OBJECT

protected:
  void mouseReleaseEvent(QMouseEvent *e) override;

Q_SIGNALS:
  void sendSaveState();

public:
  explicit AntiquaSplitterHandle(QSplitter *parent = nullptr);
};

/**
 * @brief The Default Horizontal AntiquaCRM Splitter
 */
class AntiquaSplitter final : public QSplitter {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  QString name = QString();

  /**
   * @brief Create and set configPath with ObjectName
   */
  const QString configPath();

  /**
   * @brief Create the Splitterhandle
   */
  AntiquaSplitterHandle *createHandle() override;

  /**
   * @brief Load saveState() on Show
   */
  void showEvent(QShowEvent *) override;

private Q_SLOTS:
  /**
   * @brief saveState() Methode
   */
  void setSaveState();

public:
  explicit AntiquaSplitter(QWidget *parent = nullptr);
  void addLeft(QWidget *widget);
  void addRight(QWidget *widget);
  virtual ~AntiquaSplitter();
};

#endif // ANTIQUACRM_SPLITTER_H
