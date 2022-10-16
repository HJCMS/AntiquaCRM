// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABSTATUSBAR_H
#define ANTIQUACRM_TABSTATUSBAR_H

#include <QFrame>
#include <QIcon>
#include <QPushButton>
#include <QStatusBar>

/**
 * @brief The TabStatusBar class
 * @code
 *  Layout:
 *    MessageLabel | QFrame | Reload Button
 * @endcode
 */
class TabStatusBar : public QStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_refresh;

protected:
  QFrame *m_frame;
  const QIcon getIcon(const QString &name) const;

Q_SIGNALS:
  void sendHistoryQuery(const QString &query);
  void sendCreateEntry();
  void sendReloadView();

public:
  enum History {
    Today = 1,
    Yesterday = 2,
    LastWeek = 3,
    LastMonth = 4,
    ThisWeek = 5,
    ThisMonth = 6,
    ThisYear = 7
  };
  explicit TabStatusBar(QWidget *parent = nullptr);
};

Q_DECLARE_METATYPE(TabStatusBar::History)

#endif // ANTIQUACRM_TABSTATUSBAR_H
