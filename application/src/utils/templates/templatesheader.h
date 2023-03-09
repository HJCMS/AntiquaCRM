// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_HEADER_H
#define TEMPLATES_HEADER_H

#include <QFrame>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QObject>
#include <QWidget>

/**
 * @ingroup utils
 * @brief Custom Template Edit widget
 */
class TemplatesHeader final : public QFrame {
  Q_OBJECT

private:
  QLineEdit *m_title;
  QLineEdit *m_subject;
  QLineEdit *m_caller;
  QCheckBox *m_attachment;
  QCheckBox *m_activ;

public Q_SLOTS:
  void setSelection(const QJsonObject &);

public:
  explicit TemplatesHeader(QWidget *parent = nullptr);
  const QJsonObject getHeaderData();
};

#endif // TEMPLATES_HEADER_H
